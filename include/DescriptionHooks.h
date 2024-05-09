#pragma once

namespace Hook
{
	std::string GetItemDescription(RE::FormID& formID, Config::SubrecordType subrecord);
	void InstallDescriptionHooks();
}