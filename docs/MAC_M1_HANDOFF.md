# Mac M1: az M1 firmware/SCI/MIDI-próba folytatása

Állapot: 2026-10-27. A teljes arm64 MAME kutatási build már igazolt CI-ben;
a friss firmware-es MIDI-próba még **nem futott**. Az alábbi lépéseket a Macen
folytató fejlesztő/Codex hajtsa végre a helyi környezet ellenőrzése után.
Ez nem elkészült H8-port és nem igazolt AN1x boot.

## Szükséges helyi anyagok

- A repository aktuális `main` ága; a munka elején ellenőrizni a Git-állapotot.
- A felhasználó beérkező Yamaha-mappája a Macen, ténylegesen letöltött fájlokkal.
  A Windowsos abszolút útvonalat nem szabad Mac-útvonalként használni.
- A változatlan, ismert AN1x 1.04 firmware BIN vagy ZIP formában. Többfájlos
  ZIP-nél az eredeti tag neve a `--member` kapcsolóval adható meg.
- A kipróbált kutatási fordítókörnyezet: Xcode 16.2; Python, Git, CMake és Make.
  A telepített verziókat és az arm64 architektúrát előbb helyben ellenőrizni.
  Xcode 15.4 a rögzített MAME-forrással már elbukott; ne ezt használjuk vakon.

A fizikai AN1x nem kell ehhez a szoftveres próbához. Az EG-, órajel- és
hangzásazonosság igazolása továbbra is külön hardveres/referenciamérési feladat.
ROM, bank és nyers napló nem kerülhet Gitbe vagy CI-be. A OneDrive tartalmát
helyi bemenetként kell kezelni; újabb archívumra ne cseréljük észrevétlenül
a [rögzített buildalapot](source-identity-recovery.md).

## Ellenőrzés és build

A repository gyökerében, a forrásállapot megőrzésével:

```sh
git status --short
uname -m
export DEVELOPER_DIR=/Applications/Xcode_16.2.app/Contents/Developer
xcodebuild -version
clang++ --version
python3 --version
cmake --version
make --version
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
cmake --build build --config Release --parallel 2
ctest --test-dir build -C Release --output-on-failure
python3 tools/repository_audit.py
```

Másik gépről átmásolt `build` mappát ne használjunk. Ha ilyen már van,
válasszunk új buildkönyvtárat; ne töröljünk felhasználói munkát.
Az Xcode útvonalát csak akkor állítsuk így, ha a csomag ott valóban telepítve van.

A teljes kutatási programhoz külön, tiszta MAME-checkout szükséges:

```sh
git clone --config core.autocrlf=false https://github.com/mamedev/mame.git work/mame
git -C work/mame checkout 706dbb73281320c54d7c2f16262c1c3786f2be01
python3 tools/prepare_mame_research.py --source work/mame
make -C work/mame SUBTARGET=yamanix SOURCES=src/mame/yamaha/yman1x.cpp OSD=mac NO_USE_MIDI=1 NO_USE_PORTAUDIO=1 USE_QTDEBUG=0 USE_TAPTUN=0 USE_PCAP=0 REGENIE=1 OPTIMIZE=0 SYMBOLS=0 -j2
lipo -archs work/mame/yamanix
work/mame/yamanix -listfull an1x
```

Ha `work/mame` már létezik, előbb megvizsgálni: az előkészítő szándékosan
elutasítja a módosított vagy már patchelt checkoutot. Ilyenkor használjunk
új könyvtárat, vagy igazoljuk a meglévő build eredetét; ne alkalmazzuk újra
a patch-láncot. A ROM-mentes regisztrációs próba még nem firmware-futtatás.

## Egy parancs a párosított helyi kísérlethez

A firmware útvonalát a Macen található eredeti fájlra kell cserélni.
Az eredménykönyvtárnak újnak kell lennie:

```sh
python3 tools/run_m1_serial_experiment.py \
  --mame work/mame/yamanix \
  --rom '/absolute/local/path/to/YAMAHA AN1x 1.04.BIN' \
  --work private_runs/m1-mac-first
```

A futtató generált SMF formátumú MIDI-t készít: explicit 500000 µs/negyed
tempó, 480 tick/negyed, 60-as hang, 64-es velocity, két másodperc után note-off.
A MAME fájllejátszása tíz másodperc után indul; a két futás 14-14 emulált
másodpercig tart. Külső hangszínbankot nem használ.

Mindkét friss processzusban DMS hiányzik, bridge nincs, EG-readback nulla,
DSP-mintaléptetés kikapcsolva. Csak a `passive` és `firmware-contract`
státuszprofil különbözik. Az örökölt `YAMANIX_*` és `VOP3_INTERP` környezeti
beállításokat a futtató törli, majd a gyermekpróba explicit kapcsolókat kap.
Ez szándékosan nem a több profilt egyszerre változtató régi összehasonlítás.

## Mit ellenőrzünk az eredményben?

A helyi `suite.json` tartalmazza a projekt revízióját/módosítottságát, a közös
firmware-, végrehajtható- és MIDI-azonosítókat, valamint a két SCI-csatorna
megfigyeléseit. Hibánál a suite nem lesz sikeres; a már létrejött futások és
indítási naplók megmaradnak helyben. Régi, SCI-hozzáférés-megfigyelő nélküli
MAME-programmal a suite elutasítja a hiányzó bizonyítékot.

`suite_complete=true` kizárólag a párosított megfigyelés befejezését jelenti.
A `firmware_note_handling_verified`, `complete_an1x_boot`, `hardware_validated`
és `pks_handshake_verified` mezők ettől még hamisak maradnak. RDR-olvasás
lehet elavult adat olvasása; önmagában sem ez, sem az RXD-vonal mozgása nem
bizonyítja a note-on/note-off feldolgozását.

Következő elemzés: az SCI1 inicializáló írásai és az SCI0 SCR-engedélyezések
PC-jeinek követése privát firmware-disassemblyben; szükség esetén SSR/RDR
összefüggések és ismert bájtsorozat igazolása. PKS-választ csak bizonyított
protokoll alapján szabad hozzáadni. A futtató jelenlegi szintetikus tesztjei
a szervezést és az elutasítási feltételeket vizsgálják; valódi Mac-firmware
eredménynek nem számítanak. Az új helyi eredményeket dátummal, revízióval és
tesztstátusszal fel kell vezetni a [fejlesztési tervbe](FIRST_REAPER_SOUND_PLAN.md).
