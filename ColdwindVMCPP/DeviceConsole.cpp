#include "DeviceConsole.h"

namespace VM
{
	DeviceConsole::DeviceConsole(Instance& vm)
		: vm(vm), controlRegister(0), worker(new ConsoleWorker(*this))
	{
		//worker->start();
	}

	DeviceConsole::~DeviceConsole()
	{
		delete worker;
	}

	void DeviceConsole::newDataReady()
	{
		controlRegisterMutex.lock();

		if (controlRegister & 1)
		{
			controlRegister &= 0xfffffffe;
			controlRegisterMutex.unlock();
			vm.interrupt(9);
		}
		else
		{
			controlRegisterMutex.unlock();
		}
	}

	void DeviceConsole::handleInbound(std::int_fast16_t port, std::uint_fast8_t byte)
	{
		if (port == 0x20)
		{
			std::printf("%c", byte);
		}
		else if (port == 0x21)
		{
			;
		}
		else if (port == 0x22)
		{
			controlRegisterMutex.lock();
			controlRegister = byte;
			controlRegisterMutex.unlock();
		}
	}

	void DeviceConsole::handleOutbound(std::int_fast16_t port)
	{
		if (port == 0x20)
		{
			while (true)
			{
				char ch = worker->getCharacter();
				if (ch != '\0')
				{
					break;
				}
				
			}
		}
	}
}