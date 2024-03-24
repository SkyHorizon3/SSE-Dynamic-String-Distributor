
I strongly recommend using [xTranslator](https://www.nexusmods.com/starfield/mods/313) and [SSEEdit](https://www.nexusmods.com/skyrimspecialedition/mods/164) to get all the information below.\
If you're not trying to do an english to english replacement I also recommend using [SSE Auto Translator](https://www.nexusmods.com/skyrimspecialedition/mods/111491) to automate the entire process.

The replacement string needs to follow the json format. The best way to do this is to write the text as it should look and convert it with [freeformatter](https://www.freeformatter.com/json-escape.html#before-output).

## Replace XXXX FULL, XXXX DESC, NPC_ SHRT, WOOP TNAM, INFO RNAM, BOOK CNAM, MGEF DNAM

Use this scheme for any full name, any DESC, NPC_ SHRT, WOOP TNAM, INFO RNAM, BOOK CNAM, MGEF DNAM.

```json
[
    {
        "form_id": "FormID|BaseRecordPlugin",
        "type": "Record type",
        "string": "replacement/translation string"
    }
]
```

Example:
```json
[
    {
        "form_id": "000133C6|Skyrim.esm",
        "type": "CELL FULL",
        "string": "New full name"
    },
    {
        "form_id": "000ACC37|Skyrim.esm",
        "type": "WEAP FULL",
        "string": "New full name"
    },
    {
        "form_id": "0004D0F4|Skyrim.esm",
        "type": "REFR FULL",
        "string": "New full name"
    },
    {
        "form_id": "0003FA35|Skyrim.esm",
        "type": "DIAL FULL",
        "string": "New sentence"
    },
    {
        "form_id": "0001AD07|Skyrim.esm",
        "type": "BOOK DESC",
        "string": "<font face='$HandwrittenFont'>\r\nDelvin,\r\nhier ist die Rezeptur für die Verbesserte Knochenrüstung, die ich in meinem letzten Brief erwähnt habe. Behaltet das Gold, das Ihr durch den Verkauf einnehmt, und stellt sicher, dass es unserer kleinen Schwester an nichts mangelt.\r\n\r\nZutaten:\r\n- 4 Tassen Knochenstaub\r\n- 1 Tasse Salz\r\n- 1 Flasche Netchgelee\r\n- 1 Tasse gemahlenen Mammutstoßzahn\r\n- 1 Eimer frisches Wasser\r\n\r\nGebt den Knochenstaub und das Salz ins Wasser und lasst sie dort mindestens einen Tag lang einweichen. Dann gebt das Netchgelee und den Mammutstoßzahn hinzu. Rührt alles über einer heißen Flamme um, bis die Mischung sich langsam erhärtet. Gießt sie anschließend in die gewünschte Form und platziert die Form in der Mitte Eurer Schmiede. Erhitzt die Mischung mindestens einen halben Tag lang, dann lasst sie auskühlen und entfernt sie aus der Form.\r\n\r\n- Glover Mallory"
    },
    {
        "form_id": "000E40D3|Skyrim.esm",
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
        "form_id": "FormID|BaseRecordPlugin",
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
        "form_id": "0010DCB5|Skyrim.esm",
        "type": "ACTI RNAM",
        "original": "Mine",
        "string": "new string"
    }
]
```

## Replace QUST NNAM, INFO NAM1, MESG ITXT, PERK EPF2
Use this scheme for QUST NNAM, INFO NAM1, MESG ITXT, PERK EPF2 replacements.


```json
[
    {
        "form_id": "FormID|BaseRecordPlugin",
        "type": "Record type",
        "index": Number,
        "string": "replacement/translation string"
    }
]
```

For INFO NAM1 the index is the reponse number of the response.\
For QUST NNAM the index is the objective index (QOBJ) of the objective.\
For MESG ITXT the index is the position in the order in which you can see them in SSEEdit.\
For PERK EPF2 the index is the Fragment Index in EPF3.

Example:
```json
[
    {
        "form_id": "000E6C43|Skyrim.esm",
        "type": "INFO NAM1",
        "index": 2,
        "string": "new sentence"
    },
    {
        "form_id": "0004B2A1|Skyrim.esm",
        "type": "QUST NNAM",
        "index": 60,
        "string": "new objective sentence"
    },
    {
        "form_id": "00016837|Skyrim.esm",
        "type": "MESG ITXT",
        "index": 0,
        "string": "Replaces Accept Sign"
    }
]
```

## Replace GMST DATA
Use this scheme for GMST DATA. Due to how gamesettings work the editorID is always required.

```json
[
    {
        "form_id": "FormID|BaseRecordPlugin",
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
        "form_id": "000D48B7|Skyrim.esm",
        "editor_id": "sCurrentLocation",
        "type": "GMST DATA",
        "string": "New string"
    }
]
```


