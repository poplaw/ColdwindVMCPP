#include "ConsoleWorker.h"

namespace VM
{
	ConsoleWorker::ConsoleWorker(DeviceConsole& consoleDev)
		: consoleDev(consoleDev), shutdownLock(shutdownMutex), ready{ false }
	{
		
	}

	void ConsoleWorker::run()
	{
		//shutdown.wait(shutdownLock, [this] {return ready; });

		while (!ready)
		{
			char buffer[1];
			fread(buffer, sizeof(char), 1, stdin);
			auto ch = buffer[0];
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

	}

	char ConsoleWorker::getCharacter()
	{
		char ch = 0;
		queueMutex.lock();
		if (queue.size() > 0)
		{
			ch = queue.front();
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