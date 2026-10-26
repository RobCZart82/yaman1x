# H8 kutatási futtató

A `mame-headless-trace.patch` kizárólag a `docs/source-lock.json` MAME ZIP-jéből kibontott munkapéldányhoz készült. A Yamaha firmware tartalmát nem módosítja. Az upstream MAME-forrás változatlanul újra előállítható az eredeti archívumból.

Az első patch elhagyja az upstream, még nem bekötött MULCD eszközt és annak font/SVG követelményeit. A harmadik, `mame-lcd-controller.patch` valódi LC7985 vezérlőmodellt köt a H8 portjaihoz; a két változat közötti különbség mérhető a naplókban. Az első négy patch megfigyelő tapjai nem írják át az olvasási/írási adatot; az ötödik és hatodik patch kísérleti válaszai külön kapcsolhatók be, lásd lent. Az első 512 különböző olvasási és 512 írási címet rögzítik a `0x300000–0xFFFFFF` tartományban; ez nem teljes utasítás- vagy busztrace. PC-mintavételezés az első másodpercben 10 ms-onként, utána másodpercenként történik a legfeljebb 10 másodperces próbához.

macOS kutatási build (a kibontott MAME gyökerében):

```sh
patch -p1 < /absolute/path/to/mame-headless-trace.patch
patch -p1 < /absolute/path/to/mame-no-cocoa.patch
patch -p1 < /absolute/path/to/mame-lcd-controller.patch
patch -p1 < /absolute/path/to/mame-dsp-capture.patch
patch -p1 < /absolute/path/to/mame-upload-control-experiment.patch
patch -p1 < /absolute/path/to/mame-live-register-bridge.patch
patch -p1 < /absolute/path/to/mame-eg-readback-experiment.patch
patch -p1 < /absolute/path/to/mame-timed-vop3-experiment.patch
patch -p1 < /absolute/path/to/mame-midi-input.patch
make SUBTARGET=yamanix SOURCES=src/mame/yamaha/yman1x.cpp OSD=mac NO_USE_MIDI=1 NO_USE_PORTAUDIO=1 USE_QTDEBUG=0 USE_TAPTUN=0 USE_PCAP=0 REGENIE=1 -j4
```

Indítás a saját projekt gyökeréből, új munkamappával:

```sh
python3 tools/run_h8_probe.py --mame /absolute/path/to/yamanix --rom '/absolute/path/to/YAMAHA AN1x 1.04.zip' --work /absolute/path/to/new-probe-directory --seconds 5
```

A futtató változatlan tartalmú, helyi kutatási ROM-ZIP-et készít, majd naplókat ment. Ezek a ROM-adatok nem valók nyilvános repositoryba. NVRAM-mentés kikapcsolva. A megfigyelés nem jelenti a teljes gép sikeres bootját: PKS letiltva, VOP3 és effektintegráció hiányos. Ez külön MAME-kutatási program, nem a pluginba linkelt CPU-könyvtár; a MAME teljes licencfeltételei érvényesek rá.

A `mame-no-cocoa.patch` a kutatási Mac-változatból elhagyja a Cocoa alkalmazásindítást, és `-video none` esetén csak memóriabeli rendercélt készít. Nem nyit ablakot és nem igényel monitort. Ezt a változatot csak a fenti headless parancssorral használjuk, nem általános MAME grafikus futtatásra. A H8 CPU vagy perifériák logikáját nem módosítja.

A `mame-lcd-controller.patch` új saját fejlécet is telepít a driver mellé; annak forrása a projekt `src/lcd_bus.h` állománya. A MAME LC7985 regiszter-, memória- és busy-időzítését változatlanul használja. Csak a nem használt grafikus karakterkészlet helyén van üres, helyi régió; a betűrajzok így nem hitelesek és nem kerülnek megjelenítésre. A firmware és a H8 mag változatlan. A patch néhány korai utasítás disassemblyjét is naplózza a kézfogás ellenőrzéséhez.

A jelenlegi mérés friss processzusú hidegindításra készült. Save-state, visszatöltés és több egymást követő szoftveres reset működését még nem validáltuk.

## Teljes DSP-regiszternapló

