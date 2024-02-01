#pragma once

using json = nlohmann::json;

class Config
{
public:
	static Config* GetSingleton()
	{
		static Config menu;
		return &menu;
	}

	void EnumerateFolder();
	void LoadFiles();

private:
	bool caseInsensitiveStringCompare(const std::string& a, const std::string& b);
	std::vector<std::string> GetLoadOrder();
	void EnumerateFilesInFolders(std::string folders);
	bool isSpecialType(const std::string& type);
	bool isConstType(const std::string& type);
	std::string GetSubrecordType(const std::string& types) const;
	void HandleSpecialType(const std::string& types, const json& entry, const std::string& stringValue);
	void ParseTranslationFiles();

	Config() = default;
	~Config() = default;
	Config(const Config&) = delete;
	Config(Config&&) = delete;
	Config& operator=(const Config&) = delete;
	Config& operator=(Config&&) = delete;

	/*
	enum class RecordTypes
	{
		//Strings
		kACTI_FULL,
		kACTI_RNAM, //Look
		kALCH_FULL,
		kAMMO_FULL,
		kAPPA_FULL,
		kARMO_FULL,
		kAVIF_FULL, //Actor Value information
		kBOOK_FULL,
		kBPTD_BPTN, //BodyPartData, not visible
		kCELL_FULL,
		kCLAS_FULL, //not visible
		kCLFM_FULL, //color, not all of them are visible
		kCONT_FULL,
		kDIAL_FULL,
		kDOOR_FULL,
		kENCH_FULL,
		kEXPL_FULL,
		kEYES_FULL, //Not sure, probably not visible
		kFACT_FNAM, //Female title - Look
		kFACT_FULL, //not visible
		kFACT_MNAM, //Male title - Look
		kFLOR_FULL,
		kFLOR_RNAM,
		kFURN_FULL,
		kGMST_DATA, //Game settings, sometimes visible - Look
		kHAZD_FULL,
		kHDPT_FULL, //head part data, not visible
		kINFO_RNAM,
		kINGR_FULL,
		kKEYM_FULL,
		kLCTN_FULL,
		kLIGH_FULL,
		kLSCR_DESC,
		kMESG_FULL,
		kMESG_ITXT, //button text of messageboxes
		kMGEF_DNAM,
		kMGEF_FULL,
		kMISC_FULL,
		kNPC__FULL,
		kNPC__SHRT,
		kPERK_EPF2, //Look
		kPERK_EPFD,
		kPERK_FULL,
		kPROJ_FULL,
		kQUST_FULL,
		kQUST_NNAM,
		kRACE_FULL,
		kREFR_FULL,
		kSCRL_FULL,
		kSHOU_FULL,
		kSLGM_FULL,
		kSNCT_FULL, //some are visible?
		kSPEL_FULL,
		kTACT_FULL,
		kTREE_FULL,
		kWATR_FULL,
		kWEAP_FULL,
		kWOOP_FULL,
		kWOOP_TNAM,
		kWRLD_FULL,

		//DLStrings
		kAMMO_DESC,
		kARMO_DESC,
		kAVIF_DESC,
		kBOOK_CNAM,
		kBOOK_DESC,
		kCOLL_DESC, //not visible
		kMESG_DESC,
		kPERK_DESC,
		kQUST_CNAM,
		kRACE_DESC,
		kSCRL_DESC,
		kSHOU_DESC,
		kSPEL_DESC,
		kWEAP_DESC,

		//ILStrings
		kINFO_NAM1,
	};
	*/

private:
	std::vector<std::string> m_Folders;
	std::vector<std::string> m_Files;
	std::vector<std::string> m_FilesInPluginFolder;


	std::vector<std::string> m_BaseGamePlugins = {
		"Skyrim.esm",
		"Update.esm",
		"Dawnguard.esm",
		"HearthFires.esm",
		"Dragonborn.esm",
		"SkyrimVR.esm", //For Skyrim VR
		"ccasvsse001-almsivi.esm",
		"ccbgssse001-fish.esm",
		"ccbgssse002-exoticarrows.esl",
		"ccbgssse003-zombies.esl",
		"ccbgssse004-ruinsedge.esl",
		"ccbgssse005-goldbrand.esl",
		"ccbgssse006-stendarshammer.esl",
		"ccbgssse007-chrysamere.esl",
		"ccbgssse010-petdwarvenarmoredmudcrab.esl",
		"ccbgssse011-hrsarmrelvn.esl",
		"ccbgssse012-hrsarmrstl.esl",
		"ccbgssse014-spellpack01.esl",
		"ccbgssse019-staffofsheogorath.esl",
		"ccbgssse020-graycowl.esl",
		"ccbgssse021-lordsmail.esl",
		"ccmtysse001-knightsofthenine.esl",
		"ccqdrsse001-survivalmode.esl",
		"cctwbsse001-puzzledungeon.esm",
		"cceejsse001-hstead.esm",
		"ccqdrsse002-firewood.esl",
		"ccbgssse018-shadowrend.esl",
		"ccbgssse035-petnhound.esl",
		"ccfsvsse001-backpacks.esl",
		"cceejsse002-tower.esl",
		"ccedhsse001-norjewel.esl",
		"ccvsvsse002-pets.esl",
		"ccbgssse037-curios.esl",
		"ccbgssse034-mntuni.esl",
		"ccbgssse045-hasedoki.esl",
		"ccbgssse008-wraithguard.esl",
		"ccbgssse036-petbwolf.esl",
		"ccffbsse001-imperialdragon.esl",
		"ccmtysse002-ve.esl",
		"ccbgssse043-crosselv.esl",
		"ccvsvsse001-winter.esl",
		"cceejsse003-hollow.esl",
		"ccbgssse016-umbra.esm",
		"ccbgssse031-advcyrus.esm",
		"ccbgssse038-bowofshadows.esl",
		"ccbgssse040-advobgobs.esl",
		"ccbgssse050-ba_daedric.esl",
		"ccbgssse052-ba_iron.esl",
		"ccbgssse054-ba_orcish.esl",
		"ccbgssse058-ba_steel.esl",
		"ccbgssse059-ba_dragonplate.esl",
		"ccbgssse061-ba_dwarven.esl",
		"ccpewsse002-armsofchaos.esl",
		"ccbgssse041-netchleather.esl",
		"ccedhsse002-splkntset.esl",
		"ccbgssse064-ba_elven.esl",
		"ccbgssse063-ba_ebony.esl",
		"ccbgssse062-ba_dwarvenmail.esl",
		"ccbgssse060-ba_dragonscale.esl",
		"ccbgssse056-ba_silver.esl",
		"ccbgssse055-ba_orcishscaled.esl",
		"ccbgssse053-ba_leather.esl",
		"ccbgssse051-ba_daedricmail.esl",
		"ccbgssse057-ba_stalhrim.esl",
		"ccbgssse066-staves.esl",
		"ccbgssse067-daedinv.esm",
		"ccbgssse068-bloodfall.esl",
		"ccbgssse069-contest.esl",
		"ccvsvsse003-necroarts.esl",
		"ccvsvsse004-beafarmer.esl",
		"ccBGSSSE025-AdvDSGS.esm",
		"ccffbsse002-crossbowpack.esl",
		"ccbgssse013-dawnfang.esl",
		"ccrmssse001-necrohouse.esl",
		"ccedhsse003-redguard.esl",
		"cceejsse004-hall.esl",
		"cceejsse005-cave.esm",
		"cckrtsse001_altar.esl",
		"cccbhsse001-gaunt.esl",
		"ccafdsse001-dwesanctuary.esm",
		"_ResourcePack.esl"
	};


};
