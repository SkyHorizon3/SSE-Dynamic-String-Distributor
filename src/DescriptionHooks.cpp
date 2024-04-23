#include "MiscHooks.h"
#include "Processor.h"
#include "DescriptionHooks.h"

namespace Hook
{
	// Track if an object is valid between the two AE hooks
	static bool IsDESC = false;
	std::string SetDescription = "";
	struct ParentDESCHookAE
	{
		static void thunk(RE::BSString* a_out, RE::TESForm* a_parent, std::uint64_t a_unk)
		{
			//Perks, Race Descriptions and any other DESC in xEdit except for loading screens

			UNREFERENCED_PARAMETER(a_unk);

			func(a_out, nullptr, 0); // Invoke original

			for (const auto& Information : g_ConfigurationInformationStruct)
			{
				switch (Information.SubrecordType)
				{
				case Config::SubrecordType::kDESC: //So it doesn't set CNAM as DESC if both are in a json
				{
					if (a_parent && a_parent->formID == Information.Form->formID)
					{
						IsDESC = true;
						SetDescription = Information.ReplacerText;
						// Set object valid for GetDescription hook to check

						SKSE::log::debug("Replaced XXXX DESC {0:08X} with:", a_parent->formID);
						SKSE::log::debug("{}", Information.ReplacerText);

					}
				}
				break;

				default: break;
				}

			}
		}

		static inline REL::Relocation<decltype(thunk)> func;

		struct GetParentInsideHook : Xbyak::CodeGenerator
		{
			// This moves the parent form, which is currently in rdi, to rdx so the other hook can use it
			GetParentInsideHook()
			{
				mov(rdx, rdi);
			}
		};


		static void Install()
		{
			if (REL::Module::IsAE())
			{
				//GetDescriptionParent Hook AE
				REL::Relocation<std::uintptr_t> codeSwap{ RELOCATION_ID(0, 14552), REL::VariantOffset(0x0, 0x8B, 0x0) }; //14019C91B xor     r8d, r8d
				REL::safe_fill(codeSwap.address(), REL::NOP, 0x5);
				auto newCode = ParentDESCHookAE::GetParentInsideHook();
				assert(newCode.getSize() <= 0x5);
				REL::safe_write(codeSwap.address(), newCode.getCode(), newCode.getSize());

				REL::Relocation<std::uintptr_t> target01{ RELOCATION_ID(0, 14552), REL::VariantOffset(0x0, 0x93, 0x0) };
				stl::write_thunk_call<ParentDESCHookAE>(target01.address());
				SKSE::log::info("ParentDESCHookAE hooked at address: {:x} and offset: {:x}", target01.address(), target01.offset());
			}
		}

	};

	struct GetDescriptionHookAE //Nearly any DESC
	{
		static void thunk(RE::BSString* a_out, std::uint64_t a2, std::uint64_t a3)
		{
			func(a_out, a2, a3);

			//g_Logger->info("Output: {}", a_out->c_str());


			if (IsDESC && !a_out->empty()) //Don't replace anything thats empty. This stops the hook from also replacing BOOK CNAM with BOOK DESC if there is no CNAM.
			{
				*a_out = SetDescription;
			}
			IsDESC = false;

		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			if (REL::Module::IsAE())
			{
				REL::Relocation<std::uintptr_t> target01{ RELOCATION_ID(0, 14552), REL::VariantOffset(0x0, 0xFB, 0x0) };
				stl::write_thunk_call<GetDescriptionHookAE>(target01.address());
				SKSE::log::info("GetDescriptionHookAE hooked at address: {:x} and offset: {:x}", target01.address(), target01.offset());

				REL::Relocation<std::uintptr_t> target02{ RELOCATION_ID(0, 14552), REL::VariantOffset(0x0, 0x174, 0x0) };
				stl::write_thunk_call<GetDescriptionHookAE>(target02.address());
				SKSE::log::info("GetDescriptionHookAE hooked at address: {:x} and offset: {:x}", target02.address(), target02.offset());
			}
		}

	};

	struct GetDescriptionHookSE
	{
		static void thunk(RE::TESDescription* a_description, RE::BSString* a_out, RE::TESForm* a_parent, std::uint32_t unk)
		{
			func(a_description, a_out, a_parent, unk);  // invoke original to get original description string output

			for (const auto& Information : g_ConfigurationInformationStruct)
			{
				switch (Information.SubrecordType)
				{
				case Config::SubrecordType::kDESC: //So it doesn't set CNAM as DESC if both are in a json
				{
					if (a_parent && a_parent->formID == Information.Form->formID && !a_out->empty())
					{
						*a_out = Information.ReplacerText;

						SKSE::log::debug("Replaced XXXX DESC {0:08X} with:", a_parent->formID);
						SKSE::log::debug("{}", Information.ReplacerText);
					}
					}
				break;

				default: break;
				}
				}
			}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			if (REL::Module::IsSE() || REL::Module::IsVR())
			{
				REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(14399, 0), REL::VariantOffset(0x53, 0x0, 0x53) };
				stl::write_thunk_call<GetDescriptionHookSE>(target1.address());
				SKSE::log::info("GetDescriptionHookSE hooked at address: {:x} and offset: {:x}", target1.address(), target1.offset());
			}
		}
		};

	void InstallDescriptionHooks()
	{
		Hook::ParentDESCHookAE::Install();
		Hook::GetDescriptionHookAE::Install();
		Hook::GetDescriptionHookSE::Install();
	}

	}