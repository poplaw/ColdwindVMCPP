#include "TimerWorker.h"

namespace VM
{
	TimerWorker::TimerWorker(Instance& instance)
		: vm(instance), alarmTime(0), activationTime(0), alarm(0), active(false)
	{

	}

	void TimerWorker::setAlarm(std::int_fast32_t milliseconds)
	{
		alarm = milliseconds;
	}

	void TimerWorker::activate()
	{
		const auto timeSinceEpoch{ std::chrono::system_clock::now().time_since_epoch() };
		const auto secondsSinceEpoch{ std::chrono::duration_cast<std::chrono::seconds>(timeSinceEpoch).count() };

		std::lock_guard<std::mutex> timeGuard(mutex);
		activationTime = secondsSinceEpoch;
		alarmTime = activationTime + alarm / 1000.0;
		active = true;
	}

	void TimerWorker::deactivate()
	{
		std::lock_guard<std::mutex> timeGuard(mutex);
		active = false;
	}

	std::int_fast32_t TimerWorker::getCounter()
	{
		std::lock_guard<std::mutex> timeGuard(mutex);

		const auto timeSinceEpoch{ std::chrono::system_clock::now().time_since_epoch() };
		const auto secondsSinceEpoch{ std::chrono::duration_cast<std::chrono::seconds>(timeSinceEpoch).count() };

		const auto res{ static_cast<std::int_fast32_t>((secondsSinceEpoch - alarmTime) * 1000)};

		return res;
	}

	void TimerWorker::run()
	{
		using namespace std::chrono_literals;

		while (active)
		{
			const auto timeSinceEpoch{ std::chrono::system_clock::now().time_since_epoch() };
			const auto secondsSinceEpoch{ std::chrono::duration_cast<std::chrono::seconds>(timeSinceEpoch).count() };

			mutex.lock();
			if (active && secondsSinceEpoch >= alarmTime)
			{
				active = false;
				mutex.unlock();
				vm.interrupt(vm.INT_PIT);
			}
			else
			{
				mutex.unlock();
			}

			std::this_thread::sleep_for(0s);
		}
	}
}
