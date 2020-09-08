#pragma once

#include <thread>
#include <mutex>
#include <queue>

#include "DeviceConsole.h"

namespace VM
{
	extern class DeviceConsole;

	class ConsoleWorker
	{
		std::queue<char> queue;
		std::mutex queueMutex;
		DeviceConsole& consoleDev;
		std::mutex shutdownMutex;
		std::unique_lock<std::mutex> shutdownLock;

	public:
		bool ready;
		ConsoleWorker(DeviceConsole& consoleDev);
		void run();
		char getCharacter();
		bool dataReady();
	};

}
