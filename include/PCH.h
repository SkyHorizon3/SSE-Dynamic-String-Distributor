#pragma once
#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"

#include <unordered_map>
#include <ShlObj.h>
#include <tuple>

#include <spdlog/sinks/basic_file_sink.h>
#include <xbyak/xbyak.h>
#include <SimpleIni.h>
#include <ankerl/unordered_dense.h>

template <>
struct ankerl::unordered_dense::hash<std::string>
{
	using is_transparent = void;  // enable heterogeneous overloads
	using is_avalanching = void;  // mark class as high quality avalanching hash

	[[nodiscard]] auto operator()(std::string_view str) const noexcept -> uint64_t
	{
		return ankerl::unordered_dense::hash<std::string_view>{}(str);
	}
};

using namespace std::literals;

namespace stl
{
	using namespace SKSE::stl;

	template <class T, size_t size = 5>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		T::func = trampoline.write_call<size>(a_src, T::thunk);
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

	template <class T, std::size_t BYTES>
	void hook_function_prologue(std::uintptr_t a_src)
	{
		struct Patch : Xbyak::CodeGenerator
		{
			Patch(std::uintptr_t a_originalFuncAddr, std::size_t a_originalByteLength)
			{
				// Hook returns here. Execute the restored bytes and jump back to the original function.
				for (size_t i = 0; i < a_originalByteLength; ++i) {
					db(*reinterpret_cast<std::uint8_t*>(a_originalFuncAddr + i));
				}

				jmp(qword[rip]);
				dq(a_originalFuncAddr + a_originalByteLength);
			}
		};

		Patch p(a_src, BYTES);
		p.ready();

		auto& trampoline = SKSE::GetTrampoline();
		trampoline.write_branch<5>(a_src, T::thunk);

		auto alloc = trampoline.allocate(p.getSize());
		std::memcpy(alloc, p.getCode(), p.getSize());

		T::func = reinterpret_cast<std::uintptr_t>(alloc);
	}
}

//https://github.com/powerof3/CLibUtil/blob/master/include/CLIBUtil/singleton.hpp
template <class T>
class ISingleton
{
public:
	static T* GetSingleton()
	{
		static T singleton;
		return std::addressof(singleton);
	}

protected:
	ISingleton() = default;
	~ISingleton() = default;

	ISingleton(const ISingleton&) = delete;
	ISingleton(ISingleton&&) = delete;
	ISingleton& operator=(const ISingleton&) = delete;
	ISingleton& operator=(ISingleton&&) = delete;
};

#include "Plugin.h"