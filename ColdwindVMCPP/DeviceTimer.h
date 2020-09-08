#pragma once

#include <mutex>
#include <thread>

#include "Instance.h"

namespace VM
{
	extern class Instance;
	extern class TimeWorker;

	class DeviceTimer
	{
		std::thread* workerThread;
		Instance& vm;
		TimeWorker* worker;
		std::int_fast16_t controlRegister;
		std::int_fast16_t remainingCounterValue;
		bool hasCounterData = false;

	public:
		DeviceTimer(Instance& vm);
		~DeviceTimer();
		void handleInbound(std::int_fast16_t port, std::uint_fast8_t byte);
		char handleOutbound(std::int_fast16_t port);
		void terminate();
	};
}