A negyedik patch minden olvasást és írást rögzít a `0x440000–0x4400FF` és `0x480000–0x4800FF` tartományból, a korábbi címenkénti mintavétel mellett. A CPU által látott adatot és memóriamaszkot nem módosítja. Megmarad az eseménysorrend, a nanomásodpercben megadott gépidő és a programcím. A 200 000 eseményes korlát elérésekor a napló veszteségesnek minősül, és az ellenőrző elutasítja. A szabályos kilépéskor kiírt zárósor kötelező; összeomláskor megmaradt résznaplóból nem lesz elfogadott teljes felvétel.

Az elemzés és visszajátszás a saját projekt gyökeréből:

```sh
python3 tools/dsp_trace.py /absolute/path/to/trace.log --events-out /absolute/path/to/events.tsv --snapshot-out /absolute/path/to/python-state.bin
/absolute/path/to/yamanix_dsp_replay /absolute/path/to/events.tsv /absolute/path/to/cpp-state.bin
cmp /absolute/path/to/python-state.bin /absolute/path/to/cpp-state.bin
```

Ez feltöltési audit. A C++ eszköz a tényleges VOP3 könyvtár `write()` interfészén játssza vissza a rögzített írásokat, de DSP-mintát nem futtat, a rögzített olvasásokat nem tekinti helyes hardverválasznak. A Python és a C++ kimenetének egyezése a SELECT/program/együttható/tag/slot állapotot ellenőrzi; nem bizonyít teljes chipállapot- vagy hangazonosságot. Részleges szóhozzáférés egyelőre hiba. A nyers felvételek és állapotképek a `work/` alatt maradnak, nem részei a publikálható saját forrásnak.

## Kísérleti feltöltésvezérlő profil

Az ötödik patch alapból csak megfigyel: a passzív módban az előző 6441 esemény változatlanul reprodukálható. A külön `--dsp-profile firmware-contract` kapcsoló egy nem hardvervalidált, logikai kézfogást engedélyez a 0x01 regiszteren. Csak ebben a profilban írja át a memóriatap a státuszolvasás eredményét. A vezérlőparancs 1 értékéhez 0x8000 státuszt, 0 értékéhez 0 státuszt rendel; az elismerés késleltetése és a többi bit nullája feltételezés. Ismeretlen parancs és részleges hozzáférés hibát okoz.

```sh
python3 tools/run_h8_probe.py --mame /absolute/path/to/yamanix --rom '/absolute/path/to/YAMAHA AN1x 1.04.zip' --work /absolute/path/to/new-contract-run --seconds 5 --dsp-profile firmware-contract
python3 tools/an1x_upload.py /absolute/path/to/VOP3-main/docs/vop3_an1x/voice.bin --trace /absolute/path/to/new-contract-run/trace.log --events-out /absolute/path/to/image-upload.tsv
/absolute/path/to/yamanix_dsp_replay /absolute/path/to/image-upload.tsv /absolute/path/to/image-state.bin
```

A futtató a gyermekprocesszus környezetében beállítja a `YAMANIX_DSP_PROFILE` értékét, a kiválasztott profilt pedig a manifeszt és a trace megőrzi. Direkt MAME-indításkor az elmaradó változó passzív módot jelent, ismeretlen érték hiba. A képfeltöltő adapter statikus, nullás időbélyegű referenciaírásokat állít elő; nem valós idejű feltöltési ütemezés. A 128 slot egyedi nemnulla csoportoffsetjét az első DSP tényleges firmware-feltöltése visszaigazolta ehhez a voice.bin képhez. Más, ellentmondó képet az adapter elutasít.

## DMS-panel és élő regiszterkapcsolat (macOS)

A hatodik patch a második panel érzékelését teszi választhatóvá, megfigyeli a firmware jelenlétflag-írását, és opcionális C ABI könyvtárbetöltőt ad a kutatási Mac-driverhez. Alapból továbbra is a korábbi passzív, hiányzó panelt mutató állapot aktív, bridge nélkül. A két környezeti változó: `YAMANIX_DMS_BOARD=absent|installed` és `YAMANIX_VOP3_BRIDGE=/absolute/path/to/library`; a futtató ezeket mindig explicit kezeli. A bridge csak a `firmware-contract` profil mellett engedélyezett. Ismeretlen profil, ABI-eltérés és részleges szóhozzáférés hiba.

