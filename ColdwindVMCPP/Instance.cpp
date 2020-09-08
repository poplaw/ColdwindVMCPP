#include "Instance.h"

#include <iostream>
#include <fstream>
#include <array>
#include <sstream>
#include <iterator>
#include <iomanip>

namespace VM
{
	Instance::Instance()
		: mem(Memory()), deviceConsole(new DeviceConsole(*this)), deviceTimer(new DeviceTimer(*this))
	{
		sp.value = 0x10000;

		for (int i{ CREG_INT_FIRST }; i <= CREG_INT_LAST + 1; ++i)
		{
			cr.insert(std::make_pair(i, 0xffffffff));
		}

		cr[CREG_INT_CONTROL] = 0;

		io.insert(
			std::make_pair(0x20, deviceConsole)
		);
		io.insert(
			std::make_pair(0x21, deviceConsole)
		);
		io.insert(
			std::make_pair(0x22, deviceConsole)
		);
		io.insert(
			std::make_pair(0x70, deviceTimer)
		);
		io.insert(
			std::make_pair(0x71, deviceTimer)
		);
	}

	GeneralPurposeRegister& Instance::reg(std::int_fast16_t r)
	{
		return this->r[r & 0xf];
	}

	void Instance::crash()
	{
		this->terminated = true;
		std::cout << "The virtual machine entered an erroneous state and is terminating.\n";
		std::cout << "Register values at termination: ";

		for (int i{ 0 }; i < r.size(); ++i)
		{
			std::cout << " r" << i << " = " << r[i].value << '\n';
		}
	}

	void Instance::interrupt(std::int_fast16_t errorCode)
	{
		std::lock_guard<std::mutex> guard(this->interruptQueueMutex);
		this->interruptQueue.emplace_back(errorCode);
	}

	std::optional<std::int_fast16_t> Instance::fetchPendingInterrupt()
	{
		std::lock_guard<std::mutex> guard(this->interruptQueueMutex);
		if (interruptQueue.size() < 1)
			return std::nullopt;

		if ((cr[CREG_INT_CONTROL] & 1) == 0)
		{
			for (int i{ 0 }; i < interruptQueue.size(); ++i)
			{
				bool contains = false;
				for (int j{ 0 }; j < std::size(MASKABLE_INTS); ++j)
				{
					if (MASKABLE_INTS[j] == interruptQueue[i])
					{
						contains = true;
						break;
					}
				}

				if (!contains)
				{
					auto value = interruptQueue[i];
					interruptQueue.erase(interruptQueue.begin() + i);
					return value;
				}
			}

			return std::nullopt;
		}

		auto value = interruptQueue.back();
		interruptQueue.pop_back();
		return value;
	}

	bool Instance::processInterruptQueue()
	{
		auto i = fetchPendingInterrupt();

		if (!i) return true;

		auto tmp_sp = sp.value;

		for (const auto& r : this->r)
		{
			tmp_sp -= 4;
			if (!mem.storeDword(tmp_sp, r.value))
			{
				crash();
				return false;
			}
		}

		tmp_sp -= 4;
		if (!mem.storeDword(tmp_sp, fr))
		{
			crash();
			return false;
		}

		sp.value = tmp_sp;
		pc.value = cr[CREG_INT_FIRST + (i.value() & 0xf)];
		cr[CREG_INT_CONTROL] &= 0xfffffffe;
		return true;
	}

	bool Instance::loadMemoryFromFile(std::int_fast32_t addr, std::string name)
	{
		std::ifstream input(name, std::ios::binary);

		std::vector<std::uint_fast8_t> buffer(std::istreambuf_iterator<char>(input), {});

		return mem.storeMany(addr, buffer);
	}

	void Instance::runSingleStep()
	{
		if (!processInterruptQueue())
			return;
		
		while (!deferedQueue.empty())
		{
			auto action = deferedQueue.back();
			deferedQueue.pop_back();
			action();
		}

		auto opcode = mem.fetchByte(pc.value);

		if (!opcode)
		{
			interrupt(INT_MEMORY_ERROR);
			return;
		}

		if (opcodes.find(opcode.value()) == opcodes.end())
		{
			interrupt(INT_GENERAL_ERROR);
			return;
		}

		auto length = opcodes[opcode.value()].length;
		auto argumentBytes = mem.fetchMany(pc.value + 1, length);
		if (!argumentBytes)
		{
			interrupt(INT_MEMORY_ERROR);
			return;
		}

		auto handler = opcodes[opcode.value()].handler;

		std::stringstream stream{};
		std::for_each(argumentBytes.value().begin(), argumentBytes.value().end(),
			[&stream](std::uint_fast8_t x)->void {stream << std::hex << std::setfill('0') << std::setw(2) << (int)x; }
		);

		//std::printf("%04x\t%s\t%s\n", pc.value, opcodes[opcode.value()].getName().c_str(), stream.str().c_str());
		pc.value += (1 + length);
		handler(*this, argumentBytes.value());
	}

	void Instance::run()
	{
		while (!terminated)
			runSingleStep();

		deviceConsole->terminate();

		exit(0);
	}
}