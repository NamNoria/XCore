#include <iostream>

#include "XCore.h"
#include "XLog.h"

using namespace XCORE;

#define LOG(x) logger->log(x)

#ifdef INIT_LOG_THREAD
do
{

} while (true);

#endif

int main(void)
{
    Factory factory;
    XLogThread* logger = dynamic_cast<XLogThread*>(factory.createThread(XTHREAD_LOG, SINGLETON));//����ת������
    XMainThread* main = dynamic_cast<XMainThread*>(factory.createThread(XTHREAD_MAIN));

	std::string str = "ceshi rizhi";

    if (logger)
    {

        LOG(str);
        // ģ����߳���־��¼
        std::thread t1([&logger]() {
            for (int i = 0; i < 10; ++i)
            {
                LOG("Log entry from thread 1, message " + std::to_string(i));
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            });

        std::thread t2([&logger]() {
            for (int i = 0; i < 10; ++i)
            {
                LOG("Log entry from thread 2, message " + std::to_string(i));
                std::this_thread::sleep_for(std::chrono::milliseconds(150));
            }
            });

        t1.join();
        t2.join();

        // �ȴ�һ��ʱ������־�̴߳�����������־
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
    else
    {
        std::cerr << "Failed to create XLogThread instance." << std::endl;
    }
	return 0;
}