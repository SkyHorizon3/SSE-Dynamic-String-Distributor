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

It is not necessary to add the EditorID line for the following record types:
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
        "editor_id": "CACO_FoodMeatTernCooked_HOC",
        "type": "ALCH FULL",
        "original": "Cooked Tern",
        "string": "Gegarte Tern"
    },
    {
        "editor_id": "DBJarrinRoot",
        "type": "INGR FULL",
        "original": "Jarrin Root",
        "string": "Jarrinwurzel"
    }
]
```