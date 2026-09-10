#include "RE.h"

namespace RE
{
	RE::Setting* setStringValue(RE::Setting* setting, const char* str)
	{
		using func_t = decltype(&setStringValue);
		static REL::Relocation<func_t> func{ RELOCATION_ID(73882, 75619) };
		return func(setting, str);
	}

	void setBSFixedString(RE::BSFixedString& str, const char* data)
	{
		using func_t = decltype(&setBSFixedString);
		static REL::Relocation<func_t> func{ REL::VariantID(15291, 15453, 0x1CB050) };
		func(str, data);
	}

	RE::FormID getFullNameFormIDForRef(const RE::TESObjectREFR& actor)
	{
		using func_t = decltype(&getFullNameFormIDForRef);
		static REL::Relocation<func_t> func{ RELOCATION_ID(23422, 23885) };
		return func(actor);
	}

	RE::OBJECT_TYPE stringToObjectType(std::string_view str)
	{
		switch (string::const_hash(str))
		{
		case "Activator"_h: return RE::OBJECT_TYPE::kActivator;
		case "Armor"_h: return RE::OBJECT_TYPE::kArmor;
		case "Book"_h: return RE::OBJECT_TYPE::kBook;
		case "Container"_h: return RE::OBJECT_TYPE::kContainer;
		case "Door"_h: return RE::OBJECT_TYPE::kDoor;
		case "Ingredient"_h: return RE::OBJECT_TYPE::kIngredient;
		case "Light"_h: return RE::OBJECT_TYPE::kLight;
		case "MiscItem"_h: return RE::OBJECT_TYPE::kMiscItem;
		case "Static"_h: return RE::OBJECT_TYPE::kStatic;
		case "Grass"_h: return RE::OBJECT_TYPE::kGrass;
		case "Tree"_h: return RE::OBJECT_TYPE::kTree;
		case "Weapon"_h: return RE::OBJECT_TYPE::kWeapon;
		case "Actor"_h: return RE::OBJECT_TYPE::kActor;
		case "LeveledCharacter"_h: return RE::OBJECT_TYPE::kLeveledCharacter;
		case "Spell"_h: return RE::OBJECT_TYPE::kSpell;
		case "Enchantment"_h: return RE::OBJECT_TYPE::kEnchantment;
		case "Potion"_h: return RE::OBJECT_TYPE::kPotion;
		case "LeveledItem"_h: return RE::OBJECT_TYPE::kLeveledItem;
		case "Key"_h: return RE::OBJECT_TYPE::kKey;
		case "Ammo"_h: return RE::OBJECT_TYPE::kAmmo;
		case "Flora"_h: return RE::OBJECT_TYPE::kFlora;
		case "Furniture"_h: return RE::OBJECT_TYPE::kFurniture;
		case "Sound Marker"_h: return RE::OBJECT_TYPE::kSoundMarker;
		case "LandTexture"_h: return RE::OBJECT_TYPE::kLandTexture;
		case "CombatStyle"_h: return RE::OBJECT_TYPE::kCombatStyle;
		case "LoadScreen"_h: return RE::OBJECT_TYPE::kLoadScreen;
		case "LeveledSpell"_h: return RE::OBJECT_TYPE::kLeveledSpell;
		case "AnimObject"_h: return RE::OBJECT_TYPE::kAnimObject;
		case "WaterType"_h: return RE::OBJECT_TYPE::kWaterType;
		case "IdleMarker"_h: return RE::OBJECT_TYPE::kIdleMarker;
		case "EffectShader"_h: return RE::OBJECT_TYPE::kEffectShader;
		case "Projectile"_h: return RE::OBJECT_TYPE::kProjectile;
		case "TalkingActivator"_h: return RE::OBJECT_TYPE::kTalkingActivator;
		case "Explosion"_h: return RE::OBJECT_TYPE::kExplosion;
		case "TextureSet"_h: return RE::OBJECT_TYPE::kTextureSet;
		case "Debris"_h: return RE::OBJECT_TYPE::kDebris;
		case "MenuIcon"_h: return RE::OBJECT_TYPE::kMenuIcon;
		case "FormList"_h: return RE::OBJECT_TYPE::kFormList;
		case "Perk"_h: return RE::OBJECT_TYPE::kPerk;
		case "BodyPartData"_h: return RE::OBJECT_TYPE::kBodyPartData;
		case "AddOnNode"_h: return RE::OBJECT_TYPE::kAddOnNode;
		case "MovableStatic"_h: return RE::OBJECT_TYPE::kMovableStatic;
		case  "CameraShot"_h: return RE::OBJECT_TYPE::kCameraShot;
		case "ImpactData"_h: return RE::OBJECT_TYPE::kImpactData;
		case "ImpactDataSet"_h: return RE::OBJECT_TYPE::kImpactDataSet;
		case "Quest"_h: return RE::OBJECT_TYPE::kQuest;
		case "Package"_h: return RE::OBJECT_TYPE::kPackage;
		case "VoiceType"_h: return RE::OBJECT_TYPE::kVoiceType;
		case "Class"_h: return RE::OBJECT_TYPE::kClass;
		case "Race"_h: return RE::OBJECT_TYPE::kRace;
		case "Eyes"_h: return RE::OBJECT_TYPE::kEyes;
		case "HeadPart"_h: return RE::OBJECT_TYPE::kHeadPart;
		case "Faction"_h: return RE::OBJECT_TYPE::kFaction;
		case "Note"_h: return RE::OBJECT_TYPE::kNote;
		case "Weather"_h: return RE::OBJECT_TYPE::kWeather;
		case "Climate"_h: return RE::OBJECT_TYPE::kClimate;
		case "ArmorAddon"_h: return RE::OBJECT_TYPE::kArmorAddon;
		case "Global"_h: return RE::OBJECT_TYPE::kGlobal;
		case "Imagespace"_h: return RE::OBJECT_TYPE::kImagespace;
		case "Imagespace Modifier"_h: return RE::OBJECT_TYPE::kImagespaceModifier;
		case "Encounter Zone"_h: return RE::OBJECT_TYPE::kEncounterZone;
		case "Message"_h: return RE::OBJECT_TYPE::kMessage;
		case "Constructible Object"_h: return RE::OBJECT_TYPE::kConstructibleObject;
		case "Acoustic Space"_h: return RE::OBJECT_TYPE::kAcousticSpace;
		case "Ragdoll"_h: return RE::OBJECT_TYPE::kRagdoll;
		case "Script"_h: return RE::OBJECT_TYPE::kScript;
		case "Magic Effect"_h: return RE::OBJECT_TYPE::kMagicEffect;
		case "Music Type"_h: return RE::OBJECT_TYPE::kMusicType;
		case "Static Collection"_h: return RE::OBJECT_TYPE::kStaticCollection;
		case "Keyword"_h: return RE::OBJECT_TYPE::kKeyword;
		case "Location"_h: return RE::OBJECT_TYPE::kLocation;
		case "Location Ref Type"_h: return RE::OBJECT_TYPE::kLocationRefType;
		case "Footstep"_h: return RE::OBJECT_TYPE::kFootstep;
		case "Footstep Set"_h: return RE::OBJECT_TYPE::kFootstepSet;
		case "Material Type"_h: return RE::OBJECT_TYPE::kMaterialType;
		case "Actor Action"_h: return RE::OBJECT_TYPE::kActorAction;
		case "Music Track"_h: return RE::OBJECT_TYPE::kMusicTrack;
		case "Word of Power"_h: return RE::OBJECT_TYPE::kWordofPower;
		case "Shout"_h: return RE::OBJECT_TYPE::kShout;
		case "Relationship"_h: return RE::OBJECT_TYPE::kRelationship;
		case "Equip Slot"_h: return RE::OBJECT_TYPE::kEquipSlot;
		case "Association Type"_h: return RE::OBJECT_TYPE::kAssociationType;
		case "Outfit"_h: return RE::OBJECT_TYPE::kOutfit;
		case "Art Object"_h: return RE::OBJECT_TYPE::kArtObject;
		case "Material Object"_h: return RE::OBJECT_TYPE::kMaterialObject;
		case "Lighting Template"_h: return RE::OBJECT_TYPE::kLightingTemplate;
		case "Shader Particle Geometry"_h: return RE::OBJECT_TYPE::kShaderParticleGeometry;
		case "Visual Effect"_h: return RE::OBJECT_TYPE::kVisualEffect;
		case "Apparatus"_h: return RE::OBJECT_TYPE::kApparatus;
		case "Movement Type"_h: return RE::OBJECT_TYPE::kMovementType;
		case "Hazard"_h: return RE::OBJECT_TYPE::kHazard;
		case "SM Event Node"_h: return RE::OBJECT_TYPE::kSMEventNode;
		case "Sound Descriptor"_h: return RE::OBJECT_TYPE::kSoundDescriptor;
		case "Dual Cast Data"_h: return RE::OBJECT_TYPE::kDualCastData;
		case "Sound Category"_h: return RE::OBJECT_TYPE::kSoundCategory;
		case "Soul Gem"_h: return RE::OBJECT_TYPE::kSoulGem;
		case "Sound Output Model"_h: return RE::OBJECT_TYPE::kSoundOutputModel;
		case "Collision Layer"_h: return RE::OBJECT_TYPE::kCollisionLayer;
		case "Scroll"_h: return RE::OBJECT_TYPE::kScroll;
		case "ColorForm"_h: return RE::OBJECT_TYPE::kColorForm;
		case "Reverb Parameters"_h: return RE::OBJECT_TYPE::kReverbParameters;
		case "LensFlare"_h: return RE::OBJECT_TYPE::kLensFlare;
		case "VolumetricLighting"_h: return RE::OBJECT_TYPE::kVolumetricLighting;
		default:
		{
			if (const auto formType = RE::StringToFormType(str); formType != RE::FormType::None)
			{
				return RE::FormTypeToObjectType(formType);
			}
			return RE::OBJECT_TYPE::kInvalid;
		}
		}
	}
}