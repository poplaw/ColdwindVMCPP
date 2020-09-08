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
		Instance& vm;
		std::int_fast32_t alarmTime;
		std::int_fast32_t activationTime;

	public:
		bool active;
		std::int_fast32_t alarm;
		TimerWorker(Instance& instance);
		void setAlarm(std::int_fast32_t milliseconds);
		void activate();
		void deactivate();
		std::int_fast32_t getCounter();
		void run();
	};
}

