#ifndef XCORE_H
#define XCORE_H


#include "XThreadBase.h"
#include "XLog.h"

namespace XCORE
{ 
	enum EmClassType
	{
		XTHREAD_LOG,
		XTHREAD_MAIN
	};

	enum EmInstanceType
	{
		NORMAL = 0,
		SINGLETON = 1
	};

	class XMainThread : public XThreadBase
	{
	public:
		void Run()
		{
			std::cout << "Ö÷Ïß³Ì" << std::endl;
		}
	};

	template <typename T>
	class Singleton
	{
	public:
		static T* getInstance()
		{
			if (instance == NULL)
			{
				std::lock_guard<std::mutex> guard(mutex);
				if (instance == NULL)
				{
					instance = new T();
				}
			}
			return instance;
		}

	private:
		Singleton() {}
		~Singleton() {}
		Singleton(const Singleton&);
		Singleton& operator=(const Singleton);

		static T* instance;
		std::thread thread;
		static std::mutex mutex;
	};

	template <typename T> T* Singleton<T>::instance = NULL;
	template <typename T> std::mutex Singleton<T>::mutex;

	class Factory
	{
	public:
		XThreadBase* createThread(EmClassType emClassType, EmInstanceType emInstanceType = NORMAL)
		{
			switch (emClassType)
			{
			case XTHREAD_MAIN:
				return emInstanceType == NORMAL ? static_cast<XThreadBase*>(new XMainThread()) : Singleton<XMainThread>::getInstance();
				break;

			case XTHREAD_LOG:
				return emInstanceType == NORMAL ? static_cast<XThreadBase*>(new XLogThread()) : Singleton<XLogThread>::getInstance();
				break;

			default:
				return NULL;
			}
		}
	private:
	};


}


#endif