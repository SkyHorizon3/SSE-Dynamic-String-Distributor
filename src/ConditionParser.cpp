#include "ConditionParser.h"

std::int32_t ConditionParser::GetFuncID(std::string_view a_str)
{
	switch (string::const_hash(a_str))
	{
	case "GetWantBlocking"_h: return 0;
	case "GetDistance"_h: return 1;
	case "GetLocked"_h: return 5;
	case "GetPos"_h: return 6;
	case "GetAngle"_h: return 8;
	case "GetStartingPos"_h: return 10;
	case "GetStartingAngle"_h: return 11;
	case "GetSecondsPassed"_h: return 12;
	case "GetActorValue"_h: return 14;
	case "GetCurrentTime"_h: return 18;
	case "GetScale"_h: return 24;
	case "IsMoving"_h: return 25;
	case "IsTurning"_h: return 26;
	case "GetLineOfSight"_h: return 27;
	case "GetInSameCell"_h: return 32;
	case "GetDisabled"_h: return 35;
	case "MenuMode"_h: return 36;
	case "GetDisease"_h: return 39;
	case "GetClothingValue"_h: return 41;
	case "SameFaction"_h: return 42;
	case "SameRace"_h: return 43;
	case "SameSex"_h: return 44;
	case "GetDetected"_h: return 45;
	case "GetDead"_h: return 46;
	case "GetItemCount"_h: return 47;
	case "GetGold"_h: return 48;
	case "GetSleeping"_h: return 49;
	case "GetTalkedToPC"_h: return 50;
	case "GetScriptVariable"_h: return 53;
	case "GetQuestRunning"_h: return 56;
	case "GetStage"_h: return 58;
	case "GetStageDone"_h: return 59;
	case "GetFactionRankDifference"_h: return 60;
	case "GetAlarmed"_h: return 61;
	case "IsRaining"_h: return 62;
	case "GetAttacked"_h: return 63;
	case "GetIsCreature"_h: return 64;
	case "GetLockLevel"_h: return 65;
	case "GetShouldAttack"_h: return 66;
	case "GetInCell"_h: return 67;
	case "GetIsClass"_h: return 68;
	case "GetIsRace"_h: return 69;
	case "GetIsSex"_h: return 70;
	case "GetInFaction"_h: return 71;
	case "GetIsID"_h: return 72;
	case "GetFactionRank"_h: return 73;
	case "GetGlobalValue"_h: return 74;
	case "IsSnowing"_h: return 75;
	case "GetRandomPercent"_h: return 77;
	case "GetQuestVariable"_h: return 79;
	case "GetLevel"_h: return 80;
	case "IsRotating"_h: return 81;
	case "GetDeadCount"_h: return 84;
	case "GetIsAlerted"_h: return 91;
	case "GetPlayerControlsDisabled"_h: return 98;
	case "GetHeadingAngle"_h: return 99;
	case "IsWeaponMagicOut"_h: return 101;
	case "IsTorchOut"_h: return 102;
	case "IsShieldOut"_h: return 103;
	case "IsFacingUp"_h: return 106;
	case "GetKnockedState"_h: return 107;
	case "GetWeaponAnimType"_h: return 108;
	case "IsWeaponSkillType"_h: return 109;
	case "GetCurrentAIPackage"_h: return 110;
	case "IsWaiting"_h: return 111;
	case "IsIdlePlaying"_h: return 112;
	case "IsIntimidatedbyPlayer"_h: return 116;
	case "IsPlayerInRegion"_h: return 117;
	case "GetActorAggroRadiusViolated"_h: return 118;
	case "GetCrime"_h: return 122;
	case "IsGreetingPlayer"_h: return 123;
	case "IsGuard"_h: return 125;
	case "HasBeenEaten"_h: return 127;
	case "GetStaminaPercentage"_h: return 128;
	case "GetPCIsClass"_h: return 129;
	case "GetPCIsRace"_h: return 130;
	case "GetPCIsSex"_h: return 131;
	case "GetPCInFaction"_h: return 132;
	case "SameFactionAsPC"_h: return 133;
	case "SameRaceAsPC"_h: return 134;
	case "SameSexAsPC"_h: return 135;
	case "GetIsReference"_h: return 136;
	case "IsTalking"_h: return 141;
	case "GetWalkSpeed"_h: return 142;
	case "GetCurrentAIProcedure"_h: return 143;
	case "GetTrespassWarningLevel"_h: return 144;
	case "IsTrespassing"_h: return 145;
	case "IsInMyOwnedCell"_h: return 146;
	case "GetWindSpeed"_h: return 147;
	case "GetCurrentWeatherPercent"_h: return 148;
	case "GetIsCurrentWeather"_h: return 149;
	case "IsContinuingPackagePCNear"_h: return 150;
	case "GetIsCrimeFaction"_h: return 152;
	case "CanHaveFlames"_h: return 153;
	case "HasFlames"_h: return 154;
	case "GetOpenState"_h: return 157;
	case "GetSitting"_h: return 159;
	case "GetIsCurrentPackage"_h: return 161;
	case "IsCurrentFurnitureRef"_h: return 162;
	case "IsCurrentFurnitureObj"_h: return 163;
	case "GetDayOfWeek"_h: return 170;
	case "GetTalkedToPCParam"_h: return 172;
	case "IsPCSleeping"_h: return 175;
	case "IsPCAMurderer"_h: return 176;
	case "HasSameEditorLocAsRef"_h: return 180;
	case "HasSameEditorLocAsRefAlias"_h: return 181;
	case "GetEquipped"_h: return 182;
	case "IsSwimming"_h: return 185;
	case "GetAmountSoldStolen"_h: return 190;
	case "GetIgnoreCrime"_h: return 192;
	case "GetPCExpelled"_h: return 193;
	case "GetPCFactionMurder"_h: return 195;
	case "GetPCEnemyofFaction"_h: return 197;
	case "GetPCFactionAttack"_h: return 199;
	case "GetDestroyed"_h: return 203;
	case "HasMagicEffect"_h: return 214;
	case "GetDefaultOpen"_h: return 215;
	case "GetAnimAction"_h: return 219;
	case "IsSpellTarget"_h: return 223;
	case "GetVATSMode"_h: return 224;
	case "GetPersuasionNumber"_h: return 225;
	case "GetVampireFeed"_h: return 226;
	case "GetCannibal"_h: return 227;
	case "GetIsClassDefault"_h: return 228;
	case "GetClassDefaultMatch"_h: return 229;
	case "GetInCellParam"_h: return 230;
	case "GetVatsTargetHeight"_h: return 235;
	case "GetIsGhost"_h: return 237;
	case "GetUnconscious"_h: return 242;
	case "GetRestrained"_h: return 244;
	case "GetIsUsedItem"_h: return 246;
	case "GetIsUsedItemType"_h: return 247;
	case "IsScenePlaying"_h: return 248;
	case "IsInDialogueWithPlayer"_h: return 249;
	case "GetLocationCleared"_h: return 250;
	case "GetIsPlayableRace"_h: return 254;
	case "GetOffersServicesNow"_h: return 255;
	case "HasAssociationType"_h: return 258;
	case "HasFamilyRelationship"_h: return 259;
	case "HasParentRelationship"_h: return 261;
	case "IsWarningAbout"_h: return 262;
	case "IsWeaponOut"_h: return 263;
	case "HasSpell"_h: return 264;
	case "IsTimePassing"_h: return 265;
	case "IsPleasant"_h: return 266;
	case "IsCloudy"_h: return 267;
	case "IsSmallBump"_h: return 274;
	case "GetBaseActorValue"_h: return 277;
	case "IsOwner"_h: return 278;
	case "IsCellOwner"_h: return 280;
	case "IsHorseStolen"_h: return 282;
	case "IsLeftUp"_h: return 285;
	case "IsSneaking"_h: return 286;
	case "IsRunning"_h: return 287;
	case "GetFriendHit"_h: return 288;
	case "IsInCombat"_h: return 289;
	case "IsInInterior"_h: return 300;
	case "IsWaterObject"_h: return 304;
	case "GetPlayerAction"_h: return 305;
	case "IsActorUsingATorch"_h: return 306;
	case "IsXBox"_h: return 309;
	case "GetInWorldspace"_h: return 310;
	case "GetPCMiscStat"_h: return 312;
	case "GetPairedAnimation"_h: return 313;
	case "IsActorAVictim"_h: return 314;
	case "GetTotalPersuasionNumber"_h: return 315;
	case "GetIdleDoneOnce"_h: return 318;
	case "GetNoRumors"_h: return 320;
	case "GetCombatState"_h: return 323;
	case "GetWithinPackageLocation"_h: return 325;
	case "IsRidingMount"_h: return 327;
	case "IsFleeing"_h: return 329;
	case "IsInDangerousWater"_h: return 332;
	case "GetIgnoreFriendlyHits"_h: return 338;
	case "IsPlayersLastRiddenMount"_h: return 339;
	case "IsActor"_h: return 353;
	case "IsEssential"_h: return 354;
	case "IsPlayerMovingIntoNewSpace"_h: return 358;
	case "GetInCurrentLoc"_h: return 359;
	case "GetInCurrentLocAlias"_h: return 360;
	case "GetTimeDead"_h: return 361;
	case "HasLinkedRef"_h: return 362;
	case "IsChild"_h: return 365;
	case "GetStolenItemValueNoCrime"_h: return 366;
	case "GetLastPlayerAction"_h: return 367;
	case "IsPlayerActionActive"_h: return 368;
	case "IsTalkingActivatorActor"_h: return 370;
	case "IsInList"_h: return 372;
	case "GetStolenItemValue"_h: return 373;
	case "GetCrimeGoldViolent"_h: return 375;
	case "GetCrimeGoldNonviolent"_h: return 376;
	case "HasShout"_h: return 378;
	case "GetHasNote"_h: return 381;
	case "GetHitLocation"_h: return 390;
	case "IsPC1stPerson"_h: return 391;
	case "GetCauseofDeath"_h: return 396;
	case "IsLimbGone"_h: return 397;
	case "IsWeaponInList"_h: return 398;
	case "IsBribedbyPlayer"_h: return 402;
	case "GetRelationshipRank"_h: return 403;
	case "GetVATSValue"_h: return 407;
	case "IsKiller"_h: return 408;
	case "IsKillerObject"_h: return 409;
	case "GetFactionCombatReaction"_h: return 410;
	case "Exists"_h: return 414;
	case "GetGroupMemberCount"_h: return 415;
	case "GetGroupTargetCount"_h: return 416;
	case "GetIsVoiceType"_h: return 426;
	case "GetPlantedExplosive"_h: return 427;
	case "IsScenePackageRunning"_h: return 429;
	case "GetHealthPercentage"_h: return 430;
	case "GetIsObjectType"_h: return 432;
	case "GetDialogueEmotion"_h: return 434;
	case "GetDialogueEmotionValue"_h: return 435;
	case "GetIsCreatureType"_h: return 437;
	case "GetInCurrentLocFormList"_h: return 444;
	case "GetInZone"_h: return 445;
	case "GetVelocity"_h: return 446;
	case "GetGraphVariableFloat"_h: return 447;
	case "HasPerk"_h: return 448;
	case "GetFactionRelation"_h: return 449;
	case "IsLastIdlePlayed"_h: return 450;
	case "GetPlayerTeammate"_h: return 453;
	case "GetPlayerTeammateCount"_h: return 454;
	case "GetActorCrimePlayerEnemy"_h: return 458;
	case "GetCrimeGold"_h: return 459;
	case "IsPlayerGrabbedRef"_h: return 463;
	case "GetKeywordItemCount"_h: return 465;
	case "GetDestructionStage"_h: return 470;
	case "GetIsAlignment"_h: return 473;
	case "IsProtected"_h: return 476;
	case "GetThreatRatio"_h: return 477;
	case "GetIsUsedItemEquipType"_h: return 479;
	case "IsCarryable"_h: return 487;
	case "GetConcussed"_h: return 488;
	case "GetMapMarkerVisible"_h: return 491;
	case "PlayerKnows"_h: return 493;
	case "GetPermanentActorValue"_h: return 494;
	case "GetKillingBlowLimb"_h: return 495;
	case "CanPayCrimeGold"_h: return 497;
	case "GetDaysInJail"_h: return 499;
	case "EPAlchemyGetMakingPoison"_h: return 500;
	case "EPAlchemyEffectHasKeyword"_h: return 501;
	case "GetAllowWorldInteractions"_h: return 503;
	case "GetLastHitCritical"_h: return 508;
	case "IsCombatTarget"_h: return 513;
	case "GetVATSRightAreaFree"_h: return 515;
	case "GetVATSLeftAreaFree"_h: return 516;
	case "GetVATSBackAreaFree"_h: return 517;
	case "GetVATSFrontAreaFree"_h: return 518;
	case "GetLockIsBroken"_h: return 519;
	case "IsPS3"_h: return 520;
	case "IsWin32"_h: return 521;
	case "GetVATSRightTargetVisible"_h: return 522;
	case "GetVATSLeftTargetVisible"_h: return 523;
	case "GetVATSBackTargetVisible"_h: return 524;
	case "GetVATSFrontTargetVisible"_h: return 525;
	case "IsInCriticalStage"_h: return 528;
	case "GetXPForNextLevel"_h: return 530;
	case "GetInfamy"_h: return 533;
	case "GetInfamyViolent"_h: return 534;
	case "GetInfamyNonViolent"_h: return 535;
	case "GetQuestCompleted"_h: return 543;
	case "IsGoreDisabled"_h: return 547;
	case "IsSceneActionComplete"_h: return 550;
	case "GetSpellUsageNum"_h: return 552;
	case "GetActorsInHigh"_h: return 554;
	case "HasLoaded3D"_h: return 555;
	case "HasKeyword"_h: return 560;
	case "HasRefType"_h: return 561;
	case "LocationHasKeyword"_h: return 562;
	case "LocationHasRefType"_h: return 563;
	case "GetIsEditorLocation"_h: return 565;
	case "GetIsAliasRef"_h: return 566;
	case "GetIsEditorLocAlias"_h: return 567;
	case "IsSprinting"_h: return 568;
	case "IsBlocking"_h: return 569;
	case "HasEquippedSpell"_h: return 570;
	case "GetCurrentCastingType"_h: return 571;
	case "GetCurrentDeliveryType"_h: return 572;
	case "GetAttackState"_h: return 574;
	case "GetEventData"_h: return 576;
	case "IsCloserToAThanB"_h: return 577;
	case "GetEquippedShout"_h: return 579;
	case "IsBleedingOut"_h: return 580;
	case "GetRelativeAngle"_h: return 584;
	case "GetMovementDirection"_h: return 589;
	case "IsInScene"_h: return 590;
	case "GetRefTypeDeadCount"_h: return 591;
	case "GetRefTypeAliveCount"_h: return 592;
	case "GetIsFlying"_h: return 594;
	case "IsCurrentSpell"_h: return 595;
	case "SpellHasKeyword"_h: return 596;
	case "GetEquippedItemType"_h: return 597;
	case "GetLocationAliasCleared"_h: return 598;
	case "GetLocAliasRefTypeDeadCount"_h: return 600;
	case "GetLocAliasRefTypeAliveCount"_h: return 601;
	case "IsWardState"_h: return 602;
	case "IsInSameCurrentLocAsRef"_h: return 603;
	case "IsInSameCurrentLocAsRefAlias"_h: return 604;
	case "LocAliasIsLocation"_h: return 605;
	case "GetKeywordDataForLocation"_h: return 606;
	case "GetKeywordDataForAlias"_h: return 608;
	case "LocAliasHasKeyword"_h: return 610;
	case "IsNullPackageData"_h: return 611;
	case "GetNumericPackageData"_h: return 612;
	case "IsFurnitureAnimType"_h: return 613;
	case "IsFurnitureEntryType"_h: return 614;
	case "GetHighestRelationshipRank"_h: return 615;
	case "GetLowestRelationshipRank"_h: return 616;
	case "HasAssociationTypeAny"_h: return 617;
	case "HasFamilyRelationshipAny"_h: return 618;
	case "GetPathingTargetOffset"_h: return 619;
	case "GetPathingTargetAngleOffset"_h: return 620;
	case "GetPathingTargetSpeed"_h: return 621;
	case "GetPathingTargetSpeedAngle"_h: return 622;
	case "GetMovementSpeed"_h: return 623;
	case "GetInContainer"_h: return 624;
	case "IsLocationLoaded"_h: return 625;
	case "IsLocAliasLoaded"_h: return 626;
	case "IsDualCasting"_h: return 627;
	case "GetVMQuestVariable"_h: return 629;
	case "GetVMScriptVariable"_h: return 630;
	case "IsEnteringInteractionQuick"_h: return 631;
	case "IsCasting"_h: return 632;
	case "GetFlyingState"_h: return 633;
	case "IsInFavorState"_h: return 635;
	case "HasTwoHandedWeaponEquipped"_h: return 636;
	case "IsExitingInstant"_h: return 637;
	case "IsInFriendStateWithPlayer"_h: return 638;
	case "GetWithinDistance"_h: return 639;
	case "GetActorValuePercent"_h: return 640;
	case "IsUnique"_h: return 641;
	case "GetLastBumpDirection"_h: return 642;
	case "IsInFurnitureState"_h: return 644;
	case "GetIsInjured"_h: return 645;
	case "GetIsCrashLandRequest"_h: return 646;
	case "GetIsHastyLandRequest"_h: return 647;
	case "IsLinkedTo"_h: return 650;
	case "GetKeywordDataForCurrentLocation"_h: return 651;
	case "GetInSharedCrimeFaction"_h: return 652;
	case "GetBribeSuccess"_h: return 654;
	case "GetIntimidateSuccess"_h: return 655;
	case "GetArrestedState"_h: return 656;
	case "GetArrestingActor"_h: return 657;
	case "EPTemperingItemIsEnchanted"_h: return 659;
	case "EPTemperingItemHasKeyword"_h: return 660;
	case "GetReplacedItemType"_h: return 664;
	case "IsAttacking"_h: return 672;
	case "IsPowerAttacking"_h: return 673;
	case "IsLastHostileActor"_h: return 674;
	case  "GetGraphVariableInt"_h: return 675;
	case "GetCurrentShoutVariation"_h: return 676;
	case "ShouldAttackKill"_h: return 678;
	case "GetActivatorHeight"_h: return 680;
	case "EPMagic_IsAdvanceSkill"_h: return 681;
	case "WornHasKeyword"_h: return 682;
	case "GetPathingCurrentSpeed"_h: return 683;
	case "GetPathingCurrentSpeedAngle"_h: return 684;
	case "EPModSkillUsage_AdvanceObjectHasKeyword"_h: return 691;
	case "EPModSkillUsage_IsAdvanceAction"_h: return 692;
	case "EPMagic_SpellHasKeyword"_h: return 693;
	case "GetNoBleedoutRecovery"_h: return 694;
	case "EPMagic_SpellHasSkill"_h: return 696;
	case "IsAttackType"_h: return 697;
	case "IsAllowedToFly"_h: return 698;
	case "HasMagicEffectKeyword"_h: return 699;
	case "IsCommandedActor"_h: return 700;
	case "IsStaggered"_h: return 701;
	case "IsRecoiling"_h: return 702;
	case "IsExitingInteractionQuick"_h: return 703;
	case "IsPathing"_h: return 704;
	case "GetShouldHelp"_h: return 705;
	case "HasBoundWeaponEquipped"_h: return 706;
	case "GetCombatTargetHasKeyword"_h: return 707;
	case "GetCombatGroupMemberCount"_h: return 709;
	case "IsIgnoringCombat"_h: return 710;
	case "GetLightLevel"_h: return 711;
	case "SpellHasCastingPerk"_h: return 713;
	case "IsBeingRidden"_h: return 714;
	case "IsUndead"_h: return 715;
	case "GetRealHoursPassed"_h: return 716;
	case "IsUnlockedDoor"_h: return 718;
	case "IsHostileToActor"_h: return 719;
	case "GetTargetHeight"_h: return 720;
	case "IsPoison"_h: return 721;
	case "WornApparelHasKeywordCount"_h: return 722;
	case "GetItemHealthPercent"_h: return 723;
	case "EffectWasDualCast"_h: return 724;
	case "GetKnockedStateEnum"_h: return 725;
	case "DoesNotExist"_h: return 726;
	case "IsOnFlyingMount"_h: return 730;
	case "CanFlyHere"_h: return 731;
	case "IsFlyingMountPatrolQueud"_h: return 732;
	case "IsFlyingMountFastTravelling"_h: return 733;
	case "IsOverEncumbered"_h: return 734;
	case "GetActorWarmth"_h: return 735;
	case "GetSKSEVersion"_h: return 1024;
	case "GetSKSEVersionMinor"_h: return 1025;
	case "GetSKSEVersionBeta"_h: return 1026;
	case "GetSKSERelease"_h: return 1027;
	case "ClearInvalidRegistrations"_h: return 1028;

	default:return -1;
	}
}

