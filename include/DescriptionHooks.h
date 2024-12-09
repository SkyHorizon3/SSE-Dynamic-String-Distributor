#pragma once

namespace Hook
{
	bool getItemDescription(const RE::FormID& formID, const Config::SubrecordType& subrecord, std::string& description);
	void InstallDescriptionHooks();
}