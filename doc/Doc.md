Scheme:

```json
[
    {
        "editor_id": "EditorID",
        "type": "Record type",
        "original": "original string",
        "string": "replacement/translation string"
    },
    {
        "editor_id": "EditorID",
        "type": "Record type",
        "original": "original string",
        "string": "replacement/translation string"
    }
]
```

I highly recommend using [xTranslator](https://www.nexusmods.com/starfield/mods/313) to get all the information.\
If you're not trying to do an english to english replacement I also recommend using [SSE Auto Translator](https://www.nexusmods.com/skyrimspecialedition/mods/111491) to automate the entire process.

It is not necessary to add the EditorID line for the following record types. But it is necessary to add the original line:
- ACTI RNAM
- FLOR RNAM
- REFR FULL
- REGN RDMP
- DIAL FULL
- INFO RNAM
- QUST CNAM
- QUST NNAM
- INFO NAM1
- MESG ITXT


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
    },
    {
        "type": "INFO RNAM",
        "original": "You... sent me to die? Traitor!",
        "string": "New string"
    },
    {
        "editor_id": "EnchFrostDamageFFContact",
        "type": "MGEF DNAM",
        "original": "Target takes <mag> points of frost damage to Health and Stamina.",
        "string": "New string"
    },
    {
        "editor_id": "DLC2TGFormula",
        "type": "BOOK DESC",
        "string": "<font face='$HandwrittenFont'>\r\nDelvin,\r\nhier ist die Rezeptur für die Verbesserte Knochenrüstung, die ich in meinem letzten Brief erwähnt habe. Behaltet das Gold, das Ihr durch den Verkauf einnehmt, und stellt sicher, dass es unserer kleinen Schwester an nichts mangelt.\r\n\r\nZutaten:\r\n- 4 Tassen Knochenstaub\r\n- 1 Tasse Salz\r\n- 1 Flasche Netchgelee\r\n- 1 Tasse gemahlenen Mammutstoßzahn\r\n- 1 Eimer frisches Wasser\r\n\r\nGebt den Knochenstaub und das Salz ins Wasser und lasst sie dort mindestens einen Tag lang einweichen. Dann gebt das Netchgelee und den Mammutstoßzahn hinzu. Rührt alles über einer heißen Flamme um, bis die Mischung sich langsam erhärtet. Gießt sie anschließend in die gewünschte Form und platziert die Form in der Mitte Eurer Schmiede. Erhitzt die Mischung mindestens einen halben Tag lang, dann lasst sie auskühlen und entfernt sie aus der Form.\r\n\r\n- Glover Mallory"
    }
]
```

To convert texts like the BOOK DESC exmaple above, you can just use [freeformatter](https://www.freeformatter.com/json-escape.html#before-output) or SSE Auto Translator.
