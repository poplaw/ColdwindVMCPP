#include "ConsoleWorker.h"

namespace VM
{
	ConsoleWorker::ConsoleWorker(DeviceConsole& consoleDev)
		: consoleDev(consoleDev), shutdownLock(shutdownMutex), ready{ false }
	{
		
	}

	void ConsoleWorker::run()
	{
		shutdown.wait(shutdownLock, [this] {return ready; });

		char buffer[1];
		auto ch = fread(buffer, 1, 1, stdin);
		if (ch == '\0')
		{
			return;
		}

		{
			std::lock_guard<std::mutex> guard(queueMutex);
			queue.emplace(ch);
		}

		consoleDev.newDataReady();
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
}