PARAMS ConditionParser::GetFuncType(FUNC_ID a_funcID)
{
	PARAMS paramPair;

	switch (a_funcID) {
	case FUNC_ID::kGetWantBlocking:
	case FUNC_ID::kGetLocked:
	case FUNC_ID::kGetSecondsPassed:
	case FUNC_ID::kGetCurrentTime:
	case FUNC_ID::kGetScale:
	case FUNC_ID::kIsMoving:
	case FUNC_ID::kIsTurning:
	case FUNC_ID::kGetDisabled:
	case FUNC_ID::kGetDisease:
	case FUNC_ID::kGetClothingValue:
	case FUNC_ID::kGetDead:
	case FUNC_ID::kGetGold:
	case FUNC_ID::kGetSleeping:
	case FUNC_ID::kGetTalkedToPC:
	case FUNC_ID::kGetAlarmed:
	case FUNC_ID::kIsRaining:
	case FUNC_ID::kGetAttacked:
	case FUNC_ID::kGetIsCreature:
	case FUNC_ID::kGetLockLevel:
	case FUNC_ID::kIsSnowing:
	case FUNC_ID::kGetRandomPercent:
	case FUNC_ID::kGetLevel:
	case FUNC_ID::kIsRotating:
	case FUNC_ID::kGetIsAlerted:
	case FUNC_ID::kIsWeaponMagicOut:
	case FUNC_ID::kIsTorchOut:
	case FUNC_ID::kIsShieldOut:
	case FUNC_ID::kIsFacingUp:
	case FUNC_ID::kGetKnockedState:
	case FUNC_ID::kGetWeaponAnimType:
	case FUNC_ID::kGetCurrentAIPackage:
	case FUNC_ID::kIsWaiting:
	case FUNC_ID::kIsIdlePlaying:
	case FUNC_ID::kIsIntimidatedByPlayer:
	case FUNC_ID::kGetActorAggroRadiusViolated:
	case FUNC_ID::kIsGreetingPlayer:
	case FUNC_ID::kIsGuard:
	case FUNC_ID::kHasBeenEaten:
	case FUNC_ID::kGetStaminaPercentage:
	case FUNC_ID::kSameFactionAsPC:
	case FUNC_ID::kSameRaceAsPC:
	case FUNC_ID::kSameSexAsPC:
	case FUNC_ID::kIsTalking:
	case FUNC_ID::kGetWalkSpeed:
	case FUNC_ID::kGetCurrentAIProcedure:
	case FUNC_ID::kGetTrespassWarningLevel:
	case FUNC_ID::kIsTrespassing:
	case FUNC_ID::kIsInMyOwnedCell:
	case FUNC_ID::kGetWindSpeed:
	case FUNC_ID::kGetCurrentWeatherPercent:
	case FUNC_ID::kIsContinuingPackagePCNear:
	case FUNC_ID::kCanHaveFlames:
	case FUNC_ID::kHasFlames:
	case FUNC_ID::kGetOpenState:
	case FUNC_ID::kGetSitting:
	case FUNC_ID::kGetDayOfWeek:
	case FUNC_ID::kIsPCSleeping:
	case FUNC_ID::kIsPCAMurderer:
	case FUNC_ID::kIsSwimming:
	case FUNC_ID::kGetAmountSoldStolen:
	case FUNC_ID::kGetIgnoreCrime:
	case FUNC_ID::kGetDestroyed:
	case FUNC_ID::kGetDefaultOpen:
	case FUNC_ID::kGetAnimAction:
	case FUNC_ID::kGetVATSMode:
	case FUNC_ID::kGetPersuasionNumber:
	case FUNC_ID::kGetVampireFeed:
	case FUNC_ID::kGetCannibal:
	case FUNC_ID::kGetClassDefaultMatch:
	case FUNC_ID::kGetVatsTargetHeight:
	case FUNC_ID::kGetIsGhost:
	case FUNC_ID::kGetUnconscious:
	case FUNC_ID::kGetRestrained:
	case FUNC_ID::kIsInDialogueWithPlayer:
	case FUNC_ID::kGetIsPlayableRace:
	case FUNC_ID::kGetOffersServicesNow:
	case FUNC_ID::kIsWeaponOut:
	case FUNC_ID::kIsTimePassing:
	case FUNC_ID::kIsPleasant:
	case FUNC_ID::kIsCloudy:
	case FUNC_ID::kIsSmallBump:
	case FUNC_ID::kIsHorseStolen:
	case FUNC_ID::kIsLeftUp:
	case FUNC_ID::kIsSneaking:
	case FUNC_ID::kIsRunning:
	case FUNC_ID::kGetFriendHit:
	case FUNC_ID::kIsInInterior:
	case FUNC_ID::kIsWaterObject:
	case FUNC_ID::kGetPlayerAction:
	case FUNC_ID::kIsActorUsingATorch:
	case FUNC_ID::kIsXBox:
	case FUNC_ID::kGetPairedAnimation:
	case FUNC_ID::kIsActorAVictim:
	case FUNC_ID::kGetTotalPersuasionNumber:
	case FUNC_ID::kGetIdleDoneOnce:
	case FUNC_ID::kGetNoRumors:
	case FUNC_ID::kGetCombatState:
	case FUNC_ID::kIsRidingMount:
	case FUNC_ID::kIsFleeing:
	case FUNC_ID::kIsInDangerousWater:
	case FUNC_ID::kGetIgnoreFriendlyHits:
	case FUNC_ID::kIsPlayersLastRiddenMount:
	case FUNC_ID::kIsActor:
	case FUNC_ID::kIsEssential:
	case FUNC_ID::kIsPlayerMovingIntoNewSpace:
	case FUNC_ID::kGetTimeDead:
	case FUNC_ID::kIsChild:
	case FUNC_ID::kGetLastPlayerAction:
	case FUNC_ID::kGetCrimeGoldViolent:
	case FUNC_ID::kGetCrimeGoldNonviolent:
	case FUNC_ID::kGetHitLocation:
	case FUNC_ID::kIsPC1stPerson:
	case FUNC_ID::kGetCauseofDeath:
	case FUNC_ID::kIsBribedbyPlayer:
	case FUNC_ID::kGetGroupMemberCount:
	case FUNC_ID::kGetGroupTargetCount:
	case FUNC_ID::kGetPlantedExplosive:
	case FUNC_ID::kIsScenePackageRunning:
	case FUNC_ID::kGetHealthPercentage:
	case FUNC_ID::kGetDialogueEmotion:
	case FUNC_ID::kGetDialogueEmotionValue:
	case FUNC_ID::kGetPlayerTeammate:
	case FUNC_ID::kGetPlayerTeammateCount:
	case FUNC_ID::kGetActorCrimePlayerEnemy:
	case FUNC_ID::kGetCrimeGold:
	case FUNC_ID::kGetDestructionStage:
	case FUNC_ID::kIsProtected:
	case FUNC_ID::kIsCarryable:
	case FUNC_ID::kGetConcussed:
	case FUNC_ID::kGetMapMarkerVisible:
	case FUNC_ID::kGetKillingBlowLimb:
	case FUNC_ID::kCanPayCrimeGold:
	case FUNC_ID::kGetDaysInJail:
	case FUNC_ID::kEPAlchemyGetMakingPoison:
	case FUNC_ID::kGetAllowWorldInteractions:
	case FUNC_ID::kGetLastHitCritical:
	case FUNC_ID::kGetIsLockBroken:
	case FUNC_ID::kIsPS3:
	case FUNC_ID::kIsWin32:
	case FUNC_ID::kGetXPForNextLevel:
	case FUNC_ID::kGetInfamy:
	case FUNC_ID::kGetInfamyViolent:
	case FUNC_ID::kGetInfamyNonViolent:
	case FUNC_ID::kIsGoreDisabled:
	case FUNC_ID::kGetActorsInHigh:
	case FUNC_ID::kHasLoaded3D:
	case FUNC_ID::kIsSprinting:
	case FUNC_ID::kIsBlocking:
	case FUNC_ID::kGetAttackState:
	case FUNC_ID::kIsBleedingOut:
	case FUNC_ID::kGetMovementDirection:
	case FUNC_ID::kIsInScene:
	case FUNC_ID::kGetIsFlying:
	case FUNC_ID::kGetHighestRelationshipRank:
	case FUNC_ID::kGetLowestRelationshipRank:
	case FUNC_ID::kHasFamilyRelationshipAny:
	case FUNC_ID::kGetPathingTargetSpeed:
	case FUNC_ID::kGetMovementSpeed:
	case FUNC_ID::kIsDualCasting:
	case FUNC_ID::kIsEnteringInteractionQuick:
	case FUNC_ID::kIsCasting:
	case FUNC_ID::kGetFlyingState:
	case FUNC_ID::kIsInFavorState:
	case FUNC_ID::kHasTwoHandedWeaponEquipped:
	case FUNC_ID::kIsExitingInstant:
	case FUNC_ID::kIsInFriendStateWithPlayer:
	case FUNC_ID::kIsUnique:
	case FUNC_ID::kGetLastBumpDirection:
	case FUNC_ID::kGetIsInjured:
	case FUNC_ID::kGetIsCrashLandRequest:
	case FUNC_ID::kGetIsHastyLandRequest:
	case FUNC_ID::kGetBribeSuccess:
	case FUNC_ID::kGetIntimidateSuccess:
	case FUNC_ID::kGetArrestedState:
	case FUNC_ID::kGetArrestingActor:
	case FUNC_ID::kEPTemperingItemIsEnchanted:
	case FUNC_ID::kIsAttacking:
	case FUNC_ID::kIsPowerAttacking:
	case FUNC_ID::kIsLastHostileActor:
	case FUNC_ID::kGetCurrentShoutVariation:
	case FUNC_ID::kGetActivationHeight:
	case FUNC_ID::kGetPathingCurrentSpeed:
	case FUNC_ID::kGetNoBleedoutRecovery:
	case FUNC_ID::kIsAllowedToFly:
	case FUNC_ID::kIsCommandedActor:
	case FUNC_ID::kIsStaggered:
	case FUNC_ID::kIsRecoiling:
	case FUNC_ID::kIsExitingInteractionQuick:
	case FUNC_ID::kIsPathing:
	case FUNC_ID::kGetCombatGroupMemberCount:
	case FUNC_ID::kIsIgnoringCombat:
	case FUNC_ID::kGetLightLevel:
	case FUNC_ID::kIsBeingRidden:
	case FUNC_ID::kIsUndead:
	case FUNC_ID::kGetRealHoursPassed:
	case FUNC_ID::kIsUnlockedDoor:
	case FUNC_ID::kIsPoison:
	case FUNC_ID::kGetItemHealthPercent:
	case FUNC_ID::kEffectWasDualCast:
	case FUNC_ID::kGetKnockStateEnum:
	case FUNC_ID::kDoesNotExist:
	case FUNC_ID::kIsOnFlyingMount:
	case FUNC_ID::kCanFlyHere:
	case FUNC_ID::kIsFlyingMountPatrolQueued:
	case FUNC_ID::kIsFlyingMountFastTravelling:
	case FUNC_ID::kIsOverEncumbered:
	case FUNC_ID::kGetActorWarmth:
		paramPair = { std::nullopt, std::nullopt };
		break;
	case FUNC_ID::kGetDistance:
	case FUNC_ID::kGetLineOfSight:
	case FUNC_ID::kGetInSameCell:
	case FUNC_ID::kGetHeadingAngle:
	case FUNC_ID::kGetIsReference:
	case FUNC_ID::kIsCurrentFurnitureRef:
	case FUNC_ID::kGetRelationshipRank:
	case FUNC_ID::kExists:
	case FUNC_ID::kIsPlayerGrabbedRef:
	case FUNC_ID::kGetVATSRightAreaFree:
	case FUNC_ID::kGetVATSLeftAreaFree:
	case FUNC_ID::kGetVATSBackAreaFree:
	case FUNC_ID::kGetVATSFrontAreaFree:
	case FUNC_ID::kGetVATSRightTargetVisible:
	case FUNC_ID::kGetVATSLeftTargetVisible:
	case FUNC_ID::kGetVATSBackTargetVisible:
	case FUNC_ID::kGetVATSFrontTargetVisible:
	case FUNC_ID::kGetInContainer:
	case FUNC_ID::kGetInSharedCrimeFaction:
	case FUNC_ID::kGetTargetHeight:
		paramPair = { PARAM_TYPE::kObjectRef, std::nullopt };
		break;
	case FUNC_ID::kGetPos:
	case FUNC_ID::kGetAngle:
	case FUNC_ID::kGetStartingPos:
	case FUNC_ID::kGetStartingAngle:
	case FUNC_ID::kGetVelocity:
	case FUNC_ID::kGetPathingTargetOffset:
	case FUNC_ID::kGetPathingTargetAngleOffset:
	case FUNC_ID::kGetPathingTargetSpeedAngle:
	case FUNC_ID::kGetPathingCurrentSpeedAngle:
		paramPair = { PARAM_TYPE::kAxis, std::nullopt };
		break;
	case FUNC_ID::kGetActorValue:
	case FUNC_ID::kIsWeaponSkillType:
	case FUNC_ID::kGetBaseActorValue:
	case FUNC_ID::kGetPermanentActorValue:
	case FUNC_ID::kGetActorValuePercent:
	case FUNC_ID::kEPModSkillUsage_IsAdvanceSkill:
	case FUNC_ID::kEPMagic_SpellHasSkill:
		paramPair = { PARAM_TYPE::kActorValue, std::nullopt };
		break;
	case FUNC_ID::kMenuMode:
	case FUNC_ID::kIsInCombat:
	case FUNC_ID::kIsPlayerActionActive:
	case FUNC_ID::kGetHasNote:
	case FUNC_ID::kIsLimbGone:
	case FUNC_ID::kGetIsCreatureType:
	case FUNC_ID::kGetNumericPackageData:
		paramPair = { PARAM_TYPE::kInt, std::nullopt };
		break;
	case FUNC_ID::kSameFaction:
	case FUNC_ID::kSameRace:
	case FUNC_ID::kSameSex:
	case FUNC_ID::kGetDetected:
	case FUNC_ID::kGetShouldAttack:
	case FUNC_ID::kGetTalkedToPCParam:
	case FUNC_ID::kHasFamilyRelationship:
	case FUNC_ID::kHasParentRelationship:
	case FUNC_ID::kIsTalkingActivatorActor:
	case FUNC_ID::kIsKiller:
	case FUNC_ID::kGetFactionRelation:
	case FUNC_ID::kGetThreatRatio:
	case FUNC_ID::kIsCombatTarget:
	case FUNC_ID::kShouldAttackKill:
	case FUNC_ID::kGetShouldHelp:
	case FUNC_ID::kIsHostileToActor:
		paramPair = { PARAM_TYPE::kActor, std::nullopt };
		break;
	case FUNC_ID::kGetItemCount:
	case FUNC_ID::kGetEquipped:
		paramPair = { PARAM_TYPE::kInvObjectOrFormList, std::nullopt };
		break;
	case FUNC_ID::kGetScriptVariable:
	case FUNC_ID::kGetVMScriptVariable:
		paramPair = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kChar };
		break;
	case FUNC_ID::kGetQuestRunning:
	case FUNC_ID::kGetStage:
	case FUNC_ID::kGetQuestCompleted:
		paramPair = { PARAM_TYPE::kQuest, std::nullopt };
		break;
	case FUNC_ID::kGetStageDone:
		paramPair = { PARAM_TYPE::kQuest, PARAM_TYPE::kInt };
		break;
	case FUNC_ID::kGetFactionRankDifference:
		paramPair = { PARAM_TYPE::kFaction, PARAM_TYPE::kActor };
		break;
	case FUNC_ID::kGetInCell:
		paramPair = { PARAM_TYPE::kCell, std::nullopt };
		break;
	case FUNC_ID::kGetIsClass:
	case FUNC_ID::kGetPCIsClass:
	case FUNC_ID::kGetIsClassDefault:
		paramPair = { PARAM_TYPE::kClass, std::nullopt };
		break;
	case FUNC_ID::kGetIsRace:
	case FUNC_ID::kGetPCIsRace:
		paramPair = { PARAM_TYPE::kRace, std::nullopt };
		break;
	case FUNC_ID::kGetIsSex:
	case FUNC_ID::kGetPCIsSex:
		paramPair = { PARAM_TYPE::kSex, std::nullopt };
		break;
	case FUNC_ID::kGetInFaction:
	case FUNC_ID::kGetFactionRank:
	case FUNC_ID::kGetPCInFaction:
	case FUNC_ID::kGetIsCrimeFaction:
	case FUNC_ID::kGetPCExpelled:
	case FUNC_ID::kGetPCFactionMurder:
	case FUNC_ID::kGetPCEnemyofFaction:
	case FUNC_ID::kGetPCFactionAttack:
	case FUNC_ID::kGetStolenItemValueNoCrime:
	case FUNC_ID::kGetStolenItemValue:
		paramPair = { PARAM_TYPE::kFaction, std::nullopt };
		break;
	case FUNC_ID::kGetIsID:
	case FUNC_ID::kGetIsUsedItem:
		paramPair = { PARAM_TYPE::kObjectOrFormList, std::nullopt };
		break;
	case FUNC_ID::kGetGlobalValue:
		paramPair = { PARAM_TYPE::kGlobal, std::nullopt };
		break;
	case FUNC_ID::kGetQuestVariable:
	case FUNC_ID::kGetVMQuestVariable:
		paramPair = { PARAM_TYPE::kQuest, PARAM_TYPE::kChar };
		break;
	case FUNC_ID::kGetDeadCount:
		paramPair = { PARAM_TYPE::kActorBase, std::nullopt };
		break;
	case FUNC_ID::kGetPlayerControlsDisabled:
		paramPair = { PARAM_TYPE::kInt, PARAM_TYPE::kInt };
		break;
	case FUNC_ID::kIsPlayerInRegion:
		paramPair = { PARAM_TYPE::kRegion, std::nullopt };
		break;
	case FUNC_ID::kGetCrime:
		paramPair = { PARAM_TYPE::kActor, PARAM_TYPE::kCrimeType };
		break;
	case FUNC_ID::kGetIsCurrentWeather:
		paramPair = { PARAM_TYPE::kWeather, std::nullopt };
		break;
	case FUNC_ID::kGetIsCurrentPackage:
		paramPair = { PARAM_TYPE::kPackage, std::nullopt };
		break;
	case FUNC_ID::kIsCurrentFurnitureObj:
		paramPair = { PARAM_TYPE::kFurnitureOrFormList, std::nullopt };
		break;
	case FUNC_ID::kHasSameEditorLocAsRef:
	case FUNC_ID::kIsInSameCurrentLocAsRef:
	case FUNC_ID::kIsLinkedTo:
		paramPair = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kKeyword };
		break;
	case FUNC_ID::kHasSameEditorLocAsRefAlias:
	case FUNC_ID::kIsInSameCurrentLocAsRefAlias:
	case FUNC_ID::kGetKeywordDataForAlias:
	case FUNC_ID::kLocAliasHasKeyword:
		paramPair = { PARAM_TYPE::kAlias, PARAM_TYPE::kKeyword };
		break;
	case FUNC_ID::kHasMagicEffect:
		paramPair = { PARAM_TYPE::kMagicEffect, std::nullopt };
		break;
	case FUNC_ID::kIsSpellTarget:
	case FUNC_ID::kHasSpell:
	case FUNC_ID::kGetSpellUsageNum:
		paramPair = { PARAM_TYPE::kMagicItem, std::nullopt };
		break;
	case FUNC_ID::kGetInCellParam:
		paramPair = { PARAM_TYPE::kCell, PARAM_TYPE::kObjectRef };
		break;
	case FUNC_ID::kGetIsUsedItemType:
	case FUNC_ID::kGetIsObjectType:
		paramPair = { PARAM_TYPE::kFormType, std::nullopt };
		break;
	case FUNC_ID::kIsScenePlaying:
		paramPair = { PARAM_TYPE::kBGSScene, std::nullopt };
		break;
	case FUNC_ID::kGetLocationCleared:
	case FUNC_ID::kGetInCurrentLoc:
	case FUNC_ID::kGetIsEditorLocation:
	case FUNC_ID::kIsLocationLoaded:
		paramPair = { PARAM_TYPE::kLocation, std::nullopt };
		break;
	case FUNC_ID::kHasAssociationType:
		paramPair = { PARAM_TYPE::kActor, PARAM_TYPE::kAssociationType };
		break;
	case FUNC_ID::kIsWarningAbout:
	case FUNC_ID::kIsInList:
	case FUNC_ID::kIsWeaponInList:
	case FUNC_ID::kIsKillerObject:
	case FUNC_ID::kGetInCurrentLocFormList:
		paramPair = { PARAM_TYPE::kFormList, std::nullopt };
		break;
	case FUNC_ID::kIsOwner:
		paramPair = { PARAM_TYPE::kOwner, std::nullopt };
		break;
	case FUNC_ID::kIsCellOwner:
		paramPair = { PARAM_TYPE::kCell, PARAM_TYPE::kOwner };
		break;
	case FUNC_ID::kGetInWorldspace:
		paramPair = { PARAM_TYPE::kWorldOrList, std::nullopt };
		break;
	case FUNC_ID::kGetPCMiscStat:
		paramPair = { PARAM_TYPE::kMiscStat, std::nullopt };
		break;
	case FUNC_ID::kGetWithinPackageLocation:
	case FUNC_ID::kIsNullPackageData:
		paramPair = { PARAM_TYPE::kPackageDataCanBeNull, std::nullopt };
		break;
	case FUNC_ID::kGetInCurrentLocAlias:
	case FUNC_ID::kGetIsAliasRef:
	case FUNC_ID::kGetIsEditorLocAlias:
	case FUNC_ID::kGetLocationAliasCleared:
	case FUNC_ID::kIsLocAliasLoaded:
		paramPair = { PARAM_TYPE::kAlias, std::nullopt };
		break;
	case FUNC_ID::kHasLinkedRef:
	case FUNC_ID::kGetKeywordItemCount:
	case FUNC_ID::kEPAlchemyEffectHasKeyword:
	case FUNC_ID::kHasKeyword:
	case FUNC_ID::kLocationHasKeyword:
	case FUNC_ID::kGetKeywordDataForCurrentLocation:
	case FUNC_ID::kEPTemperingItemHasKeyword:
	case FUNC_ID::kWornHasKeyword:
	case FUNC_ID::kEPModSkillUsage_AdvanceObjectHasKeyword:
	case FUNC_ID::kEPMagic_SpellHasKeyword:
	case FUNC_ID::kIsAttackType:
	case FUNC_ID::kHasMagicEffectKeyword:
	case FUNC_ID::kGetCombatTargetHasKeyword:
	case FUNC_ID::kWornApparelHasKeywordCount:
		paramPair = { PARAM_TYPE::kKeyword, std::nullopt };
		break;
	case FUNC_ID::kHasShout:
	case FUNC_ID::kGetEquippedShout:
		paramPair = { PARAM_TYPE::kShout, std::nullopt };
		break;
	case FUNC_ID::kGetVATSValue:
		paramPair = { PARAM_TYPE::kInt, PARAM_TYPE::kInt };
		break;
	case FUNC_ID::kGetFactionCombatReaction:
		paramPair = { PARAM_TYPE::kFaction, PARAM_TYPE::kFaction };
		break;
	case FUNC_ID::kGetIsVoiceType:
		paramPair = { PARAM_TYPE::kVoiceType, std::nullopt };
		break;
	case FUNC_ID::kGetInZone:
		paramPair = { PARAM_TYPE::kEncounterZone, std::nullopt };
		break;
	case FUNC_ID::kGetGraphVariableFloat:
	case FUNC_ID::kGetGraphVariableInt:
		paramPair = { PARAM_TYPE::kChar, std::nullopt };
		break;
	case FUNC_ID::kHasPerk:
		paramPair = { PARAM_TYPE::kPerk, PARAM_TYPE::kInt };
		break;
	case FUNC_ID::kIsLastIdlePlayed:
		paramPair = { PARAM_TYPE::kIdleForm, std::nullopt };
		break;
	case FUNC_ID::kGetIsAlignment:
		paramPair = { PARAM_TYPE::kAlignment, std::nullopt };
		break;
	case FUNC_ID::kGetIsUsedItemEquipType:
		paramPair = { PARAM_TYPE::kEquipType, std::nullopt };
		break;
	case FUNC_ID::kPlayerKnows:
		paramPair = { PARAM_TYPE::kKnowableForm, std::nullopt };
		break;
	case FUNC_ID::kIsInCriticalStage:
		paramPair = { PARAM_TYPE::kCritStage, std::nullopt };
		break;
	case FUNC_ID::kIsSceneActionComplete:
		paramPair = { PARAM_TYPE::kBGSScene, PARAM_TYPE::kInt };
		break;
	case FUNC_ID::kHasRefType:
	case FUNC_ID::kLocationHasRefType:
		paramPair = { PARAM_TYPE::kRefType, std::nullopt };
		break;
	case FUNC_ID::kHasEquippedSpell:
	case FUNC_ID::kGetCurrentCastingType:
	case FUNC_ID::kGetCurrentDeliveryType:
	case FUNC_ID::kGetEquippedItemType:
	case FUNC_ID::kGetReplacedItemType:
	case FUNC_ID::kHasBoundWeaponEquipped:
		paramPair = { PARAM_TYPE::kCastingSource, std::nullopt };
		break;
	case FUNC_ID::kGetEventData:
		paramPair = { PARAM_TYPE::kEventFunction, PARAM_TYPE::kEventFunctionData };  // third parameter in xEdit but who cares, we're skipping this
		break;
	case FUNC_ID::kIsCloserToAThanB:
		paramPair = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kObjectRef };
		break;
	case FUNC_ID::kGetRelativeAngle:
		paramPair = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kAxis };
		break;
	case FUNC_ID::kGetRefTypeDeadCount:
	case FUNC_ID::kGetRefTypeAliveCount:
		paramPair = { PARAM_TYPE::kLocation, PARAM_TYPE::kRefType };
		break;
	case FUNC_ID::kIsCurrentSpell:
		paramPair = { PARAM_TYPE::kMagicItem, PARAM_TYPE::kCastingSource };
		break;
	case FUNC_ID::kSpellHasKeyword:
		paramPair = { PARAM_TYPE::kCastingSource, PARAM_TYPE::kKeyword };
		break;
	case FUNC_ID::kGetLocAliasRefTypeDeadCount:
	case FUNC_ID::kGetLocAliasRefTypeAliveCount:
		paramPair = { PARAM_TYPE::kAlias, PARAM_TYPE::kRefType };
		break;
	case FUNC_ID::kIsWardState:
		paramPair = { PARAM_TYPE::kWardState, std::nullopt };
		break;
	case FUNC_ID::kLocAliasIsLocation:
		paramPair = { PARAM_TYPE::kAlias, PARAM_TYPE::kLocation };
		break;
	case FUNC_ID::kGetKeywordDataForLocation:
		paramPair = { PARAM_TYPE::kLocation, PARAM_TYPE::kKeyword };
		break;
	case FUNC_ID::kIsFurnitureAnimType:
	case FUNC_ID::kIsInFurnitureState:
		paramPair = { PARAM_TYPE::kFurnitureAnimType, std::nullopt };
		break;
	case FUNC_ID::kIsFurnitureEntryType:
		paramPair = { PARAM_TYPE::kFurnitureEntryType, std::nullopt };
		break;
	case FUNC_ID::kHasAssociationTypeAny:
		paramPair = { PARAM_TYPE::kAssociationType, std::nullopt };
		break;
	case FUNC_ID::kGetWithinDistance:
		paramPair = { PARAM_TYPE::kObjectRef, PARAM_TYPE::kFloat };
		break;
	case FUNC_ID::kEPModSkillUsage_IsAdvanceAction:
		paramPair = { PARAM_TYPE::kSkillAction, std::nullopt };
		break;
	case FUNC_ID::kSpellHasCastingPerk:
		paramPair = { PARAM_TYPE::kPerk, std::nullopt };
		break;
	default:
		paramPair = { std::nullopt, std::nullopt };
		break;
	}

	return paramPair;
}

