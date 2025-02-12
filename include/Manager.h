#pragma once

enum class SubrecordType : std::uint32_t
{
	kFULL,
	kSHRT,
	kTNAM,
	kDATA,
	kRDMP,
	kDESC,
	kCNAM,
	kDNAM,
	kNNAM,
	kITXT,
	kEPFD,

	kUnknown
};

template <class T>
struct custom_nullable_t
{
	std::optional<T> val{};
	bool has_value() const { return val.has_value(); }

	T& value() { return *val; }
	const T& value() const { return *val; }

	template <class... Args>
	void emplace(Args&&... args) {
		val.emplace(std::forward<Args>(args)...);
	}
};

struct ParseData
{
	std::string form_id{};
	std::string type{};
	std::string string{};
	std::string original{};
	std::string conditions{};
	custom_nullable_t<int> index{};
	custom_nullable_t<std::string> editor_id{};
};

struct StringData
{
	StringData(const std::string& original, const std::string& conditions)
		: replacerText(original), conditionString(conditions), pos(0), subRecordType(SubrecordType::kUnknown) {
	}

	std::string form_id{};
	std::string replacerText{};
	SubrecordType subRecordType;
	std::optional<std::string> originalString;
	std::string conditionString{};
	int pos;


	//std::shared_ptr<RE::TESCondition> conditions;
};


class Manager : public ISingleton<Manager>
{

public:
	//void buildConditions();

	SubrecordType getSubrecordType_map(std::string_view type);

	template <typename Map, typename Key, typename replace>
	bool getReplacerText(const Map& map, const Key& key, replace& string);

	void addToConst(RE::TESForm* form, const ParseData& data);
	void checkConst();

	void addDIAL(const RE::FormID& formID, const std::string& plugin, const ParseData& data);
	bool getDIAL(const RE::FormID& formID, const std::string& plugin, RE::BSString& description);

	void addINFO_NAM1(const RE::FormID& formID, const std::string& plugin, const int& responseNumber, const ParseData& data);
	bool getINFO_NAM1(const RE::FormID& formID, const std::string& plugin, const std::uint8_t& responseNumber, RE::BSString& string);
	StringMap<std::vector<StringData>>& getINFO_NAM1_Map() { return m_INFO_NAM1_Map; }

	static std::string constructKey(const RE::FormID& formID, const std::string& plugin);

	void addDESC(RE::TESForm* form, const ParseData& data);
	bool getDESC(const RE::FormID& formID, std::string& description);

	void addCNAM(RE::TESForm* form, const ParseData& data);
	bool getCNAM(const RE::FormID& formID, std::string& description);

	void addREFR(RE::TESForm* form, const ParseData& data);
	bool getREFR(const RE::FormID& formID, std::string& description);

	void addQUST(const std::string& original, const ParseData& data);
	bool getQUST(const std::string& original, RE::BSString& description);

	void addACTI(const RE::FormID& formID, const std::string& plugin, const ParseData& data);
	bool getACTI(const RE::FormID& formID, const std::string& plugin, std::string& description);

	void runConstTranslation(RE::TESForm* form, const StringData& data);
private:

	template <typename T>
	void SetConstStrings(RE::TESForm* form, const RE::BSFixedString& newString, RE::BSFixedString T::* memberPtr);

	void SetFullnameStrings(RE::TESForm* form, const std::string& newString);

	void SetGameSettingString(const std::string& name, const std::string& newString);

	void SetMessageBoxButtonStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index);

	void SetRegionDataStrings(RE::TESForm* form, const RE::BSFixedString& newString);

	void SetEntryPointStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index);

	void SetQuestObjectiveStrings(RE::TESForm* form, const RE::BSFixedString& newString, const int& index);

	void Report(const RE::TESForm* form);

	static RE::Setting* changeSettingString(RE::Setting* setting, const char* str)
	{
		using func_t = decltype(&changeSettingString);
		static REL::Relocation<func_t> func{ RELOCATION_ID(73882, 75619) };
		return func(setting, str);
	}

	FlatMap<RE::FormID, StringData> m_DESC;
	FlatMap<RE::FormID, StringData> m_CNAM;
	FlatMap<RE::FormID, StringData> m_REFR;

	StringMap<std::vector<StringData>> m_INFO_NAM1_Map;
	StringMap<StringData> m_DIAL_RNAM_Map;
	StringMap<StringData> m_QUST_CNAM_Map;
	StringMap<StringData> m_ACTI_Map;

	//FlatMap<RE::FormID, StringData> m_constTranslation;
	std::vector<StringData> m_constTranslationGMST;
};