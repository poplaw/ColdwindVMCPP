#include "DeviceTimer.h"

namespace VM
{
	DeviceTimer::DeviceTimer(Instance& vm)
		: vm(vm), controlRegister(0), worker(new TimerWorker(vm)), remainingCounterValue(0), hasCounterData(false)
	{
		workerThread = new std::thread(&TimerWorker::run, worker);
	}

	DeviceTimer::~DeviceTimer()
	{
		workerThread->join();
		delete worker;
		delete workerThread;
	}

	void DeviceTimer::handleInbound(std::int_fast16_t port, std::uint_fast8_t byte)
	{
		if (port == 0x71)
		{
			worker->alarm = ((worker->alarm << 8) | byte) & 0xffff;
		}
		else if (port == 0x70)
		{
			const auto activationBit{ byte & 1 };
			if (activationBit == 1)
			{
				controlRegister = 1;
				worker->activate();
			}
			else
			{
				controlRegister = 0;
				worker->deactivate();
			}
		}
	}

	char DeviceTimer::handleOutbound(std::int_fast16_t port)
	{
		if (port == 0x70)
		{
			return controlRegister;
		}
		else if (port == 0x71)
		{
			if (!hasCounterData)
			{
				const auto counter{ worker->getCounter() };
				remainingCounterValue = (counter >> 8) & 0xff;
				hasCounterData = true;
				return counter & 0xff;
			}
			else
			{
				hasCounterData = false;
				return remainingCounterValue;
			}
		}
	}

	void DeviceTimer::terminate()
	{
		worker->active = false;
	}
}