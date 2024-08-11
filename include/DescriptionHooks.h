#pragma once

namespace Hook
{
	std::string GetItemDescription(const RE::FormID& formID, const Config::SubrecordType& subrecord);
	void InstallDescriptionHooks();
}