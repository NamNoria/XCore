#include <ctime>

#include "XLog.h"

XCORE::XLogThread::XLogThread()
	: running(true), thread(&XLogThread::Run, this)
{
}

void XCORE::XLogThread::Run()
{
	while (running)
	{
		std::unique_lock<std::mutex> lock(mutex);
		cond.wait(lock, [this] { return !logQueue.empty() || !running; });

		while (!logQueue.empty())
		{
			std::time_t now = std::time(nullptr);
			struct tm timeinfo;
			localtime_s(&timeinfo, &now);
			char buf[80];
			std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);

			std::string logEntry = "[" + std::string(buf) + "] " + logQueue.front();
			std::cout << logEntry << std::endl;
			logQueue.pop();

			std::ofstream file("logfile.txt", std::ios_base::app);
			file << logEntry << std::endl;
			file.close();
		}
	}
}

void XCORE::XLogThread::log(const std::string & message)
{
	std::unique_lock<std::mutex> lock(mutex);
	logQueue.push(message);
	lock.unlock();
	cond.notify_one();
}

XCORE::XLogThread::~XLogThread()
{
	running = false;
	cond.notify_one();
	if (thread.joinable())
	{
		thread.join();
	}
}