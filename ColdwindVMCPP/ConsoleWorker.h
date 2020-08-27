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
		bool shallShutdown;
		std::thread thread;
		std::queue<char> queue;
		std::mutex queueMutex;
		DeviceConsole& consoleDev;

	public:
		ConsoleWorker(DeviceConsole& consoleDev);
		void run();
		char getCharacter();
		bool dataReady();
		void terminate();
	};

}
