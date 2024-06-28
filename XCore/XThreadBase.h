#ifndef XTHREADBASE_H
#define XTHREADBASE_H

namespace XCORE
{
	class XThreadBase
	{
	public:
		virtual void Run() = 0;
		virtual ~XThreadBase() {}

	};
}
#endif