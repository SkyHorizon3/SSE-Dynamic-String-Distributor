#include "Globals.h"
#include "Utils.h"

namespace Utils
{
	void RunConsoleCommand(const std::string& command)
	{
		const auto factory = RE::IFormFactory::GetConcreteFormFactoryByType<RE::Script>();
		if (const auto script = factory ? factory->Create() : nullptr)
		{
			const auto ref = RE::Console::GetSelectedRef();
			script->SetCommand(command);
			script->CompileAndRun(ref.get());
			delete script;
		}
	}
}