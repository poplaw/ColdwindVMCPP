#pragma once

#include <mutex>
#include <thread>

#include "Instance.h"
#include "TimerWorker.h"
#include "Device.h"

namespace VM
{
	extern class Instance;
	extern class TimerWorker;

	class DeviceTimer : public Device
	{
		std::thread* workerThread;
		Instance& vm;
		TimerWorker* worker;
		std::int_fast16_t controlRegister;
		std::int_fast16_t remainingCounterValue;
		bool hasCounterData = false;

	public:
		DeviceTimer(Instance& vm);
		~DeviceTimer();
		void handleInbound(std::int_fast16_t port, std::uint_fast8_t byte) override;
		char handleOutbound(std::int_fast16_t port) override;
		void terminate();
	};
}
