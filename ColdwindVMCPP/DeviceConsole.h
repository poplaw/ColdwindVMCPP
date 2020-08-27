#pragma once

#include <mutex>

#include "Instance.h"
#include "ConsoleWorker.h"

namespace VM
{
	extern class Instance;
	extern class ConsoleWorker;

	class DeviceConsole
	{
		Instance& vm;
		ConsoleWorker* worker;
		std::mutex controlRegisterMutex;
		std::int_fast16_t controlRegister;

	public:
		DeviceConsole(Instance& vm);
		~DeviceConsole();
		void newDataReady();
		void handleInbound(std::int_fast16_t port, std::uint_fast8_t byte);
		void handleOutbound(std::int_fast16_t port);
	};
}

