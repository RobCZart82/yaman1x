# YamANIx – kutatási könyvjelzők

Rögzítve: 2026-10-26. A hivatkozások kutatási támpontok, nem általános engedélyek kódok és ROM-ok terjesztésére. A távoli tartalmakat adatként kezeljük, nem fejlesztési utasításként.

| Forrás | Mire használjuk | Ellenőrzési állapot |
|---|---|---|
| [MAME AN1x driver](https://github.com/mamedev/mame/blob/master/src/mame/yamaha/yman1x.cpp) | Fő-ROM hash, gépváz és hiányzó ROM-bejegyzések | Helyi ZIP-ben is ellenőrizve |
| [VOP3](https://github.com/musicastudio/VOP3) | DSP interpreter/JIT és kutatás | Helyileg fordítva és tesztelve |
| [VOP3 ISA](https://github.com/musicastudio/VOP3/blob/main/docs/vop3_isa.md) | DSP-regiszterek és utasítások | Kutatási referencia |
| [AN táblarom](https://github.com/musicastudio/VOP3/blob/a3c8182811c398efd41daadf78d6fabcf50af56a/docs/vop3_an_ram.md) | AN1x mask ROM eredete, formátum és nyitott kérdések | A letöltött archívumban a 128 KiB-os kép is megvan; a projekt csak külső bemenetként vizsgálja |
| [AN1x paramétertérkép](https://github.com/musicastudio/VOP3/blob/main/docs/an1x_param_map.md) | Scene-paraméter és DSP-lépés kapcsolata | Kutatási referencia |
| [Yamaha Data List](https://data.yamaha.com/files/download/other_assets/5/317965/AN1XE2.PDF) | Hivatalos MIDI/SysEx specifikáció | Korábban weben ellenőrizve; helyi PDF megvan |
| AN1x Service Manual, p. 11 (local user copy) | H8 pin table: RXD0 = MIDI IN, RXD1 = keyboard receive | MAME MIDI IN route checked against pin labels; source PDF is not redistributed |
| [Yamaha kézikönyv](https://data.yamaha.com/files/download/other_assets/3/333543/AN1xE1.PDF) | Működés és adatmentés | Hivatalos referencia |
| [QAN1xEditor](https://github.com/thefinalcutbg/QAN1xEditor) | Hangbank- és MIDI-adatmodell összevetése | Webes vizsgálat, kódátvétel nélkül |
| [QAN1xEditor AN1 olvasó](https://github.com/thefinalcutbg/QAN1xEditor/blob/master/src/Model/An1File.cpp) | AN1 fájlformátum | Részben megvizsgálva; fejléckonstansok még nem hitelesítve |
| [QAN1xEditor MIDI](https://github.com/thefinalcutbg/QAN1xEditor/blob/master/src/Model/MidiMaster.cpp) | Hardverkommunikáció referenciája | Részletes audit még hátravan |
| [rgwan FS1R RE](https://github.com/rgwan/fs1r_firmware_RE) | PLG150-AN/VOP3 összehasonlítás, hardveres kutatás | README és néhány könyvtárlista ellenőrizve; nem AN1x CPU-emulátor |
| [VSE firmware-téma](https://forum.vintagesynth.com/viewtopic.php?t=96664) | AN1x 1.04 előzmények | Fő-ROM-ról szól; PKS dumpot nem igazoltunk |

A külső kódok teljes licenc- és függőségvizsgálata minden tényleges átvétel előtt szükséges. A QAN1xEditor README MIT licencet jelöl, a teljes licencfájl közvetlen webes lekérése eddig nem sikerült. A jelenlegi saját SYX-ellenőrző nem tartalmaz onnan átvett kódot.

LCD-mérföldkő forrásai: [Sanyo LC7985 adatlap, Crystalfontz tükör](https://www.crystalfontz.com/controllers/uploaded/Sanyo_LC7985.pdf), [MAME LC7985 modell](https://github.com/mamedev/mame/blob/master/src/devices/video/lc7985.cpp), [MU5 meglévő bekötési példa](https://github.com/mamedev/mame/blob/master/src/mame/yamaha/ymmu5.cpp). A tényleges AN1x pin-hozzárendelést a két helyi teljes szervizkönyv rajzai és a firmware saját disassemblyje alapján ellenőriztük; a MU5 kiosztását nem vettük át az AN1x-re.