RE::TESForm* ConditionParser::LookupForm(const std::string& a_str)
{
	auto formOrEditorID = dist::get_record(a_str);
	if (auto formid_pair = std::get_if<dist::formid_pair>(&formOrEditorID)) {
		if (auto& [formID, modName] = *formid_pair; formID) {
			const auto [mergedModName, mergedFormID] = g_mergeMapperInterface->GetNewFormID(modName.value_or("").c_str(), *formID);
			if (mergedFormID != *formID) {
				formID.emplace(mergedFormID);
			}
			if (const std::string mergedModString{ mergedModName }; modName && !mergedModString.empty() && *modName != mergedModString) {
				modName.emplace(mergedModName);
			}
			if (modName) {
				return RE::TESDataHandler::GetSingleton()->LookupForm(*formID, *modName);
			}
			return RE::TESForm::LookupByID(*formID);
		}
	}
	else {
		return RE::TESForm::LookupByEditorID(std::get<std::string>(formOrEditorID));
	}

	return nullptr;
}

bool ConditionParser::ParseVoidParam(const std::string& a_str, VOID_PARAM& a_param, PARAM_TYPE a_type)
{
	switch (a_type) {
	case PARAM_TYPE::kInt:
	case PARAM_TYPE::kStage:
	case PARAM_TYPE::kRelationshipRank:
		a_param.i = string::to_num<std::int32_t>(a_str);
		break;
	case PARAM_TYPE::kFloat:
		a_param.f = string::to_num<float>(a_str);
		break;
	case PARAM_TYPE::kActorValue:
		a_param.i = static_cast<std::int32_t>(RE::ActorValueList::GetSingleton()->LookupActorValueByName(a_str.c_str()));
		break;
	case RE::SCRIPT_PARAM_TYPE::kAxis:
	{
		switch (string::const_hash(a_str)) {
		case "X"_h:
			a_param.i = 0;
			break;
		case "Y"_h:
			a_param.i = 1;
			break;
		case "Z"_h:
			a_param.i = 2;
			break;
		default:
			a_param.i = -1;
			break;
		}
	}
	break;
	case RE::SCRIPT_PARAM_TYPE::kSex:
		switch (string::const_hash(a_str)) {
		case "Male"_h:
			a_param.i = RE::SEX::kMale;
			break;
		case "Female"_h:
			a_param.i = RE::SEX::kFemale;
			break;
		default:
			a_param.i = -1;
			break;
		}
		break;
	case PARAM_TYPE::kFormType:
		a_param.i = static_cast<std::int32_t>(RE::StringToFormType(a_str));
		break;
	case PARAM_TYPE::kCastingSource:
	{
		switch (string::const_hash(a_str)) {
		case "Instant"_h:
			a_param.i = static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kInstant);
			break;
		case "Left"_h:
			a_param.i = static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kLeftHand);
			break;
		case "Right"_h:
			a_param.i = static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kRightHand);
			break;
		case "Voice"_h:
			a_param.i = static_cast<std::int32_t>(RE::MagicSystem::CastingSource::kOther);
			break;
		default:
			a_param.i = -1;
			break;
		}
	}
	break;
	case PARAM_TYPE::kWardState:
	{
		switch (string::const_hash(a_str)) {
		case "Absorb"_h:
			a_param.i = static_cast<std::int32_t>(RE::MagicSystem::WardState::kAbsorb);
			break;
		case "Break"_h:
			a_param.i = static_cast<std::int32_t>(RE::MagicSystem::WardState::kBreak);
			break;
		case "None"_h:
			a_param.i = static_cast<std::int32_t>(RE::MagicSystem::WardState::kNone);
			break;
		default:
			a_param.i = -1;
			break;
		}
	}
	break;
	case PARAM_TYPE::kObjectRef:
	case PARAM_TYPE::kActor:
	case PARAM_TYPE::kMapMarker:
	case PARAM_TYPE::kContainerRef:
	case PARAM_TYPE::kEquipType:
	case PARAM_TYPE::kInventoryObject:
	case PARAM_TYPE::kSpellItem:
	case PARAM_TYPE::kMessage:
	case PARAM_TYPE::kInvObjectOrFormList:
	case PARAM_TYPE::kObjectOrFormList:
	case PARAM_TYPE::kMusic:
	case PARAM_TYPE::kVoiceType:
	case PARAM_TYPE::kIdleForm:
	case PARAM_TYPE::kFormList:
	case PARAM_TYPE::kQuest:
	case PARAM_TYPE::kFaction:
	case PARAM_TYPE::kCell:
	case PARAM_TYPE::kClass:
	case PARAM_TYPE::kRace:
	case PARAM_TYPE::kActorBase:
	case PARAM_TYPE::kGlobal:
	case PARAM_TYPE::kWeather:
	case PARAM_TYPE::kPackage:
	case PARAM_TYPE::kEncounterZone:
	case PARAM_TYPE::kPerk:
	case PARAM_TYPE::kOwner:
	case PARAM_TYPE::kFurnitureOrFormList:
	case PARAM_TYPE::kMagicItem:
	case PARAM_TYPE::kMagicEffect:
	case PARAM_TYPE::kWorldOrList:
	case PARAM_TYPE::kObject:
	case PARAM_TYPE::kRegion:
	case PARAM_TYPE::kShout:
	case PARAM_TYPE::kLocation:
	case PARAM_TYPE::kRefType:
	case PARAM_TYPE::kAssociationType:
	case PARAM_TYPE::kBGSScene:
	case PARAM_TYPE::kForm:
	case PARAM_TYPE::kKnowableForm:
	case PARAM_TYPE::kWordOfPower:
	case PARAM_TYPE::kTopic:
	case PARAM_TYPE::kNPC:
	case PARAM_TYPE::kSound:
	case PARAM_TYPE::kCombatStyle:
	case PARAM_TYPE::kShaderEffect:
	case PARAM_TYPE::kReferenceEffect:
	case PARAM_TYPE::kMenuIcon:
	case PARAM_TYPE::kNote:
	case PARAM_TYPE::kImagespaceMod:
	case PARAM_TYPE::kImagespace:
	{
		if (a_str == "PlayerRef") {
			a_param.ptr = RE::PlayerCharacter::GetSingleton();
		}
		else {
			a_param.ptr = LookupForm(a_str);
		}
	}
	break;
	case PARAM_TYPE::kKeyword:
	{
		if (dist::get_record_type(a_str) == dist::kEditorID) {
			const auto& keywordArray = RE::TESDataHandler::GetSingleton()->GetFormArray<RE::BGSKeyword>();
			const auto  it = std::ranges::find_if(keywordArray, [&](const auto& keyword) { return keyword->formEditorID == a_str.c_str(); });
			if (it != keywordArray.end()) {
				a_param.ptr = *it;
			}
		}
		else {
			a_param.ptr = LookupForm(a_str);
		}
	}
	break;
	default:
		return false;
	}

	return true;
}

