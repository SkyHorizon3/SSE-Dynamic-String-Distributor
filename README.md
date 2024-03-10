# Features

- This SKSE plugin allows you to replace existing game strings with new ones defined in jsons\
- Esp/esm/esl version independent text replacements\
- Primarily designed for translations generated via [SSE Auto Translator](https://www.nexusmods.com/skyrimspecialedition/mods/111491)﻿.


# JSON-Configuration

- The json-configuration works similar to the esp/esm/esl-plugin list\
- Within SKSE\Plugins\DynamicStringDistributor, create a folder named after the ESP/ESM/ESL you want to create replacements for (e.g.
SKSE\Plugins\DynamicStringDistributor\skyrim.esm)\
- Inside each of those folders named like plugins you can put json files with any name and following [scheme](https://github.com/SkyHorizon3/SSE-Dynamic-String-Distributor/blob/main/doc/Doc.md)﻿﻿\
- JSON files are only loaded from the folder if the corresponding plugin is in your active plugin list\
- Conflicts between JSON definitions within plugin folders are solved by the position of the active plugin in your plugin list.\
- Conflicts inside of the plugin folders are solved alphabetically
  - Use SSE Auto Translator﻿ for automation

- If there is an issue in a json file you will see it in the DynamicStringDistributor.log


# Compatibility

Works on:
- SE - 1.5.97
- AE - 1.6.640, 1.6.1130, 1.6.1170 + GOG
- VR


# FAQ

### Compatible with [Description Framework](https://www.nexusmods.com/skyrimspecialedition/mods/105799)﻿?
- Yes, but [BOOK CNAM﻿](https://en.uesp.net/wiki/Skyrim_Mod:Mod_File_Format/BOOK) will conflict if both plugins try to add CNAM to same book. In this unlikely case the string defined in the Dynamic String Distributor json is shown.

### Compatible with [Subtitles﻿](https://www.nexusmods.com/skyrimspecialedition/mods/113214)﻿?
- Yes.

### Compatible with [SkyPatcher](https://www.nexusmods.com/skyrimspecialedition/mods/106659)﻿?
- Yes, but full name changes will conflict if both plugins try to add it to same item. In this unlikely case the string defined in SkyPatcher INI is shown.

### What's the point?
- This mod is mainly intended to make translations plugin independent. This reduces the effort of updating the translations after every small plugin update.The automation provided by SSE Auto Translator makes the whole process even easier.


# Source

- Suggestions and contributions are welcome


# Credits
Nightfallstorm, SeaSparrow, Fenix, meatdeath and Sylennus for answering my questions\
Nightfallstorm and po3 for some hooks

