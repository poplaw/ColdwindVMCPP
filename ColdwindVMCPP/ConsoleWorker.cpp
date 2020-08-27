#include "ConsoleWorker.h"

namespace VM
{
	ConsoleWorker::ConsoleWorker(DeviceConsole& consoleDev)
		: consoleDev(consoleDev), shallShutdown(false)
	{
	}

	void ConsoleWorker::run()
	{
		/*while(!shallShutdown)*/
	}

	char ConsoleWorker::getCharacter()
	{
		char ch = '\0';
		queueMutex.lock();
		if (queue.size() > 0)
		{
			ch = queue.back();
			queue.pop();
		}

		queueMutex.unlock();

		return ch;
	}

	bool ConsoleWorker::dataReady()
	{
		queueMutex.lock();
		auto res = (queue.size() > 0);
		queueMutex.unlock();
		return res;
	}

	void ConsoleWorker::terminate()
	{
		shallShutdown = true;
	}
}