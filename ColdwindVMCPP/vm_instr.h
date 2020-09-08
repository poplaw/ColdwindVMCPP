#pragma once

#include "Instance.h"

#include <map>
#include <functional>

namespace VM
{
	extern class Instance;

	struct InstructionSignature
	{
		std::function<void(Instance&, const std::vector<std::uint_fast8_t>&)> handler;
		std::int_fast16_t length;
		std::string name = "";

		const std::string& getName() const {
			return this->name;
		}
	};

	std::uint_fast16_t to_dw(std::array<std::uint_fast8_t, 2> args);
	std::uint_fast32_t to_dd(std::array<std::uint_fast8_t, 4> args);

	//TODO: union type of 1,2 and 4?
	void VMOV(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VSET(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VLD(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VST(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VLDB(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VSTB(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VADD(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VSUB(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VMUL(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VDIV(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VMOD(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VOR(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VAND(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VXOR(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VNOT(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VSHL(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VSHR(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VCMP(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VJZ(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VJNZ(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VJC(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VJNC(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VJBE(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VJA(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VPUSH(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VPOP(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VJMP(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VJMPR(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VCALL(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VCALLR(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VRET(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VCRL(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VCRS(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VOUTB(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VINB(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VIRET(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VCRSH(Instance& vm, const std::vector<std::uint_fast8_t>& args);
	void VOFF(Instance& vm, const std::vector<std::uint_fast8_t>& args);

	extern std::map <std::int_fast16_t, InstructionSignature> vm_opcodes;
}