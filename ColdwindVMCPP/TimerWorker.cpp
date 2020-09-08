#include "TimerWorker.h"

namespace VM
{
	void TimerWorker::setAlarm(const std::chrono::milliseconds& ms)
	{
		alarm = ms;
	}

	void TimerWorker::activate()
	{
		const auto currentTime = std::chrono::system_clock::now();

		std::lock_guard<std::mutex> timeGuard(mutex);
		activationTime = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime.time_since_epoch());
		alarmTime = activationTime + std::chrono::milliseconds((long)(alarm.count() / 1000.0));
		active = true;
	}

	void TimerWorker::deactivate()
	{
		std::lock_guard<std::mutex> timeGuard(mutex);
		active = false;
	}

	std::chrono::milliseconds TimerWorker::getCounter()
	{
		std::lock_guard<std::mutex> timeGuard(mutex);

		const auto currentTime = std::chrono::system_clock::now();

		//const auto res {(int)()}

		return std::chrono::milliseconds();
	}
}
