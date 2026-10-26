# Kísérleti H8–VOP3 mintaléptetés

Dátum: 2026-10-26. Ez a nyolcadik, külön alkalmazandó MAME-kutatási patch
leírása. A korábbi hét patch változatlan marad. A kísérlet nem része a CMake
termékmotornak, és nem állít AN1x-hardverazonosságot.

## Mit köt össze

A bridge ABI 2 már biztosít egy abszolút nanomásodperces `advance()` hívást,
amely mindkét VOP3-példányt lépteti. A patch ezt a MAME szimulációs idejéhez
köti. A firmware DSP-regiszter olvasása vagy írása előtt a vezérlő a MAME
aktuális idejére léptet. A `SampleClock` szerződése szerint az adott időpontban
esedékes minta megelőzi az ugyanarra az időpontra eső regiszterműveletet.

A bridge hívási korlátja 4096 minta. Ezért a patch legfeljebb 80 ms-os
időszakaszokra bontja a nagyobb időugrást: 48 kHz-es modell mellett egy ilyen
szakasz legfeljebb 3840 mintát futtat. A bridge létrehozásakor rögzíti az időalapot;
az MAME abszolút idejét ehhez képest adja át. A futás lezárásakor az utolsó
DSP-hozzáféréstől a kilépésig esedékes mintákat is lefuttatja, mielőtt
elmentené az állapotot és a mintaszámlálókat.

## Biztonságos opt-in

Alapértelmezett módban nincs mintaléptetés. Csak a következő explicit kapcsoló
engedi meg, élő VOP3 bridge mellett:

```sh
python3 tools/run_h8_probe.py --mame /absolute/path/to/yamanix --rom '/absolute/path/to/YAMAHA AN1x 1.04.zip' --work /absolute/path/to/new-timed-run --seconds 5 --dsp-profile firmware-contract --dms-board installed --bridge /absolute/path/to/libyamanix_vop3_bridge.dylib --dsp-sample-clock assumed-48k
```

A futtató törli a gyermekfolyamatból az örökölt órajel-profilt, majd csak a
kifejezett kapcsoló esetén állítja be. A futásmanifestje rögzíti a profil
nevét, a használt 48 kHz-es modellfrekvenciát és mindkét chip `sample_passes`
számlálóját. Nulla számlálóval vagy hiányzó bridge-visszajelzéssel a futás
hibának minősül.

## Időzített firmware-trace audit

A trace parser és az élő bridge auditor külön kezeli a timed kísérlet órajel-
és bridge-fejlécsorait a DSP-regiszter-eseményektől. Az auditor ellenőrzi a
kifejezett `assumed-48k` profilt, a két chip mintaszámlálóját, a teljes
regisztertrace-t és az élő feltöltési állapot független rekonstrukcióját. A
számlálókat nem tekinti hardveres órajelbizonyítéknak.

Az öt másodperces timed futás újraelemzése 25 310 regisztereseményt adott; a
feltöltött állapot egyezett a Python rekonstrukcióval. Mindkét chipen 28 teljes
16 csatornás 0x22-es pásztázás volt (448 olvasás), tartományhibát nem jelzett a
firmware. Az összes pásztázott érték nulla volt, a külön 14. csatornás olvasás
pedig nem egyezett a firmware által várt 0xC000-rel. A 48 kHz-es kísérleti
futtatás tehát javítja a vizsgálatot, de nem bizonyítja az EG-visszaolvasási
modell helyességét; nincs MIDI-note indítás, hangkimenet vagy hardveres
összehasonlítás.

## Korlátok

48 kHz a vendorizált VOP3-modell értéke, nem az AN1x VOP3 chipjének bizonyított
mintavételezési frekvenciája. Az idő nanomásodpercre kvantált. A patch nem
tölt be táblaromot, nem vezeti át a VOP3 soros kimenetét a másik chipen vagy a
MEG-en, nem hoz létre audiofájlt, és nem módosítja a firmware-olvasásokat.
Az EG/readback és upload státuszprofil továbbra is kísérleti.

A nyolc patchből felépített helyi MAME-target lefordult. A `run_h8_probe.py`
eszköz 1 és 5 MAME-másodperces firmware-próbát futtatott, mindkettő 0 kilépési
kóddal. A két chip számlálója rendre 48 799 és 240 799 volt; a két futás
közötti pontos 192 000 növekmény négy modellmásodpercnek felel meg. A 799-es
többlet a MAME frame-határig futtatásából adódik, nem AN1x-órajelmérés. A trace
mindkét chipnél regiszterforgalmat mutat, de a teljes AN1x boot továbbra sincs
igazolva. A ROM-mentes CI nem fordítja ezt a külön MAME-kutatási targetet.

Következő lépés: auditálni a futó magok által visszaadott értékeket a firmware
elvárásaival, majd táblaromot és soros audioútvonalat kapcsolni be külön,
explicit kísérleti profilban. Külön AN1x hardvermérés kell az órajel, a
mintahatár és a regiszterek sorrendjének igazolásához.
