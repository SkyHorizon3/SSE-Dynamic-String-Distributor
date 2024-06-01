#include "MiscHooks.h"
#include "QuestHooks.h"
#include "Processor.h"
#include "Utils.h"

namespace Hook
{
	struct QuestCNAMTextHook //QUST CNAM
	{
		static void thunk(RE::BSString& a_out, char* a_buffer, std::uint64_t a3)
		{
			func(a_out, a_buffer, a3);

			const auto it = g_QUST_CNAM_Map.find(a_out.c_str()); //using original language as key since I couldn't find a way to match the strings without the QuestInstanceID

			if (it != g_QUST_CNAM_Map.end())
			{
				a_out = it->second;
			}

		}
		static inline REL::Relocation<decltype(thunk)> func;


		static void Install()
		{
			//JournalMenu quest description text Hook
			REL::Relocation<std::uintptr_t> target1{ RELOCATION_ID(24778, 25259), REL::Relocate(0x21C, 0x221) };//First: 5B ->Don't know; Second: C4 ->Don't know; Third: 221 ->Quest description text
			stl::write_thunk_call<QuestCNAMTextHook>(target1.address());
		}

	};

	void InstallQuestHooks()
	{
		Hook::QuestCNAMTextHook::Install();
	}

}