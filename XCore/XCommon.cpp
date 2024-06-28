#include <iostream>
#include <ctime>

#include "XCommon.h"

char* GetLocalTime()
{
	std::time_t now = std::time(nullptr);
	struct tm timeinfo;
	localtime_s(&timeinfo, &now);
	char buffer[0x100] = {0};
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &timeinfo);
	char* pTime = (char*)new char[strlen(buffer) + 1];
	if (!pTime)
	{
		return nullptr;
	}
	memset(pTime, 0, strlen(buffer) + 1);
	strncpy(pTime, buffer, strlen(buffer));
	return pTime;
}
