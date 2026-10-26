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

## Korlátok

48 kHz a vendorizált VOP3-modell értéke, nem az AN1x VOP3 chipjének bizonyított
mintavételezési frekvenciája. Az idő nanomásodpercre kvantált. A patch nem
tölt be táblaromot, nem vezeti át a VOP3 soros kimenetét a másik chipen vagy a
MEG-en, nem hoz létre audiofájlt, és nem módosítja a firmware-olvasásokat.
Az EG/readback és upload státuszprofil továbbra is kísérleti.

A patch sikeresen alkalmazható a helyi, hash-azonosítóval rögzített MAME
forrásra. MAME-fordítása és dinamikus firmware-futtatása ezen a mérföldkövön
még nincs elvégezve; a CI csak a ROM-mentes saját CMake projektet építi.

Következő mérés: a helyi MAME-kutatási target lefordítása és futtatása az
explicit 48 kHz-es profillal; ellenőrizni kell a két chip nemnulla és idővel
arányosan növekvő mintaszámlálóját, az állapotot, valamint a futás költségét.
Ezután külön AN1x hardvermérés kell az órajel, a mintahatár és a regiszterek
sorrendjének igazolásához.
