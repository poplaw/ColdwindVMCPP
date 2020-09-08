#pragma once

#include <mutex>
#include <thread>

#include "Instance.h"
#include "ConsoleWorker.h"
#include "Device.h"

namespace VM
{
	extern class Instance;
	extern class ConsoleWorker;

	class DeviceConsole : public Device
	{
		std::thread *workerThread;
		Instance& vm;
		ConsoleWorker* worker;
		std::mutex controlRegisterMutex;
		std::int_fast16_t controlRegister;

	public:
		DeviceConsole(Instance& vm);
		~DeviceConsole();
		void newDataReady();
		void handleInbound(std::int_fast16_t port, std::uint_fast8_t byte) override;
		char handleOutbound(std::int_fast16_t port) override;
		void terminate();
	};
}

