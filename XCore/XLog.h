#ifndef XLOG_H
#define XLOG_H

#include <string>
#include <iostream>
#include <fstream>
#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>


#include "XThreadBase.h"

namespace XCORE
{
	class XLogThread : public XThreadBase
	{
	public:
		XLogThread()
			: running(true), thread(&XLogThread::Run, this)
		{}
		void Run()
		{
			while (running)
			{
				std::unique_lock<std::mutex> lock(mutex);
				cond.wait(lock, [this] { return !logQueue.empty() || !running; });

				while (!logQueue.empty())
				{
					std::ofstream file("logfile.txt", std::ios_base::app);
					file << logQueue.front() << std::endl;
					file.close();

					std::cout << "Logged: " << logQueue.front() << std::endl;
					logQueue.pop();
				}
			}
		}

		void log(const std::string& message)
		{
			std::unique_lock<std::mutex> lock(mutex);
			logQueue.push(message);
			lock.unlock();
			cond.notify_one();
		}


		~XLogThread()
		{
			running = false;
			cond.notify_one();
			if (thread.joinable())
			{
				thread.join();
			}
		}
	private:
		bool running;
		std::mutex mutex;
		std::condition_variable cond;
		std::queue<std::string> logQueue;
		std::thread thread;
	};
}

#endif