# Fejlesztési állapot — magyar összefoglaló

A műszaki státusz kanonikus dokumentuma az angol [STATUS.md](STATUS.md). Ez a magyar oldal a fő eredményeket és nyitott elfogadási pontokat foglalja össze.

## Ellenőrzött főági alap — #294 — 2026-11-14

A [#294 PR](https://github.com/RobCZart82/YamAN1x/pull/294)
2026-11-14 19:54:57 UTC-kor, squash módszerrel olvadt be. A pontos PR-fej
`4d4d21f254e339652c18f1f4bcd4f7d44edbaa46`, az új főági commit
`1fc79543043a5b0fcce51a640e094879c2554849`. A PR-fejen a Linux, macOS és
Windows ROM-mentes ellenőrzések sikeresek voltak (`43631172552`), az
initial-decode MSan is sikerült (`43631172629`). A pontos főági commiton a
Linux, macOS és Windows ellenőrzések (`43634657685`), valamint az
initial-decode MSan (`43634657687`) is sikeresek. Az Ubuntu-feladat kiterjesztett
UBSan/ASan ellenőrzései is sikeresek. A MAME-kutatási munkafolyamat útvonal
alapján nem futott, mert ez csak dokumentációs módosítás volt.

A Codex pontos PR-fejre szóló áttekintése nem talált hibát (összefoglaló:
`6882850078`, nyugta: `6882868957`). GitHub review-beküldés, beágyazott review-
megjegyzés és review-szál nem volt. A `25910947` szabályzat nulla jóváhagyást
és lezárt szálakat követelt; nem volt kvótahiba vagy megkerülés. A #294
kizárólag a #293 beolvadási adatait és az aktuális főági alapot egyeztette; nem
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

A #286 PR a véges próbakeretet 600 millió H8-ciklusra és 125 millió
visszahívásra emelte; a szeletmaximum 1 millió marad. Négy friss, csak olvasó AN1x
1.04 futás (AN3 0/1023, 4096/1 millió ciklusos szeletek) elérte a ciklushatárt
hozzáférési hiba nélkül. Az első AN3-konverzió ciklusa `95 706 795`, az AN0–AN3
pásztázás kezdete `95 707 121`, az AN4–AN7 pásztázásé `127 793 965`; az időpontok
mind a négy futásban egyeztek. A végső PC AN3=0 esetén `62010`, AN3=1023
esetén `1896`; a visszahívások száma szeletfüggő. Mindegyik futásnál nulla
VOP3-minta, `audio_accepted:false` és `firmware_boot_accepted:false`. Ez
szintetikus korlátozott profil, nem teljes hardver-, indulási, MIDI-, hang- vagy
REAPER-elfogadás. A ROM változatlan és a repositoryn kívül maradt.

## Nyitott elfogadási pontok

- A 39A4 rutinba írt értékek forrásának és hívóinak követése, független érték-, cím-, flag-, időzítési és folytathatósági ellenőrzéssel.
- Az indulás folytatása, a PKS-szerep és a MIDI note-on/note-off feldolgozás igazolása.
- DSP burkológörbe-, órajel- és táblázatadatok ellenőrzése; közös H8–VOP3 időalap.
- Reprodukálható első hang és WAV-összehasonlítás valódi AN1x felvétellel.
- Hosztadapter, VST3 és végül REAPER-próba.

A diagnosztikai profil és a teljes Yamaha-hardverelfogadás külön állapot. A helyi 1.04 ROM változatlan és a repositoryn kívül maradt; SHA-256: `4f72c8c595258ec9de83006287cc9b14f1450349a2253ebbf4d7437c22f95cbe`.