A saját projekt gyökeréből előbb fordítsd le a CMake projektet a gyökér README szerint; ez készíti a `libyamanix_vop3_bridge.dylib` könyvtárat. Az új patchben lévő C fejléc a saját `src/vop3_bridge.h` másolata. A VOP3 külső kódja változatlan marad; a MAME nem tartalmaz közvetlen VOP3-fordítási függőséget. Ez a futtatási felület külön kutatási programhoz készült, nem kész plugin-motor; az egyes komponensek licencfeltételei továbbra is érvényesek.

```sh
python3 tools/run_h8_probe.py --mame /absolute/path/to/yamanix --rom '/absolute/path/to/YAMAHA AN1x 1.04.zip' --work /absolute/path/to/new-live-run --seconds 5 --dsp-profile firmware-contract --dms-board installed --bridge /absolute/path/to/libyamanix_vop3_bridge.dylib
python3 tools/dsp_trace.py /absolute/path/to/new-live-run/trace.log --events-out /absolute/path/to/events.tsv
/absolute/path/to/yamanix_dsp_replay /absolute/path/to/events.tsv /absolute/path/to/replay-state.bin
python3 tools/live_bridge_audit.py /absolute/path/to/new-live-run --replay-snapshot /absolute/path/to/replay-state.bin
python3 tools/an1x_upload.py /absolute/path/to/VOP3-main/docs/vop3_an1x/voice.bin --trace /absolute/path/to/new-live-run/trace.log --chip 0
python3 tools/an1x_upload.py /absolute/path/to/VOP3-main/docs/vop3_an1x/voice.bin --trace /absolute/path/to/new-live-run/trace.log --chip 1
```

A `--dms-board installed` csak P92 bemeneti szintjét húzza alacsonyra, a szervizrajz DM CN11 pin 1 → DMS CN1 pin 1 → DG útvonala alapján. A jelenlétflaget a változatlan firmware állítja be. A flag megfigyelője az érintett RAM-bájt írásait naplózza, nem módosítja őket.

A bridge az írásokat ténylegesen, a H8 futása közben továbbítja a két VOP3-objektumhoz. Nem hív `Vop3::run()` mintafuttatást; az ABI 1 egyetlen szálú, regiszterekre korlátozott felület. A 0x01 olvasást a kísérleti feltöltésvezérlő, a többit a VOP3 `read()` kezeli. **Az AN1x által olvasott 0x22 a jelenlegi VOP3 magban nincs modellezve: nulla helyettesítő választ kap, külön nem támogatott olvasásként számlálva.** Az egyes konfigurációs és host-RAM írások modellje is hiányos.

Kilépéskor `live-vop3-state.bin` és két chipzárósor készül. Az auditor megköveteli a teljes trace-t, a konzisztens profiljelölést és számlálókat, majd a SELECT/program/együttható/tag/slot mezőket hasonlítja össze a független rekonstrukcióval és az opcionális natív visszajátszással. Nem teljes chipállapotot vagy hangazonosságot auditál. A nyers felvételek és firmware-eredetű állapotképek helyi kutatási adatok, nem nyilvános csomag részei. Részletes eredmény: `docs/live-register-milestone.md`.

## Kiválasztott EG-szint és ABI 2

A hetedik, `mame-eg-readback-experiment.patch` az AN1x 1.04 szint-visszaolvasási rutinjait naplózza disassemblyként, és a `--eg-readback zero|an1x-level-experiment` kapcsolót vezeti be. A MAME és a saját bridge egyaránt újrafordítandó: a driver ABI 2-t követel, a régi ABI 1 könyvtárat elutasítja. Az első hat patch önmagában az előző ABI 1 kutatási állapotot állítja elő. A mostani auditor mindkét formátum korábbi felvételeit ellenőrzi.

Az új profil a szoftveres EG `level × 64` értékét adja vissza a kiválasztott 0..15 csatornára, nulla felé csonkolva, int16-ra telítve. Ez részben firmware-ből levezetett, **nem hardvervalidált kísérlet**, ezért alapból a korábbi `zero` profil marad. A 0x21 státusz és a 0x22 szint külön felület. Nem érvényes csatorna, ismeretlen profil vagy bridge nélküli kísérleti profil hiba. A futtató explicit beállítja a `YAMANIX_EG_READBACK` értékét, tehát a gyermekprocesszus nem örökölhet észrevétlenül másik profilt.

