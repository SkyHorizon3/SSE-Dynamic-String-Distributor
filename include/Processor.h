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

	template <typename T>
	void SetName(RE::TESForm* Form, RE::BSFixedString newName, RE::BSFixedString T::* memberPtr);

	Processor() = default;
	~Processor() = default;
	Processor(const Processor&) = delete;
	Processor(Processor&&) = delete;
	Processor& operator=(const Processor&) = delete;
	Processor& operator=(Processor&&) = delete;
};