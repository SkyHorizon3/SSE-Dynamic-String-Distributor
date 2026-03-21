#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include "RE/Skyrim.h"
#include "REX/REX/Singleton.h"
#include "SKSE/SKSE.h"

#include <MergeMapperPluginAPI.h>
#include <boost/unordered/unordered_node_map.hpp>
#include <xbyak/xbyak.h>
#include <SimpleIni.h>
#include <glaze/json.hpp>
#include <ClibUtil/string.hpp>
#include "RE.h"

using namespace std::literals;
using namespace clib_util::string::literals;
namespace string = clib_util::string;
namespace hash = clib_util::hash;

namespace stl
{
	using namespace SKSE::stl;

	template <class T, size_t size = 5>
	void write_thunk_call(std::uintptr_t a_src)
	{
		auto& trampoline = SKSE::GetTrampoline();
		T::func = trampoline.write_call<size>(a_src, T::thunk);
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

				jmp(ptr[rip]);
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

	template <class T>
	void write_thunk_lea(std::uintptr_t a_src) //only use on x64, e.g. rexw (0x48), rexrw (0x4C)
	{
		auto bytes = a_src;
		const auto opCode = *reinterpret_cast<std::uint8_t*>(++bytes);

		if (opCode == 0x8D) // check if it's lea
		{
			const auto operand1 = *reinterpret_cast<std::uint8_t*>(++bytes); // mostly 0x05 in case of lea
			const auto writeAddress = bytes;

			// get original displacement
			std::int32_t disp = 0;
			for (std::uint8_t i = 0; i < 4; ++i)
			{
				disp |= *reinterpret_cast<std::uint8_t*>(++bytes) << (i * 8);
			}

			SKSE::GetTrampoline().write_call<5>(writeAddress, T::thunk); // overwrite last 5 bytes of lea instruction

			REL::safe_write(writeAddress, operand1); // write back the operand which got modified by write_call

			T::func = a_src + 7 + disp; // address + lea size + displacement
		}
	}
}

template <class K, class D, class H = boost::hash<K>, class KEqual = std::equal_to<K>>
using FlatMap = boost::unordered_node_map<K, D, H, KEqual>;

struct string_hash
{
	using is_transparent = void;  // enable heterogeneous overloads

	std::size_t operator()(std::string_view str) const
	{
		std::size_t seed = 0;
		for (auto it = str.begin(); it != str.end(); ++it) {
			boost::hash_combine(seed, std::tolower(*it));
		}
		return seed;
	}
};

struct string_cmp
{
	using is_transparent = void;  // enable heterogeneous overloads

	bool operator()(const std::string& str1, const std::string& str2) const
	{
		return string::iequals(str1, str2);
	}
	bool operator()(std::string_view str1, std::string_view str2) const
	{
		return string::iequals(str1, str2);
	}
};

template <class D>
using StringMap = FlatMap<std::string, D, string_hash, string_cmp>;

#include "Plugin.h"