```sh
python3 tools/run_h8_probe.py --mame /absolute/path/to/yamanix --rom '/absolute/path/to/YAMAHA AN1x 1.04.zip' --work /absolute/path/to/new-eg-run --seconds 5 --dsp-profile firmware-contract --dms-board installed --bridge /absolute/path/to/libyamanix_vop3_bridge.dylib --eg-readback an1x-level-experiment
python3 tools/eg_readback_audit.py /absolute/path/to/new-eg-run
python3 tools/live_bridge_audit.py /absolute/path/to/new-eg-run
ctest --test-dir /absolute/path/to/own-build --output-on-failure
```

A library új `yamanix_vop3_advance(handle, absolute_ns)` függvényét ebben a mérföldkőben kizárólag a saját szintetikus teszt hívja. Mindkét chip valódi `run()` függvényét lépteti, legfeljebb 4096 mintával/hívás a core modelljének 48 kHz-én. Az AN1x hardverórajele ezzel nincs igazolva. A MAME-driver nem hívja az időléptetőt: az élő firmware-próba továbbra is regiszterkezelést mér, nulla mintafuttatással. A 14. csatorna firmware-beli 0xC000 feltétele a jelenlegi, nullás modellállapottal nem teljesül.

Az új teszt a külső EG-modellben eltérést mutat a célszinten megállást állító leíráshoz képest: a cél keresztezése után is közelít az aim felé. A sikertelen célon tartást a `docs/eg-readback-milestone/model-finding.json` rögzíti; a sikeres tesztek nem jelentik ennek a viselkedésnek a hardveres validálását. A VOP3 forrása változatlan. Az eredmények és a következő motorfeladatok: `docs/eg-readback-milestone.md`.

## Kísérleti H8–VOP3 mintaléptetés

A nyolcadik patch a bridge már létező `advance(absolute_ns)` függvényét köti a MAME szimulációs idejéhez. Alapértelmezetten kikapcsolva marad. Csak a `--dsp-sample-clock assumed-48k` és a kötelező élő bridge mellett fut; 48 kHz a külső VOP3-modell beállítása, **nem igazolt AN1x-órajel**. A DSP-MMIO esemény előtt a MAME-idő szerint esedékes minták futnak le, így a mintahatárral egybeeső tick megelőzi az olvasást/írást. A legfeljebb 80 ms-os lépések tartják a bridge 4096-mintás hívási korlátját.

```sh
python3 tools/run_h8_probe.py --mame /absolute/path/to/yamanix --rom '/absolute/path/to/YAMAHA AN1x 1.04.zip' --work /absolute/path/to/new-timed-run --seconds 5 --dsp-profile firmware-contract --dms-board installed --bridge /absolute/path/to/libyamanix_vop3_bridge.dylib --dsp-sample-clock assumed-48k
```

A futás manifesztje elmenti a kiválasztott profilt, modellfrekvenciát és mindkét chip mintaszámlálóját. A profil bekapcsolása nem aktiválódhat örökölt környezeti változótól. A teljes MAME-kutatási target lefordult, az 1 és 5 másodperces firmware-próba sikeresen befejeződött, és mindkét chip számlálója 48 799, illetve 240 799 volt. Az öt másodperces mintaszám a MAME frame-végéig futásából adódó 799-es többletet tartalmaz; ez nem hardverórajel-mérés. Nincs táblarom-bekötés, soros hangút vagy AN1x-hardveres validálás; lásd [az időzített kísérlet feljegyzését](../docs/timed-vop3-experiment.md).

## Kísérleti MIDI IN soros bemenet

A kilencedik, `mame-midi-input.patch` a szervizkönyv pin-táblája alapján a
MAME `midiin` portját az H8 RXD0 / SCI0 bemenetére köti. A `run_h8_probe.py`
`--midi-file` kapcsolóval külső `.mid` vagy `.syx` fájlt ad át a MAME-nek; a
fájl nem másolódik be a futásmappába. A MAME a fájlból legalább 10 másodperc
után kezd adatot küldeni, ezért a próbához `--seconds 11` vagy több szükséges.

Egy generált note-on/note-off MIDI-fájl 12 és 30 másodperces futása egyaránt 21
RXD0-vonalátmenetet adott. Az SCI0 SCR egyetlen megfigyelt írása `0x00` volt,
az RDR-t pedig a firmware nem olvasta. Ez a bekötést és a soros vonal aktivitását
igazolja; a UART-bájt-vételt, a firmware eseményfeldolgozását és a hangot nem.
Részletek: [MIDI IN mérföldkő](../docs/midi-input-milestone.md).
