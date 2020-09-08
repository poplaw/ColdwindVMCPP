#pragma once

#include "Instance.h";

#include <thread>
#include <mutex>
#include <chrono>

namespace VM
{
	extern class Instance;

	class TimerWorker
	{
		std::mutex mutex;
		Instance& instance;
		std::chrono::milliseconds alarmTime;
		std::chrono::milliseconds activationTime;
		bool active;
		std::chrono::milliseconds alarm;

	public:
		std::condition_variable shutdown;
		TimerWorker();
		void setAlarm(const std::chrono::milliseconds& ms);
		void activate();
		void deactivate();
		std::chrono::milliseconds getCounter();
	};
}

