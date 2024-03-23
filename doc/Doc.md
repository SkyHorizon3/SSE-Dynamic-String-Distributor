
I strongly recommend using [xTranslator](https://www.nexusmods.com/starfield/mods/313) and [SSEEdit](https://www.nexusmods.com/skyrimspecialedition/mods/164) to get all the information below.\
If you're not trying to do an english to english replacement I also recommend using [SSE Auto Translator](https://www.nexusmods.com/skyrimspecialedition/mods/111491) to automate the entire process.

The replacement string needs to follow the json format. The best way to do this is to write the text as it should look and convert it with [freeformatter](https://www.freeformatter.com/json-escape.html#before-output).

## Replace XXXX FULL, NPC_ SHRT, WOOP TNAM, GMST DATA

Use this scheme for any full name, NPC_ SHRT, WOOP TNAM, GMST DATA replacements except REFR FULL and DIAL FULL.

```json
[
    {
        "editor_id": "EditorID",
        "type": "Record type",
        "string": "replacement/translation string"
    }
]
```

Example:
```json
[
    {
        "editor_id": "WhiterunDrunkenHuntsman",
        "type": "CELL FULL",
        "string": "New full name"
    },
    {
        "editor_id": "EnchDwarvenSwordFire02",
        "type": "WEAP FULL",
        "string": "New full name"
    }
]
```


## Replace REFR FULL, DIAL FULL, INFO RNAM

Use this scheme for REFR FULL, DIAL FULL, INFO RNAM replacements.

```json
[
    {
        "form_id": "[FormID]",
        "type": "Record type",
        "string": "replacement/translation string"
    }
]
```

Example:
```json
[
    {
        "form_id": "[0004D0F4]",
        "type": "REFR FULL",
        "string": "New full name"
    },
    {
        "form_id": "[0003FA35]",
        "type": "DIAL FULL",
        "string": "New sentence"
    }
]
```


## Replace XXXX DESC, BOOK CNAM, MGEF DNAM
Use this scheme for description (DESC) or BOOK CNAM, MGEF DNAM replacements.

```json
[
    {
        "editor_id": "EditorID",
        "type": "Record type",
        "string": "replacement/translation string"
    }
]
```

Example:
```json
[
    {
        "editor_id": "DLC2TGFormula",
        "type": "BOOK DESC",
        "string": "<font face='$HandwrittenFont'>\r\nDelvin,\r\nhier ist die Rezeptur für die Verbesserte Knochenrüstung, die ich in meinem letzten Brief erwähnt habe. Behaltet das Gold, das Ihr durch den Verkauf einnehmt, und stellt sicher, dass es unserer kleinen Schwester an nichts mangelt.\r\n\r\nZutaten:\r\n- 4 Tassen Knochenstaub\r\n- 1 Tasse Salz\r\n- 1 Flasche Netchgelee\r\n- 1 Tasse gemahlenen Mammutstoßzahn\r\n- 1 Eimer frisches Wasser\r\n\r\nGebt den Knochenstaub und das Salz ins Wasser und lasst sie dort mindestens einen Tag lang einweichen. Dann gebt das Netchgelee und den Mammutstoßzahn hinzu. Rührt alles über einer heißen Flamme um, bis die Mischung sich langsam erhärtet. Gießt sie anschließend in die gewünschte Form und platziert die Form in der Mitte Eurer Schmiede. Erhitzt die Mischung mindestens einen halben Tag lang, dann lasst sie auskühlen und entfernt sie aus der Form.\r\n\r\n- Glover Mallory"
    },
    {
        "editor_id": "AbResistDisease",
        "type": "MGEF DNAM",
        "string": "<mag>-prozentige Krankheitsresistenz."
    }
]
```

## Replace ACTI RNAM, FLOR RNAM, REGN RDMP, PERK EPFD, QUST CNAM
Use this scheme for ACTI RNAM, FLOR RNAM, REGN RDMP, PERK EPFD, QUST CNAM replacements.


```json
[
    {
        "type": "Record type",
        "original": "original string",
        "string": "replacement/translation string"
    }
]
```

Example:
```json
[
    {
        "type": "ACTI RNAM",
        "original": "Mine",
        "string": "new string"
    }
]
```

## Replace QUST NNAM, INFO NAM1
Use this scheme for QUST NNAM, INFO NAM1 replacements.


```json
[
    {
        "form_id": "[FormID]",
        "type": "Record type",
        "index": Number,
        "string": "replacement/translation string"
    }
]
```

For INFO NAM1 the index is the reponse number of the response.\
For QUST NNAM the index is the objective index (QOBJ) of the objective.

Example:
```json
[
    {
        "form_id": "[000E6C43]",
        "type": "INFO NAM1",
        "index": 2,
        "string": "new sentence"
    },
    {
        "form_id": "[0004B2A1]",
        "type": "QUST NNAM",
        "index": 60,
        "string": "new objective sentence"
    }
]
```

## Replace MESG ITXT, PERK EPF2
Use this scheme for MESG ITXT, PERK EPF2 replacements.


```json
[
    {
        "editor_id": "EditorID",
        "type": "Record type",
        "index": Number,
        "string": "replacement/translation string"
    }
]
```

For MESG ITXT the index is the position in the order in which you can see them in SSEEdit.\
For PERK EPF2 the index is the Fragment Index in EPF3.

Example:
```json
[
    {
        "editor_id": "doomLadyMSG",
        "type": "MESG ITXT",
        "index": 0,
        "string": "Replaces Accept Sign"
    }
]
```



