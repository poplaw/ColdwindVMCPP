#include "vm_instr.h"

#include <array>
#include <iostream>

namespace VM
{
	std::uint_fast16_t to_dw(std::array<std::uint_fast8_t, 2> args)
	{
		std::uint_fast16_t returnValue{ 0 };

		returnValue = (
			(returnValue | args[0]) |
			(returnValue | args[1]) << 8
		);

		return returnValue;
	}

	std::uint_fast32_t to_dd(std::array<std::uint_fast8_t, 4> args)
	{
		std::uint_fast32_t returnValue{ 0 };

		returnValue = (
			(returnValue | args[0]) |
			((returnValue | args[1]) << 8) | 
			((returnValue | args[2]) << 16) |
			((returnValue | args[3]) << 24)
		);

		return returnValue;
	}

	void VMOV(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value = vm.reg(args[1]).value;
	}

	void VSET(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		std::array<std::uint_fast8_t, 4> buffer;
		for (int i{ 1 }; i < 5; ++i)
		{
			buffer[i - 1] = args[i];
		}

		vm.reg(args[0]).value = to_dd(buffer);
	}

	void VLD(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto dd = vm.mem.fetchDword(vm.reg(args[1]).value);

		if (dd)
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
			return;
		}
		vm.reg(args[0]).value = dd.value();
	}

	void VST(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (!vm.mem.storeDword(vm.reg(args[0]).value, vm.reg(args[1]).value))
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
		}
	}

	void VLDB(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto db = vm.mem.fetchByte(vm.reg(args[1]).value);
		if (!db)
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
			return;
		}
		vm.reg(args[0]).value = db.value();
	}

	void VSTB(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (!vm.mem.storeByte(vm.reg(args[0]).value, vm.reg(args[1]).value & 0xff))
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
		}
	}

	void VADD(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value = ((vm.reg(args[0]).value + vm.reg(args[1]).value) & 0xffffffff);
	}

	void VSUB(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value = ((vm.reg(args[0]).value - vm.reg(args[1]).value) & 0xffffffff);
	}

	void VMUL(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value = ((vm.reg(args[0]).value * vm.reg(args[1]).value) & 0xffffffff);
	}

	void VDIV(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (vm.reg(args[1]).value == 0)
		{
			vm.interrupt(vm.INT_DIVISION_ERROR);
		}
		else
		{
			vm.reg(args[0]).value = (vm.reg(args[0]).value / vm.reg(args[1]).value);
		}
	}

	void VMOD(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (vm.reg(args[1]).value == 0)
		{
			vm.interrupt(vm.INT_DIVISION_ERROR);
		}
		else
		{
			vm.reg(args[0]).value = (vm.reg(args[0]).value & vm.reg(args[1]).value);
		}
	}

	void VOR(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value |= vm.reg(args[1]).value;
	}

	void VAND(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value &= vm.reg(args[1]).value;
	}

	void VXOR(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value ^= vm.reg(args[1]).value;
	}

	void VNOT(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value = vm.reg(args[0]).value ^ 0xffffffff;
	}

	void VSHL(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value = ((vm.reg(args[0]).value << (vm.reg(args[1]).value & 0x1f)) & 0xffffffff);
	}

	void VSHR(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.reg(args[0]).value = ((vm.reg(args[0]).value >> (vm.reg(args[1]).value & 0x1f)));
	}

	void VCMP(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto res = vm.reg(args[0]).value - vm.reg(args[1]).value;
		vm.fr &= 0xfffffffc;

		if (res == 0)
		{
			vm.fr |= vm.FLAG_ZF;
		}

		if (res < 0)
		{
			vm.fr |= vm.FLAG_CF;
		}
	}

	void VJZ(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (vm.fr & vm.FLAG_ZF)
		{
			std::array<std::uint_fast8_t, 2> buffer;
			for (int i{ 0 }; i < 2; ++i)
			{
				buffer[i] = args[i];
			}

			vm.pc.value = (vm.pc.value + to_dw(buffer));
		}
	}

	void VJNZ(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (!(vm.fr & vm.FLAG_ZF))
		{
			std::array<std::uint_fast8_t, 2> buffer;
			for (int i{ 0 }; i < 2; ++i)
			{
				buffer[i] = args[i];
			}

			vm.pc.value = (vm.pc.value + to_dw(buffer));
		}
	}

	void VJC(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (vm.fr & vm.FLAG_CF)
		{
			std::array<std::uint_fast8_t, 2> buffer;
			for (int i{ 0 }; i < 2; ++i)
			{
				buffer[i] = args[i];
			}

			vm.pc.value = (vm.pc.value + to_dw(buffer));
		}
	}

	void VJNC(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (!(vm.fr & vm.FLAG_CF))
		{
			std::array<std::uint_fast8_t, 2> buffer;
			for (int i{ 0 }; i < 2; ++i)
			{
				buffer[i] = args[i];
			}

			vm.pc.value = (vm.pc.value + to_dw(buffer));
		}
	}

	void VJBE(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if ((vm.fr & vm.FLAG_CF) or (vm.fr & vm.FLAG_ZF))
		{
			std::array<std::uint_fast8_t, 2> buffer;
			for (int i{ 0 }; i < 2; ++i)
			{
				buffer[i] = args[i];
			}

			vm.pc.value = (vm.pc.value + to_dw(buffer));
		}
	}

	void VJA(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		if (!(vm.fr & vm.FLAG_CF) && !(vm.fr & vm.FLAG_ZF))
		{
			std::array<std::uint_fast8_t, 2> buffer;
			for (int i{ 0 }; i < 2; ++i)
			{
				buffer[i] = args[i];
			}

			vm.pc.value = (vm.pc.value + to_dw(buffer));
		}
	}

	void VPUSH(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.sp.value -= 4;
		if (!vm.mem.storeDword(vm.sp.value, vm.reg(args[0]).value))
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
		}
	}

	void VPOP(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto res = vm.mem.fetchDword(vm.sp.value);

		if (!res)
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
			return;
		}

		vm.reg(args[0]).value = res.value();
		vm.sp.value += 4;
	}

	void VJMP(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		std::array<std::uint_fast8_t, 2> buffer;
		for (int i{ 0 }; i < 2; ++i)
		{
			buffer[i] = args[i];
		}

		vm.pc.value = (vm.pc.value + to_dw(buffer)) & 0xffff;
	}

	void VJMPR(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.pc.value = vm.reg(args[0]).value & 0xffff;
	}

	void VCALL(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.sp.value -= 4;
		if (!vm.mem.storeDword(vm.sp.value, vm.pc.value))
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
			return;
		}

		std::array<std::uint_fast8_t, 2> buffer;
		for (int i{ 0 }; i < 2; ++i)
		{
			buffer[i] = args[i];
		}

		vm.pc.value = (vm.pc.value + to_dw(buffer)) & 0xffff;
	}

	void VCALLR(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.sp.value -= 4;
		if (!vm.mem.storeDword(vm.sp.value, vm.pc.value))
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
			return;
		}

		vm.pc.value = vm.reg(args[0]).value & 0xffff;
	}

	void VRET(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto res = vm.mem.fetchDword(vm.sp.value);
		if (!res)
		{
			vm.interrupt(vm.INT_MEMORY_ERROR);
			return;
		}
		vm.pc.value = res.value();
		vm.sp.value += 4;
	}

	void VCRL(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto v = vm.reg(args[0]).value;
		std::array<std::uint_fast8_t, 2> buffer;
		for (int i{ 0 }; i < 2; ++i)
		{
			buffer[i] = args[i + 1];
		}

		auto cr_id = to_dw(buffer);
		if (vm.cr.find(cr_id) == vm.cr.end())
		{
			vm.interrupt(vm.INT_GENERAL_ERROR);
			return;
		}
	
		auto deferredLoad = [cr_id, v, &vm] {
			vm.cr[cr_id] = v; 
		};

		vm.deferedQueue.push_back(deferredLoad);
	}

	void VCRS(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		std::array<std::uint_fast8_t, 2> buffer;
		for (int i{ 0 }; i < 2; ++i)
		{
			buffer[i] = args[i + 1];
		}
		auto cr_id = to_dw(buffer);

		if (vm.cr.find(cr_id) == vm.cr.end())
		{
			vm.interrupt(vm.INT_GENERAL_ERROR);
			return;
		}
		vm.reg(args[0]).value = vm.cr[cr_id];
	}

	void VOUTB(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto port = args[1];

		if (vm.io.find(port) == vm.io.end())
			return;

		vm.io[port]->handleInbound(port, vm.reg(args[0]).value & 0xff);
	}

	void VINB(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto port = args[1];
		if (vm.io.find(port) == vm.io.end())
		{
			return;
		}

		vm.reg(args[0]).value = vm.io.at(port)->handleOutbound(port) & 0xff;
	}

	void VIRET(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		auto tmp_sp = vm.sp.value;

		for (int i{ 16 }; i >= 0; --i)
		{
			auto v = vm.mem.fetchDword(tmp_sp);
			if (!v)
			{
				vm.interrupt(vm.INT_GENERAL_ERROR);
				return;
			}
			if (i == 16)
			{
				vm.fr = v.value();
			}
			else
			{
				vm.r[i].value = v.value();
			}
			tmp_sp += 4;
		}
	}

	void VCRSH(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.crash();
	}

	void VOFF(Instance& vm, const std::vector<std::uint_fast8_t>& args)
	{
		vm.terminated = true;
	}

	std::map <std::int_fast16_t, InstructionSignature> vm_opcodes = {
		{ 0x00, {VMOV, 1 + 1, "VMOV"}},
		{ 0x01, {VSET, 1 + 4, "VSET"}},
		{ 0x02, {VLD, 1 + 1, "VLD"}},
		{ 0x03, {VST, 1 + 1, "VST"}},
		{ 0x04, {VLDB, 1 + 1, "VLDB"}},
		{ 0x05, {VSTB, 1 + 1, "VSTB"}},
		{ 0x10, {VADD, 1 + 1, "VADD"}},
		{ 0x11, {VSUB, 1 + 1, "VSUB"}},
		{ 0x12, {VMUL, 1 + 1, "VMUL"}},
		{ 0x13, {VDIV, 1 + 1, "VDIV"}},
		{ 0x14, {VMOD, 1 + 1, "VMOD"}},
		{ 0x15, {VOR, 1 + 1, "VOR"}},
		{ 0x16, {VAND, 1 + 1, "VAND"}},
		{ 0x17, {VXOR, 1 + 1, "VXOR"}},
		{ 0x18, {VNOT, 1, "VNOT"}},
		{ 0x19, {VSHL, 1 + 1, "VSHL"}},
		{ 0x1A, {VSHR, 1 + 1, "VSHR"}},
		{ 0x20, {VCMP, 1 + 1, "VCMP"}},
		{ 0x21, {VJZ, 2, "VJZ"}},
		{ 0x22, {VJNZ, 2, "VJNZ"}},
		{ 0x23, {VJC, 2, "VJC"}},
		{ 0x24, {VJNC, 2, "VJNC"}},
		{ 0x25, {VJBE, 2, "VJBE"}},
		{ 0x26, {VJA, 2, "VJA"}},
		{ 0x30, {VPUSH, 1, "VPUSH"}},
		{ 0x31, {VPOP, 1, "VPOP"}},
		{ 0x40, {VJMP, 2, "VJMP"}},
		{ 0x41, {VJMPR, 1, "VJMPR"}},
		{ 0x42, {VCALL, 2, "VCALL"}},
		{ 0x43, {VCALLR, 1, "VCALLR"}},
		{ 0x44, {VRET, 0, "VRET"}},
		{ 0xF0, {VCRL, 1 + 2, "VCRL"}},
		{ 0xF1, {VCRS, 1 + 2, "VCRS"}},
		{ 0xF2, {VOUTB, 1 + 1, "VOUTB"}},
		{ 0xF3, {VINB, 1 + 1, "VINB"}},
		{ 0xF4, {VIRET, 0, "VIRET"}},
		{ 0xFE, {VCRSH, 0, "VCRSH"}},
		{ 0xFF, {VOFF, 0, "VOFF"}}
	};
}