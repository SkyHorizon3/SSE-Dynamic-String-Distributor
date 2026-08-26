# Dynamic String Distributor (DSD)

[![GitHub Release](https://img.shields.io/github/v/release/areqpl/SSE-Dynamic-String-Distributor?style=flat-square&color=blue&label=Release)](https://github.com/areqpl/SSE-Dynamic-String-Distributor/releases)
[![Build Status](https://img.shields.io/github/actions/workflow/status/areqpl/SSE-Dynamic-String-Distributor/build.yml?branch=main&style=flat-square&label=Build)](https://github.com/areqpl/SSE-Dynamic-String-Distributor/actions)
[![License: GPL-3.0](https://img.shields.io/badge/License-GPL--3.0-green?style=flat-square)](https://github.com/areqpl/SSE-Dynamic-String-Distributor/blob/main/LICENSE)
[![Skyrim Compatibility](https://img.shields.io/badge/Skyrim-AE%201.7.99.0%20%7C%201.6.1170%20%7C%20SE%201.5.97%20%7C%20VR-purple?style=flat-square)](https://www.nexusmods.com/skyrimspecialedition/mods/107676)
[![SKSE64](https://img.shields.io/badge/SKSE64-v2.3.0%2B-orange?style=flat-square)](https://skse.silverlock.org/)
[![Nexus Mods](https://img.shields.io/badge/NexusMods-107676-yellow?style=flat-square&logo=nexusmods)](https://www.nexusmods.com/skyrimspecialedition/mods/107676)

**Dynamic String Distributor (DSD)** is a high-performance **SKSE64 plugin** for **Skyrim Special Edition (SE)**, **Anniversary Edition (AE)**, and **Skyrim VR**. It dynamically replaces, modifies, and distributes in-game text strings (names, descriptions, dialogue, quest stages, game settings) directly in memory at runtime without requiring hard `.esp` or `.esm` plugin overrides.

---

## 🌟 Key Features

- **Zero Plugin Conflicts:** Distribute and replace localized or customized text in memory without creating `.esp` masters or load order conflicts.
- **Universal Skyrim Compatibility:** Single unified binary compiled for **Skyrim 1.7.99.0**, **1.6.1170**, **1.6.1130**, **1.6.640**, **1.5.97 (SE)**, and **Skyrim VR**.
- **Full Address Library Integration:** Powered by [Address Library for SKSE Plugins](https://www.nexusmods.com/skyrimspecialedition/mods/32444) for version-independent hooking.
- **Robust Exception Handling:** Wrapped hook thunks and JSON loaders prevent crashes from corrupt files or runtime edge cases.
- **Extensive Form Type Support:** Modify Game Settings (`GMST`), Messages (`MESG`), Quests (`QUST`), Dialogues (`DIAL`/`INFO`), Weapons (`WEAP`), Armors (`ARMO`), Spells (`SPEL`), Books (`BOOK`), Actors (`NPC_`), and more.
- **xEdit Integration:** Includes automated Pascal script for extracting strings from existing plugins directly into DSD JSON configurations.

---

## 📋 Requirements & Compatibility

| Component | Required Version | Notes |
| :--- | :--- | :--- |
| **Skyrim Runtime** | **1.7.99.0** (August 2026), `1.6.x` (AE), `1.5.97` (SE), `VR` | Single universal DLL handles all runtimes |
| **SKSE64** | **2.3.0+** (matching your Skyrim runtime) | [silverlock.org](https://skse.silverlock.org/) |
| **Address Library** | Latest All-in-One (`versionlib-1-7-99-0.bin`) | [Nexus Mods #32444](https://www.nexusmods.com/skyrimspecialedition/mods/32444) |
| **MergeMapper** | *(Optional)* Recommended for merged plugin setups | [Nexus Mods #74689](https://www.nexusmods.com/skyrimspecialedition/mods/74689) |

---

## 📥 Installation

### Mod Manager (Recommended)
1. Download **Dynamic String Distributor** via Mod Organizer 2 (MO2) or Vortex.
2. Ensure **SKSE64** and **Address Library for SKSE Plugins** are installed.
3. Enable the mod in your mod manager.

### Manual Installation
1. Download the latest `DynamicStringDistributor-v1.5.3.zip` from [Releases](https://github.com/areqpl/SSE-Dynamic-String-Distributor/releases).
2. Extract the archive into your Skyrim game directory:
   ```
   Data/
   └── SKSE/
       └── Plugins/
           ├── DynamicStringDistributor.dll
           ├── DynamicStringDistributor.pdb
           └── DynamicStringDistributor.ini
   ```

---

## 🛠️ Configuration & JSON Syntax

Place your custom `.json` configuration files inside `Data/SKSE/Plugins/DynamicStringDistributor/`.

### Example Configuration (`Example_DSD.json`)

```json
[
  {
    "form_id": "0x012EB7|Skyrim.esm",
    "type": "WEAP FULL",
    "string": "Iron Sword of the Dragonborn"
  },
  {
    "form_id": "0x012EB7|Skyrim.esm",
    "type": "WEAP DESC",
    "string": "Forged in dragon fire. Deals additional damage to undead."
  },
  {
    "form_id": "0x000205|Update.esm",
    "type": "GMST DATA",
    "editor_id": "sLevelUp",
    "string": "Level Up Achieved!"
  }
]
```

> **Note:** JSON strings must be properly escaped according to [RFC 8259](https://datatracker.ietf.org/doc/html/rfc8259). Use [jsonformatter.org](https://jsonformatter.org/json-escape) for multiline strings.

---

## 🔬 Verified Address Library Relocations (Skyrim 1.7.99.0)

All 12 runtime relocation hooks have been verified against the official `offsets-1-7-99-0.txt` database:

| Hook / Feature | SE ID (`1.5.97`) | AE ID (`1.7.99.0`) | 1.7.99.0 Offset | Target Form / Behavior | Status |
| :--- | :---: | :---: | :---: | :--- | :---: |
| `GetLogEntryHook` | `24778` | **`25259`** | `+0x003E1CD0` | Quest Log CNAM text | ✅ Verified |
| `GetDescription` | `14401` | **`14552`** | `+0x001E1380` | Record DESC & CNAM descriptions | ✅ Verified |
| `GetResponseListHook` | `25083` | **`25626`** | `+0x003F3300` | Dialogue Response INFO NAM1 | ✅ Verified |
| `DialogueMenuTextHook` | `34434` | **`35254`** | `+0x005ED5D0` | Dialogue Menu DIAL FULL & INFO RNAM | ✅ Verified |
| `DataHandler::CompileFiles` | `13645` | **`13745`** | `+0x001BE500` | Plugin compilation & initialization | ✅ Verified |
| `DataHandler::ReloadPlugins` | `13672` | **`13783`** | `+0x001C3680` | Runtime plugin reload listener | ✅ Verified |
| `CopyComponentFromTESNPC` | `24216` | **`24726`** | `+0x003C2E00` | NPC Actor full name component | ✅ Verified |
| `CopyComponent` | `24160` | **`24670`** | `+0x003BE960` | Base component string copier | ✅ Verified |
| `ReconstructForms` | `34644` | **`35566`** | `+0x00615430` | Constructed forms post-initialization | ✅ Verified |
| `Setting::SetStringValue` | `73882` | **`75619`** | `+0x00FC2130` | GMST string value mutation | ✅ Verified |
| `ActivateTextOverrideMap` | `501445` | **`360165`** | `+0x0207AED8` | Activation text prompt override | ✅ Verified |
| `setBSFixedString` | `15291` | **`15453`** | `+0x0020D150` | BSFixedString memory setter | ✅ Verified |

---

## 📝 Changelog

### v1.5.3
- **CI/CD Workflow Hardening:** Added explicit `permissions: contents: write` and injected `GITHUB_TOKEN` to `action-gh-release` for automated GitHub Release uploads.

### v1.5.2
- **Documentation & Manifest Link Repairs:** Replaced retired FreeFormatter URL with `jsonformatter.org` and updated `commonlibsse-ng` `$schema` and `homepage` endpoints.
- **Portfile Hardening:** Verified all 27 dependencies with cryptographic SHA512 hashes.

### v1.5.1
- **Cross-Platform Toolchain Improvements:** Enhanced CMake with `/FIPCH.h` forced header inclusion, reordered dependency discovery, and added robust overlay ports (`clib-util`, `srell`, `directxtk`).
- **Filesystem Parsing Fix:** Corrected scoping in `Manager::processFolders()` directory sorting.
- **Robust Exception Handling:** Wrapped hook thunks, lifecycle hooks, and filesystem operations with defensive try-catch handlers.

### v1.5.0
- **Skyrim 1.7.99.0 & SKSE 2.3.0 Support:** Full validation against official `offsets-1-7-99-0.txt` database (12/12 Address Library relocation IDs verified).
- **Runtime Safety Guards:** Added `REL::Module::IsSE()` checks to `DataHandlerInitAllForms` and `NPCFullNameCopyComponent` to prevent null/zero address trampoline crashes on AE / 1.7.99.0.
- **Relocation Mapping:** Mapped `CompileFiles` to AE ID `13745` (RVA `+0x001BE500`).
- **Hook Offset Verification:** Verified hook targets for `DialogueMenuTextHook` (ID `35254` @ `+0x115`, `+0x226` within 976-byte span) and `ReconstructForms` (ID `35566` @ `+0x25F` within 1,120-byte span).
- **Automated CI/CD Workflow:** Added GitHub Actions build matrix (`.github/workflows/build.yml`) supporting universal MSVC compilation across SE, AE, and VR with automatic artifact bundling.

---

## 🔨 Building from Source

```bash
# Clone the repository with submodules
git clone --recursive https://github.com/areqpl/SSE-Dynamic-String-Distributor.git
cd SSE-Dynamic-String-Distributor

# Configure build with unified CMake preset
cmake --preset ALL -B build

# Compile Release DLL & PDB
cmake --build build --config Release
```

---

## 📜 Credits & Acknowledgments

- **Original Author:** [SkyHorizon3](https://github.com/SkyHorizon3/SSE-Dynamic-String-Distributor)
- **Fork Maintainer:** [areqpl](https://github.com/areqpl)
- **Libraries & Dependencies:**
  - [CommonLibSSE-NG](https://github.com/alandtse/CommonLibSSE-NG) by alandtse & powerof3
  - [CLibUtil](https://github.com/powerof3/CLibUtil) by powerof3
  - [MergeMapper](https://github.com/alandtse/MergeMapper) by alandtse
  - [Glaze](https://github.com/stephenberry/glaze) by stephenberry
  - [SRELL](https://github.com/data-man/SRELL) by data-man
  - [DirectXTK](https://github.com/Microsoft/DirectXTK) by Microsoft


