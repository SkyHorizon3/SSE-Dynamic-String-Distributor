#pragma once
#include "../include/Globals.h"

class Processor
{
public:
	static Processor* GetSingleton()
	{
		static Processor menu;
		return &menu;
	}

	void RunConstTranslation();

private:

	//void CollectITXTTexts(const ConstConfigurationInformation& information);
	void SetFULLName(RE::TESForm* Form, RE::BSFixedString newName);
	void SetSHRTName(RE::TESForm* Form, RE::BSFixedString newName);
	void SetTNAMName(RE::TESForm* Form, RE::BSFixedString newName);
	//void SetITXTName(RE::TESForm* Form);

	Processor() = default;
	~Processor() = default;
	Processor(const Processor&) = delete;
	Processor(Processor&&) = delete;
	Processor& operator=(const Processor&) = delete;
	Processor& operator=(Processor&&) = delete;
};