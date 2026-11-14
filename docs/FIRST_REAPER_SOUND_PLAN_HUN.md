# Az első REAPER-hang terve — magyar összefoglaló

A kanonikus részletes terv az angol [FIRST_REAPER_SOUND_PLAN.md](FIRST_REAPER_SOUND_PLAN.md). Ez az oldal elkülönítve foglalja össze a függőségi sorrendet.

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

Négy friss, csak olvasó AN1x 1.04 futás 600 millió ciklusig jutott hozzáférési
hiba nélkül; a profil 125 millió callbacknél és legfeljebb 1 millió ciklusos
szeleteknél megáll. AN3=0 és 1023 mellett, 4096 és 1 millió ciklusos
szeletekkel az első ADC- és pásztázási határidők egyeztek. A futások végén nulla
VOP3-minta volt, a teljes indulás és a hangelfogadás eredménye hamis maradt. Ez korlátozott,
szintetikus megfigyelés; nem teljes hardver- vagy hangelfogadás.

## Mérföldkövek

1. **Firmware és perifériák:** kis, bizonyítékhoz kötött H8, busz, MMIO és IRQ lépések; minden elfogadott profil után új firmware-futtatás. A konstans bemenetű szintetikus diagnosztika nem hardverkalibráció.
2. **Boot, PKS és MIDI:** a firmware további indulása, a PKS szerepe, majd a note-on/note-off üzenetek vétele külön-külön bizonyítandó.
3. **DSP:** az EG célérték-megtartása, az órajel és a hullámtáblák formátuma/címzése ellenőrzendő. Ezután közös időalapon kell léptetni a H8-at és a két VOP3 chipet.
4. **Első száraz hang:** rögzített patch, note-on, tartás és elengedés; dokumentált kétchipes jelút és kimeneti skála; reprodukálható WAV, majd összehasonlítás a hangszerből rögzített referenciával.
5. **Hosztintegráció:** csak a jelút igazolása után jöhet a motor, MIDI-időzítés és minimális VST3, majd a REAPER-ben lejátszott első hang.

A REAPER-elfogadás az utolsó lépés. Részprofil, teljes hardverhű indulás, MIDI, offline hang és REAPER-hang nem vonható össze egyetlen sikerállítássá.
