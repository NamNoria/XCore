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
		XLogThread();
		~XLogThread();

		void Run();

		void log(const std::string& message);

	private:
		bool running;
		std::mutex mutex;
		std::condition_variable cond;
		std::queue<std::string> logQueue;
		std::thread thread;
	};
}

#endif