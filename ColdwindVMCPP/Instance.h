#pragma once

#include "Memory.h"
#include "GeneralPurposeRegister.h"
#include "DeviceConsole.h"
#include "vm_instr.h"

#include <cstdint>
#include <array>
#include <mutex>
#include <map>
#include <string>
#include <deque>
#include <functional>

namespace VM
{
	extern struct InstructionSignature;
	extern class DeviceConsole;
	extern std::map <std::int_fast16_t, InstructionSignature> vm_opcodes;

	class Instance
	{
	public:
		const int
			INSTR_HANDLER = 0,
			INSTR_LENGTH = 1,
			INT_MEMORY_ERROR = 0,
			INT_DIVISION_ERROR = 1,
			INT_GENERAL_ERROR = 2,
			INT_PIT = 8,
			INT_CONSOLE = 9,
			FLAG_ZF = (1 << 0),
			FLAG_CF = (1 << 1),
			CREG_INT_FIRST = 0x100,
			CREG_INT_LAST = 0x10f,
			CREG_INT_CONTROL = 0x110;

		const int MASKABLE_INTS[2] = { 8, 9 };


		std::array<GeneralPurposeRegister, 16> r;
		GeneralPurposeRegister& sp = r[14];
		GeneralPurposeRegister& pc = r[15];
		std::int_fast32_t fr = 0;
		Memory mem;
		bool terminated = false;
		std::map <std::int_fast16_t, InstructionSignature>& opcodes = vm_opcodes;
		std::map<std::uint_fast32_t, std::uint_fast32_t> cr;
		DeviceConsole *deviceConsole;
		std::map<std::int_fast16_t, DeviceConsole*> io;
		std::vector<std::int_fast16_t> interruptQueue;
		std::mutex interruptQueueMutex;
		std::deque<std::function<void(void)>> deferedQueue;


	public:
		Instance();
		GeneralPurposeRegister& reg(std::int_fast16_t r);
		void crash();
		void interrupt(std::int_fast16_t errorCode);
		std::optional<std::int_fast16_t> fetchPendingInterrupt();
		bool processInterruptQueue();
		bool loadMemoryFromFile(std::int_fast32_t addr, std::string name);
		void runSingleStep();
		void run();
	};
}