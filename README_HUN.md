# YamAN1x — magyar összefoglaló

A YamAN1x a Yamaha AN1x hangszer firmware-alapú emulációját kutató projekt. A cél egy későbbi VST3/AU hangszerbővítmény. A jelenlegi kód kutatási alap: még nem ad AN1x-hangot, nem igazol teljes, hardverhű bootot, és nincs benne kész bővítmény vagy grafikus kezelőfelület.

Az adattár dokumentációja nyelvenként külön fájlban található: az angol dokumentumok nem tartalmaznak magyar prózát, a magyar összefoglalók pedig külön `_HUN.md` fájlokban találhatók. Az angol dokumentáció a kanonikus műszaki leírás. A gyökérben lévő `README.md` angolul tartalmazza a projekt bemutatását és állapotát. A részletes MAME-kutatási útmutató a [külön magyar fájlban](research/README_HUN.md) található. A korábbi magyar nyelvű dokumentumrészeket a [külön magyar archívum](docs/HUNGARIAN_NOTES_HUN.md) őrzi.

## Ellenőrzött főági alap — #292 — 2026-11-14

A [#292 PR](https://github.com/RobCZart82/YamAN1x/pull/292)
2026-11-14 14:52:58 UTC-kor, squash módszerrel olvadt be. A pontos PR-fej
`7ae2fccee343257a67acb7541d30f191d2f925fe`, az új főági commit
`6039cc3833c35a278517e4541ed3d84fb527bb2b`. A PR-fejen a Linux, macOS és
Windows ROM-mentes ellenőrzések sikeresek voltak (`43603781411`), az
initial-decode MSan is sikerült (`43603781407`). A pontos főági commiton a
Linux, macOS és Windows ellenőrzések (`43607171460`), valamint az
initial-decode MSan (`43607171485`) is sikeresek. Az Ubuntu-feladat kiterjesztett
UBSan/ASan ellenőrzései is sikeresek. A MAME-kutatási munkafolyamat útvonal
alapján nem futott, mert ez csak dokumentációs módosítás volt.

A Codex pontos PR-fejre szóló áttekintése nem talált hibát (összefoglaló:
`6878585874`, nyugta: `6878605101`). GitHub review-beküldés, beágyazott review-
megjegyzés és review-szál nem volt. A `25910947` szabályzat nulla jóváhagyást
és lezárt szálakat követelt; nem volt kvótahiba vagy megkerülés. A #292
kizárólag a #291 beolvadási adatait és az aktuális főági alapot egyeztette; nem
változtatott a működésen és nem igazol hardver-, boot-, MIDI-, DSP-, hang- vagy
REAPER-elfogadást.

## Friss, korlátozott helyi firmware-megfigyelés — 2026-11-14

A csak olvasó AN1x 1.04 próba az összeolvasztott főági builddel elérte a kért
20 000 000 ciklusos határt CPU-hiba vagy hozzáférési hiba nélkül. A futásban
3 184 647 CPU-visszahívás, `20 000 000` perifériaciklus, `247044` PC,
5/1 MEG-vezérlési/állapotolvasási művelet, `[11557,516]` VOP3-írás,
`[448,0]` VOP3-olvasás, `[0,0]` VOP3-minta és 6329 GPIO-írás volt; ADC-minta
nem keletkezett. Az `audio_accepted` és `firmware_boot_accepted` értéke hamis
maradt. Ez csak korlátozott előrehaladás a kiválasztott szintetikus profilban;
nem teljes indulás, hardveregyezés, MIDI-, hang- vagy REAPER-elfogadás. A ROM
SHA-256 értéke előtte és utána is
`4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`; változatlan
és Git-en kívül maradt. Új MAME-build vagy futás nem készült.

## PKS-bemeneti időablakok — helyi jelölt — 2026-11-14

A csak olvasó megfigyelő a korábban látott címek mellett a statikusan
azonosított, igazított `2349EA` szót is rögzíti. A rögzített MAME-forrással
két friss, kilenc másodperces, passzív alapbeállítású próba készült; mindkettő
hat `39AC` forrás-pufferírást fogott meg. A `002A08` utólagos programszámláló-
értéknél olvasott `2349EA` adat `4000/FF00` volt (kijelölt felső bájt: `40`);
az első három írást 12 625, 20 125 és 27 625 ns-mal előzte meg. Az írt bájtok
`E0 00 20` voltak. A `001FD8` utólagos értéknél olvasott `237204` igazított szó
adata `FF00/00FF` volt (kijelölt alsó bájt: `00`); a következő három írást
10 375, 17 875 és 25 375 ns-mal előzte meg. Az írt bájtok `E0 00 00` voltak.
A CCR-pillanatképek nyers adatként maradnak meg, értelmezés nélkül. A két futás
26 nem trace-jellegű mezője egyezett. Az időbeli közelség nem bizonyít okozati
kapcsolatot vagy adatjelentést; hardveregyezés, teljes indulás, MIDI, DSP-minta,
hang és REAPER-elfogadás sincs igazolva. A ROM változatlan maradt, és a Git-en
kívül van. A következő jelölt lépés a korábbi utasítás/adatérték út követése,
független cím-, érték-, bájt-lane-, flag-, időzítési és folytathatósági
ellenőrzéssel.

## Beolvadt korlátozott firmware-diagnosztika — #286 — 2026-11-14

A #286 PR az explicit próba véges korlátját 600 millió H8-ciklusra
és 125 millió visszahívásra emelte; a szeletmaximum 1 millió marad. Négy friss,
csak olvasó AN1x 1.04 futás (AN3 0/1023, 4096/1 millió ciklusos szeletekkel)
elérte a ciklushatárt hozzáférési hiba nélkül. Az első AN3-konverzió ciklusa
`95 706 795`, az AN0–AN3 pásztázás kezdete `95 707 121`, az AN4–AN7 pásztázásé
`127 793 965`; ezek az időpontok a négy futásban egyeztek. A végső PC AN3=0
esetén `62010`, AN3=1023 esetén `1896`; a visszahívások száma szeletfüggő.
Mind a négy futásnál nulla VOP3-minta, `audio_accepted:false` és
`firmware_boot_accepted:false`. Ez szintetikus korlátozott profil, nem
hardver-, teljes indulási, MIDI-, hang- vagy REAPER-elfogadás. A ROM változatlan
és a repositoryn kívül maradt; a rögzített MAME forrás nem épült újra és nem
futott.

## Mi van még hátra?

1. A firmware és a H8 perifériák további, bizonyítékokkal alátámasztott integrációja, majd az indulás előrehaladásának friss mérése.
2. A PKS-szerep és a MIDI note-on/note-off út külön igazolása; ezután következhet a firmware további indulási útjának vizsgálata.
3. A DSP envelope- és órajel/táblázat-kérdéseinek rendezése, majd időzített H8–VOP3 futtatás.
4. Egy rögzített hangszínnel ellenőrizhető első hang, reprodukálható WAV és összehasonlítás valódi AN1x-felvétellel.
5. Ezután következhet a motor, a minimális hosztadapter és a REAPER-próba.

A szintetikus tesztprofilokat és a teljes hardverelfogadást külön kell kezelni. Egy korlátos diagnosztika sikere nem jelent hardverhűséget vagy kész hangszert.

## ROM-mentes összeállítás és ellenőrzés

```sh
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release --parallel 4
ctest --test-dir build -C Release --output-on-failure
python3 tools/repository_audit.py
```

A tesztek szintetikus képeket használnak ideiglenes könyvtárakban. A helyi Yamaha-firmware-kép nem szükséges a buildhez vagy ehhez a tesztkészlethez. A magyar állapotösszefoglalót a [STATUS_HUN.md](docs/STATUS_HUN.md), a mérföldkövek rövid tervét a [FIRST_REAPER_SOUND_PLAN_HUN.md](docs/FIRST_REAPER_SOUND_PLAN_HUN.md) tartalmazza. A részletes műszaki napló az angol [STATUS](docs/STATUS.md), [első hang terv](docs/FIRST_REAPER_SOUND_PLAN.md) és [fejlesztési napló](docs/DEVELOPMENT_LOG.md) dokumentumokban olvasható.
