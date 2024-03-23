#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <unordered_map>
#include <ShlObj.h>
#include <tuple>

#include <spdlog/sinks/basic_file_sink.h>
#include <xbyak/xbyak.h>
#include <nlohmann/json.hpp>
#include <ankerl/unordered_dense.h>

using namespace std::literals;

namespace stl
{
	using namespace SKSE::stl;

	template <class T>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		SKSE::AllocTrampoline(14);

		T::func = trampoline.write_call<5>(a_src, T::thunk);
	}

	template <class F, size_t offset, class T>
	void write_vfunc()
	{
		REL::Relocation<std::uintptr_t> vtbl{ F::VTABLE[offset] };
		T::func = vtbl.write_vfunc(T::idx, T::thunk);
	}

	template <class F, class T>
	void write_vfunc()
	{
		write_vfunc<F, 0, T>();
	}
}

#include "Plugin.h"