void ConditionParser::BuildCondition(std::shared_ptr<RE::TESCondition>& a_condition, const std::vector<std::string>& a_conditionList)
{
	static const srell::regex condRegex{ R"((\w+)?\s*(\w+)\s+([\w.~]+)(?:\s+([\w.~]+))?\s*([=!<>]+)\s*(-?[\d.]+)\s*(AND|OR)?)" };

	for (auto& condition : a_conditionList) {
		srell::cmatch match;
		if (!srell::regex_match(condition.c_str(), match, condRegex)) {
			continue;
		}

		auto& subject = match[1];
		auto& functionID = match[2];
		auto& param1 = match[3];
		auto& param2 = match[4];
		auto& opCode = match[5];
		auto& value = match[6];
		auto& andOR = match[7];

		RE::CONDITION_ITEM_DATA condData{};
		// subject
		if (subject.str() == "Self") {
			condData.object = RE::CONDITIONITEMOBJECT::kSelf;
		}
		else if (subject.str() == "Target") {
			condData.object = RE::CONDITIONITEMOBJECT::kTarget;
		}
		else if (subject.str() == "CombatTarget") {
			condData.object = RE::CONDITIONITEMOBJECT::kCombatTarget;
		}
		else {
			RE::TESForm* refForm{};
			if (subject.str() == "PlayerRef") {
				refForm = RE::PlayerCharacter::GetSingleton();
			}
			else {
				refForm = LookupForm(subject.str());
			}
			if (auto ref = refForm ? refForm->AsReference() : nullptr) {
				condData.runOnRef = ref->CreateRefHandle();
				condData.object = RE::CONDITIONITEMOBJECT::kRef;
			}
		}
		// funcID
		const auto ID = GetFuncID(functionID.str());
		if (ID >= 0) {
			condData.functionData.function = static_cast<FUNC_ID>(ID);
		}
		else {
			continue;
		}
		auto [param1Type, param2Type] = GetFuncType(*condData.functionData.function);
		// param1
		if (param1Type && param1.matched) {
			VOID_PARAM param{};
			if (ParseVoidParam(param1.str(), param, *param1Type)) {
				condData.functionData.params[0] = std::bit_cast<void*>(param);
			}
			else {
				continue;
			}
		}
		// param2
		if (param2Type && param2.matched) {
			VOID_PARAM param{};
			if (ParseVoidParam(param2.str(), param, *param2Type)) {
				condData.functionData.params[1] = std::bit_cast<void*>(param);
			}
			else {
				continue;
			}
		}
		//opcodes
		switch (string::const_hash(opCode.str())) {
		case "=="_h:
			condData.flags.opCode = OP_CODE::kEqualTo;
			break;
		case "!="_h:
			condData.flags.opCode = OP_CODE::kNotEqualTo;
			break;
		case ">"_h:
			condData.flags.opCode = OP_CODE::kGreaterThan;
			break;
		case ">="_h:
			condData.flags.opCode = OP_CODE::kGreaterThanOrEqualTo;
			break;
		case "<"_h:
			condData.flags.opCode = OP_CODE::kLessThan;
			break;
		case "<="_h:
			condData.flags.opCode = OP_CODE::kLessThanOrEqualTo;
			break;
		default:
			continue;
		}
		// value
		condData.comparisonValue.f = string::to_num<float>(value.str());
		// andOr
		condData.flags.isOR = string::iequals(andOR.str(), "OR"sv);

		if (!a_condition) {
			a_condition = std::make_shared<RE::TESCondition>();
		}

		auto newNode = new RE::TESConditionItem;
		newNode->data = condData;
		newNode->next = nullptr;

		auto* current = a_condition->head;
		if (a_condition->head == nullptr) {
			a_condition->head = newNode;
		}
		else {
			while (current->next != nullptr) {
				current = current->next;
			}
			current->next = newNode;
		}
	}
}
