#include "MiscHooks.h"
#include "Utils.h"
#include "DescriptionHooks.h"
#include "Manager.h"

namespace Hook
{

	// Track if an object is valid between the two AE hooks
	static bool IsDESC = false;
	std::string SetDescription = "";

	struct DescriptionHookAE // XXXX DESC
	{
		struct TrampolineCall : Xbyak::CodeGenerator
		{
			TrampolineCall(std::uintptr_t retn, std::uintptr_t func)
			{
				Xbyak::Label funcLabel;
				Xbyak::Label retnLabel;

				mov(rcx, rdi);

				sub(rsp, 0x20); //allocate for call
				call(ptr[rip + funcLabel]); //call thunk
				add(rsp, 0x20);

				mov(dword[r14], 0x65); // original code

				jmp(ptr[rip + retnLabel]); //jump back to original code

				L(funcLabel);
				dq(func);

				L(retnLabel);
				dq(retn);
			}
		};

		static void thunk(RE::TESForm* a_parent)
		{
			if (a_parent)
			{
				std::string newDescription{};

				if (Manager::GetSingleton()->getDESC(a_parent->formID, newDescription))
				{
					IsDESC = true;
					SetDescription = newDescription;

					SKSE::log::debug("Replaced XXXX DESC {0:08X} with:", a_parent->formID);
					SKSE::log::debug("{}", newDescription);
				}
			}
		}

		static void Install()
		{
			constexpr auto targetAddress = REL::ID(14552);

			REL::safe_fill(targetAddress.address() + 0x84, REL::NOP, 0x7); // just in case

			REL::Relocation<std::uintptr_t> target{ targetAddress, 0x84 };
			auto trampolineJmp = TrampolineCall(target.address() + 0x7, stl::unrestricted_cast<std::uintptr_t>(thunk));

			auto& trampoline = SKSE::GetTrampoline();
			auto result = trampoline.allocate(trampolineJmp);
			trampoline.write_branch<6>(target.address(), (std::uintptr_t)result);
		}
	};

	struct GetDescriptionHookAE
	{
		static void thunk(RE::BSString& a_out, std::uint64_t a2, std::uint64_t a3)
		{
			func(a_out, a2, a3);

			if (IsDESC && !a_out.empty()) //Don't replace anything thats empty. This stops the hook from also replacing BOOK CNAM with BOOK DESC if there is no CNAM.
			{
				a_out = SetDescription;
			}
			IsDESC = false;

		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{

			REL::Relocation<std::uintptr_t> target01{ REL::ID(14552), 0xFB };
			stl::write_thunk_call<GetDescriptionHookAE>(target01.address());

			REL::Relocation<std::uintptr_t> target02{ REL::ID(14552), 0x174 };
			stl::write_thunk_call<GetDescriptionHookAE>(target02.address());

			/*
			REL::Relocation<std::uintptr_t> target03{ REL::ID(14552),0x93 };
			stl::write_thunk_call<GetDescriptionHookAE>(target03.address());
			SKSE::log::info("GetDescriptionHookAE hooked at address: {:x} and offset: {:x}", target03.address(), target03.offset());
			*/
		}

	};

	struct GetDescriptionHookSE
	{
		static void thunk(RE::TESDescription* a_description, RE::BSString& a_out, RE::TESForm* a_parent, std::uint32_t unk)
		{
			func(a_description, a_out, a_parent, unk);  // invoke original to get original description string output

			if (a_parent && !a_out.empty())
			{
				std::string newDescription{};

				if (Manager::GetSingleton()->getDESC(a_parent->formID, newDescription))
				{
					a_out = newDescription;

					SKSE::log::debug("Replaced XXXX DESC {0:08X} with:", a_parent->formID);
					SKSE::log::debug("{}", newDescription);
				}
			}
		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			REL::Relocation<std::uintptr_t> target1{ REL::ID(14399), 0x53 };
			stl::write_thunk_call<GetDescriptionHookSE>(target1.address());

			REL::Relocation<std::uintptr_t> target2{ REL::VariantID(14400, 0, 0x1A0220) , 0x78 };
			stl::write_thunk_call<GetDescriptionHookSE>(target2.address());
		}
	};

	//HUGE Credits to Nightfallstorm this hook!!
	//https://github.com/Nightfallstorm/DescriptionFramework released under GPL-3.0
	struct ItemCardPopulateHook //BOOK CNAM
	{
		static void thunk(RE::ItemCard* itemCard, RE::TESBoundObject** a_item, char a3)
		{
			if (!itemCard || !a_item || !*a_item)
			{
				return func(itemCard, a_item, a3);
			}

			func(itemCard, a_item, a3);
			handleDescriptionItems(itemCard, *a_item);
		};

		static void handleDescriptionItems(RE::ItemCard* itemCard, RE::TESBoundObject* a_item)
		{
			std::string newDescription{};

			if (Manager::GetSingleton()->getCNAM(a_item->formID, newDescription))
			{
				auto descriptionValue = RE::GFxValue(newDescription);
				itemCard->obj.SetMember("description", descriptionValue);

				SKSE::log::debug("Replaced BOOK CNAM {0:08X} with:", a_item->formID);
				SKSE::log::debug("{}", newDescription);
			}

		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			std::array targets{
				std::make_pair(RELOCATION_ID(50005, 50949), 0x80),
				std::make_pair(RELOCATION_ID(50201, 51130), 0xB2),
				std::make_pair(RELOCATION_ID(50297, 51218), 0x35),
				std::make_pair(RELOCATION_ID(50674, 51569), REL::Relocate(0x80, 0x7A)),
				std::make_pair(RELOCATION_ID(50973, 51852), REL::Relocate(0x80, 0x7A)),
			};

			for (auto& [id, offset] : targets)
			{
				REL::Relocation<std::uintptr_t> target(id, offset);
				stl::write_thunk_call<ItemCardPopulateHook>(target.address());
			}

			if (REL::Module::IsAE())
			{
				REL::Relocation<std::uintptr_t> target{ REL::ID(51458), 0x87 };
				stl::write_thunk_call<ItemCardPopulateHook>(target.address());
			}
		}
	};


	void InstallDescriptionHooks()
	{
		ItemCardPopulateHook::Install();

		if (REL::Module::IsAE())
		{
			DescriptionHookAE::Install();
			GetDescriptionHookAE::Install();
		}
		else
		{
			GetDescriptionHookSE::Install();
		}
	}

}