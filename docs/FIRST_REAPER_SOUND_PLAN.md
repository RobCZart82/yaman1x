# YamAN1x: fejlesztési terv az első REAPER-ben megszólaló hangig

Rögzítve: 2026-10-27. Kiindulópont: a #14 PR-ig beolvadt kutatási eredmények.
Ez végrehajtási terv; az alábbi, még nyitott mérföldkövek nem kész funkciók.
Állapotellenőrzés: 2026-10-29, motoralap: #116 (`7fe1d4bcb602ce96d40f34ec4ccd888430bf70e0`).
A működő H8H-részhalmaz nem teljes CPU- vagy firmware-elfogadás.

## Aktuális lezárás — #116, 2026-10-29

A [#116 PR](https://github.com/RobCZart82/yaman1x/pull/116) beolvadt:
merge `7fe1d4bcb602ce96d40f34ec4ccd888430bf70e0`,tesztelt head `1982509b2c4632b7e9198d3ae23656f59e2c2a55`.
Időpont:2026-10-29 23:25:39 Europe/Budapest (22:25:39 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649310), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649254), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40963649115):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **75 CTest-csoport /87 Python-eset**;CPU-kezelők változatlanul562
(558 változatlan,4 korábbi adaptáció),264 kiválasztott/0 hiányzó/135 kizárt sor.
1MiB saját ROM-másolat,külső256KiB NVRAM és512 bájt belső RAM,24 bites címtükrözés.
Teljes ROM/RAM-söprés,határok/izoláció/NVRAM-roundtrip és241 CPU-partíció PASS.
Helyi Release75/75 CTest,87 Python és célzott ASan/UBSan PASS.
[Bizonyíték és kutatási szabályok](an1x-memory-map.md).
Nincs aktív motorjelölt. MMIO és ROM-írás fail-closed,nem fizikai open-bus modell.
Következő:SCI MMIO és IRQ közös címtérbe,CPU-buszidőbélyeg/periféria-határidők,
reset/izoláció és szétvágástól független RX/TX,SLEEP-ébresztés.
Teljes gép,GPIO/LCD/DSP/MEG,időzítés,firmware/MIDI/PKS/hang/plugin elfogadás nyitott.
Soros fejlesztés:egyszerre egy PR;az alábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #114, 2026-10-29

A [#114 PR](https://github.com/RobCZart82/yaman1x/pull/114) beolvadt:
merge `3172c55e9a1cea766e5326bfd56a98f68d6420f3`,tesztelt head `532c5937cfc2b0b3f142e935cebba8dfa4912000`.
Időpont:2026-10-29 22:45:44 Europe/Budapest (21:45:44 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229492), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229514), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40958229555):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **74 CTest-csoport /87 Python-eset;562 upstreamből származó kezelő
(558 változatlan,4 korábbi adaptáció);264 kiválasztott /0 hiányzó /135 kizárt sor**.
A leltározott H8H-kezelők kiválasztása teljes;ez nem teljes CPU/gép-elfogadás.
SLEEP és IRQ/NMI-ébresztés rögzített profillal,standby kizárva. Helyi Release74/74
CTest,87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,nulla eltérés;
ébresztést tiltó kontroll seed13/slice7-nél felismerve. 512 maszkkombináció és644
szétvágás,periféria-előrehaladás és határidő nélküli alvás ellenőrizve.
[Bizonyíték és korlátok](h8-sleep-wake.md).
Nincs aktív motorjelölt. Következő:AN1x címtér/MMIO és CPU-periféria közös gépütemező,
reset/izoláció/folytathatóság,utána eredeti helyi ROM indítási és MIDI/PKS megfigyelés.
M0 részleges;dinamikus SYSCR,standby,DMA/DTC,trace/retry,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nincs lezárva. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #112, 2026-10-29

A [#112 PR](https://github.com/RobCZart82/yaman1x/pull/112) beolvadt:
merge `b14cd16e8d4d149cdb622371ac29928904bbc295`,tesztelt head `41c5c34be49913dea5dc025f589cfde068a27736`.
Időpont:2026-10-29 22:05:04 Europe/Budapest (21:05:04 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371785), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371726), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40952371748):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **73 CTest-csoport /87 Python-eset;560 upstreamből származó kezelő
(556 változatlan,4 korábbi adaptáció);263 kiválasztott /1 hiányzó /135 kizárt sor**.
EEPMOV.B/W:bájtonkénti másolás8/16 bites számlálóval,két prefix,nyolc változatlan kezelő.
Helyi Release73/73 CTest,87 Python,célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás forrásléptetés kontrollja
seed15/slice4-nél felismerve. Max65535 darab,átfedés,címkörbefordulás,362 szétvágás,
131072 prefixőr és12 memóriahiba ellenőrizve. [Bizonyíték](h8-eepmov-block.md).
Nincs aktív motorjelölt. Következő:SLEEP megállás/ébresztés és periféria-előrehaladás
host/ütemező-szerződésével. M0 részleges;teljes CPU/gép/MMIO,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nyitott. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #110, 2026-10-29

A [#110 PR](https://github.com/RobCZart82/yaman1x/pull/110) beolvadt:
merge `3d0ce825bb341402f40dab92c0f1ec2f409b5ee7`,tesztelt head `903844cdb2a0466a93b2a61b056191afd0b37146`.
Időpont:2026-10-29 21:25:50 Europe/Budapest (20:25:50 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943842276), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943842076), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40943841959):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **72 CTest-csoport /87 Python-eset;552 upstreamből származó kezelő
(548 változatlan,4 korábbi adaptáció);261 kiválasztott /3 hiányzó /135 kizárt sor**.
MOVFPE/MOVTPE:két művelet,négy változatlan kezelő. Az eredeti `TODO: timing`
megmarad:periféria/E-clock kézfogás és hardveridőzítés nincs implementálva/igazolva.
Helyi végső Release72/72 CTest,87 Python és célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás cím-előjelkiterjesztés
kontrollja seed15/slice1-nél felismerve. A túl tág dekódolási maszkot és az
elsőutasítás-leltárt javítottuk,teljes újrafordítás/regresszió után PASS.
[Bizonyíték és korlátok](h8-peripheral-byte-moves.md).
Nincs aktív motorjelölt. Következő:EEPMOV blokkmozgatás,majd SLEEP a szükséges
host/ütemező-szerződéssel. M0 részleges;teljes gép/MMIO,hardveridőzítés,
firmware/MIDI/hang/plugin elfogadás nyitott. Soros fejlesztés:egyszerre egy PR.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #108, 2026-10-29

A [#108 PR](https://github.com/RobCZart82/yaman1x/pull/108) beolvadt:
merge `3c7005a3a26da4149668f6ab9f4695e1672bdc6e`,tesztelt head `ffef22c9d5794ec78028a0f2d07cb9659ff9d5bb`.
Időpont:2026-10-29 20:25:40 Europe/Budapest (19:25:40 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469453), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469399), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40937469419):mind az öt feladat sikeres.
Nincs rögzített automatikus review-eredmény vagy nyitott review-szál;
sikeres automatikus review-t nem állítunk.

Beolvadt: **71 CTest-csoport /87 Python-eset;548 upstreamből származó kezelő
(544 változatlan,4 korábbi adaptáció);259 kiválasztott /5 hiányzó /135 kizárt sor**.
CCR displacement32:két művelet,két eredeti prefixfolytatás,nyolc változatlan kezelő.
Helyi Release71/71 CTest,87 Python és célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;felső eltolásszó elhagyásának
kontrollja seed0/slice1-nél felismerve. [Bizonyíték](h8-ccr-displacement32-memory.md).
Nincs aktív motorjelölt. Következő lépés:MOVFPE/MOVTPE perifériás bájtmozgatások,
majd EEPMOV és SLEEP a szükséges host/ütemező-szerződések tisztázásával.
M0 részleges;teljes gép/MMIO,hardveridőzítés,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés:új lépcső csak az aktuális PR beolvadása után.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #106, 2026-10-29

A [#106 PR](https://github.com/RobCZart82/yaman1x/pull/106) beolvadt:
merge `aee769ef224e2686ccd4a98bbd9d3851f8cc6399`, tesztelt head `e3af0b4453d19194da8d32b24e6d409d4ab8809f`.
Időpont:2026-10-29 19:21:59 Europe/Budapest (18:21:59 UTC).
[Platform/UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577461), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577203), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40928577221): mind az öt feladat sikeres.
A jelenlegi GitHub-állapotban nincs rögzített automatikus review-eredmény vagy
review-szál; sikeres automatikus review-t nem állítunk.

Beolvadt: **70 CTest-csoport /87 Python-eset;540 upstreamből származó kezelő
(536 változatlan,4 korábbi adaptáció);257 kiválasztott /7 hiányzó /135 kizárt sor**.
CCR signed16 eltolás:két művelet,négy változatlan kezelő. Helyi Release70/70
CTest,87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,
nulla eltérés;hibás előjel-kiterjesztés kontrollja seed1/slice2-nél felismerve.
[Hatókör és bizonyíték](h8-ccr-displacement16-memory.md).
Nincs aktív motorjelölt. Következő lépés:CCR displacement32 címzések.
M0 részleges;teljes gép/MMIO,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés:új lépcső csak az aktuális PR beolvadása után.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #104, 2026-10-29

A [#104 PR](https://github.com/RobCZart82/yaman1x/pull/104) beolvadt:
merge `92e52f45307dc1adad37347febd407d0d2106182`,végső tesztelt head `570d7b11d5dde819ac12211671f39fd47163f81b`.
Időpont:2026-10-29 18:30:16 Europe/Budapest (17:30:16 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264372),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264370),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40919264281):mind az öt feladat sikeres.
Automatikus review a végső commiton hibamegállapítás nélkül befejeződött;
nyitott review-szál nincs.

Beolvadt: **69 CTest-csoport /87 Python-eset;536 upstreamből származó kezelő
(532 változatlan,4 korábbi adaptáció);255 kiválasztott /9 hiányzó /135 kizárt sor**.
CCR postincrement/predecrement,két művelet,négy változatlan eredeti kezelő.
524288 memóriaszó,24576 határeset,201 szétvágás,512 IRQ/NMI-maszk,
322 megszakításos szétvágás és16 memóriahiba PASS. A host-visszahívás a memóriaelérés
előtt megfigyeli a módosított mutatót és régi CCR-t. Helyi Release69/69 CTest,
87 Python és célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,nulla eltérés;
a hibás négybájtos növelés kontrollja seed2/slice3-nál felismerve.
[Hatókör és bizonyíték](h8-ccr-autoindex-memory.md).

Nincs aktív motorjelölt. Következő lépés:CCR signed16/displacement32 címzések,
majd a fennmaradó H8H-műveletek. M0 részleges;teljes gép/MMIO,firmware/MIDI/
hang/plugin elfogadás nyitott. Soros fejlesztés;egyszerre egy PR,új lépcső csak beolvadás után.
A #103 dokumentációs PR lezárását a strukturált nyilvántartás már tartalmazza.
Az alábbi régebbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #102, 2026-10-29

A [#102 PR](https://github.com/RobCZart82/yaman1x/pull/102) beolvadt:
merge `2dab39c1598533fc0a84da9017b3f2975854ff96`, végső tesztelt head `3c0f7947efc5cc170877fa99479f181d527bd195`.
Időpont:2026-10-29 17:50:13 Europe/Budapest (16:50:13 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848068),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848027),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40912848153): mind az öt feladat sikeres.
Automatikus review a végső commiton hibamegállapítás nélkül befejeződött;
nyitott review-szál nincs.

Beolvadt: **68 CTest-csoport /87 Python-eset;532 upstreamből származó kezelő
(528 változatlan,4 korábbi adaptáció);253 kiválasztott /11 hiányzó /135 kizárt sor**.
Négy CCR abs16/abs32 betöltési/tárolási művelet,nyolc változatlan eredeti kezelő.
131072 memóriaszó,131072 abs16 cím,524288 abs32 címfél-söprés,9216 CCR-határeset,
241 szétvágás,1024 IRQ/NMI-maszk és644 megszakításos szétvágás PASS.
Helyi Release68/68 CTest és87 Python,célzott ASan/UBSan PASS.
Eredeti referencia200000 szelet,nulla eltérés;hibás alsóbájt-kiválasztás
kontrollja seed3/slice5-nél felismerve. [Hatókör és bizonyíték](h8-ccr-absolute-memory.md).

Nincs aktív motorjelölt. Következő lépés:CCR postincrement/predecrement,
majd eltolásos címzések és a fennmaradó H8H-műveletek. M0 részleges;
teljes gép/MMIO,firmware/MIDI/hang/plugin elfogadás nyitott.
Soros fejlesztés;egyszerre egy PR,új lépcső csak beolvadás után.
A #101 dokumentációs PR lezárását a strukturált nyilvántartás már tartalmazza.
Az alábbi régebbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #100, 2026-10-29

A [#100 PR](https://github.com/RobCZart82/yaman1x/pull/100) beolvadt:
merge `1de8971b8fdd7694a8c3051ae86611a9ff6cd3cd`, végső tesztelt head `ba6db1c61a3ba9d696f8affee1410e5969c0aa9d`.
Időpont:2026-10-29 17:10:15 Europe/Budapest (16:10:15 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424971),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424908),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40906424743): mind az öt feladat sikeres.
Automatikus review a végső commiton befejeződött, hibamegállapítás és nyitott review-szál nélkül.

Beolvadt: **67 CTest-csoport /87 Python-eset;524 upstreamből származó kezelő
(520 változatlan,4 korábbi adaptáció);249 kiválasztott /15 hiányzó /135 kizárt sor**.
Két MOV.L művelet, eredeti kettős prefix és öt szavas utasításpuffer.
524288 eltolásfél-söprés,55296 határeset,241 szétvágás,65536 prefixfolytatás,
regiszteraliasok és első/második memóriaátviteli hibák PASS. Helyi Release67/67
CTest és87 Python, célzott ASan/UBSan PASS. Eredeti referencia200000 szelet,
nulla eltérés; a felső eltolásszó elhagyásának hibás kontrollja seed4/slice4-nél felismerve.
[Hatókör és bizonyíték](h8-long-displacement32-memory.md).

Nincs aktív motorjelölt. Következő lépés: a fennmaradó CCR-címzések és további
hiányzó H8H-kódolások, majd teljes gép/MMIO-integráció. M0 részleges;
firmware/MIDI/hang/plugin elfogadás nyitott. Egyszerre egy PR; új fejlesztés csak beolvadás után.
A #99 dokumentációs PR beolvadását a strukturált nyilvántartás már tartalmazza.
Az alábbi korábbi jelölt/pending állapotok történetiek.

## Történeti lezárás — #98, 2026-10-29

A [#98 PR](https://github.com/RobCZart82/yaman1x/pull/98) beolvadt:
merge `1c7ba628aceb29fd4c057db73fe31b2be4841558`, tesztelt head `a3c033256ec247555b44b0d820bb228dcaf9bc39`.
Időpont: 2026-10-29 13:36:16 Europe/Budapest (12:36:16 UTC).
[Linux/macOS/Windows és Linux UBSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870231488),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870229509),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40870229586): mind az öt feladat sikeres.
Automatikus review szolgáltatási kvóta miatt nem állt rendelkezésre; sikeres review-t nem állítunk.

Beolvadt: **66 CTest-csoport /87 Python-eset;518 upstreamből származó kezelő
(514 változatlan,4 korábbi adaptáció);247 kiválasztott /17 hiányzó /135 kizárt kódolássor**.
524288 memóriaszó,24576 cím/CCR-eset,65536 prefixfolytatás,201 szétvágás,
512 IRQ/NMI-maszk és322 megszakításos szétvágás; eredeti referencia200000 szelet,
nulla eltérés. A hibás alsóbájt-kiválasztást a megerősített referencia felismerte.
A helyi korábbi Windows-futás65 futtatható CTest/86 Python-esetet ellenőrzött;
az ismert symlink-jogosultsági eset kizárását nem tekintjük sikeres tesztnek.
[Pontos bizonyíték és korlátok](h8-ccr-indirect-memory.md). M0 részleges; nincs aktív
motorjelölt. Teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.

A fejlesztés soros: egyszerre egy PR; következő fejlesztés csak az aktuális PR
beolvadása után. Következő címzési lépés a hosszúszavas32 bites eltolás,
majd a fennmaradó CCR-címzések és a leltár további hiányzó műveletei.

## Történeti lezárás — #96, 2026-10-29

Ellenőrzött motoralap: [#96](https://github.com/RobCZart82/yaman1x/pull/96),
merge `8b1c37eb569202e394ff1caeb3f64618bae78a69`, végső tesztelt head `fd5d1293a21cf63c4b39293b30ca7e650702efe4`.
Beolvadt: 2026-10-29 11:49:02 Europe/Budapest (10:49:02 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583149),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583161),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40859583141).
Linux UBSan:32 bites eltolásos bájt/szó-mozgatás és korábbi számtan PASS; eredeti
unsigned-túlcsordulási kontroll PASS. Automatikus kódellenőrzés kvótakorlát miatt
nem fejeződött be; sikeres review-t nem állítunk. Minden CI sikeres volt.

Beolvadt: **65 CTest-csoport /87 Python-eset;512 upstreamből származó kezelő
(508 változatlan,4 korábbi adaptáció);245 kiválasztott /19 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. Négy MOV-csoport:1048576 eltolásfél-söprés (nem teljes2^32
keresztszorzat),55296 cím/regiszter/CCR-határeset,241 szétvágás,65536 prefixfolytatás,
memóriahibás fail-stop és újraindítás-tiltás PASS. Eredeti referencia:200000 szelet,
nulla eltérés; felső eltolásszó elhagyása kontroll seed6/slice5 felismerve. Helyi64
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
[Pontos hatókör és bizonyíték](h8-displacement32-memory.md). Hosszúszavas32 bites
eltolás külön nyitott. Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/
hang/plugin elfogadás nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #94, 2026-10-29

Ellenőrzött motoralap: [#94](https://github.com/RobCZart82/yaman1x/pull/94),
merge `442949856a92392986f3bbc5927f36b7884a505b`, végső tesztelt head `9f3cda7fcbe545ee2b0da9cebc23d8dd5ab6b42a`.
Beolvadt: 2026-10-29 10:48:25 Europe/Budapest (09:48:25 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898827),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898826),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40850898831).
Linux UBSan: szélső előjeles szó-osztás és korábbi számtan PASS; eredeti unsigned-
túlcsordulási kontroll PASS. Automatikus kódellenőrzés a beolvasztáskor még futott;
lezárt review-t nem állítunk. Minden CI sikeres volt.

Beolvadt: **64 CTest-csoport /87 Python-eset;502 upstreamből származó kezelő
(498 változatlan,4 korábbi adaptáció);241 kiválasztott /23 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. DIVXS.W:524288 osztó-söprés,21504 alias/CCR,20000 véletlen pár,
1024 szélső/CCR-eset,321 szétvágás,128 tiltott ER-szomszéd PASS. Eredeti referencia:
200000 szelet,nulla eltérés; fordított N kontroll seed7/slice7 felismerve. Helyi63
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
INT_MIN/-1: a matematikai +2147483648 hányadost az eredeti32 bites intre szűkíti;
N=1,csomagolt eredmény0,Z=0. Ez forráskompatibilitás,nem hardveres mérés.
A forrás szintű szélsőeset-vizsgálat lezárult;[pontos hatókör](h8-signed-word-divide.md).
Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás
nyitott. Az alábbi jelölt/pending és korábbi nyitott szélsőeset-bejegyzések történetiek.

## Történeti lezárás — #92, 2026-10-29

Ellenőrzött motoralap: [#92](https://github.com/RobCZart82/yaman1x/pull/92),
merge `1adeb1030871083bb510f4e691da4467cd96a816`, végső tesztelt head `b719cf25b0d0d8b929c5b639d5971ddc424a1a49`.
Beolvadt: 2026-10-29 10:18:06 Europe/Budapest (09:18:06 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532370),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532267),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40847532335).
Linux UBSan: előjeles bájtos osztás és korábbi számtan PASS; eredeti unsigned-
túlcsordulási kontroll PASS. Automatikus kódellenőrzés a végső headen megállapítás nélkül lezárult.

Beolvadt: **63 CTest-csoport /87 Python-eset;500 upstreamből származó kezelő
(496 változatlan,4 korábbi adaptáció);240 kiválasztott /24 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. DIVXS.B:393216 osztandó-söprés,2048 osztóhatáreset,36864 alias/CCR,
20000 véletlen pár,241 szétvágás,65536 prefixfolytatás PASS. Eredeti referencia:
200000 szelet,nulla eltérés; hányados+1 kontroll seed7/slice3 felismerve. Helyi62
futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
Előjeles szó-osztás INT32_MIN/-1 és csomagolás továbbra is nyitott vizsgálat;
[pontos hatókör és bizonyítékok](h8-signed-byte-divide.md). Nincs aktív motorjelölt.
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #90, 2026-10-29

Ellenőrzött motoralap: [#90](https://github.com/RobCZart82/yaman1x/pull/90),
merge `a4f52f600d2cfd85f14a5e8822eb5e025d28825d`, végső tesztelt head `f24ca47f3511764dfec4971e84c3c1487aaed443`.
Beolvadt: 2026-10-29 09:38:37 Europe/Budapest (08:38:37 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792043),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792142),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40842792129).
Linux UBSan: DAA/DAS és korábbi számtan PASS; eredeti unsigned-túlcsordulási kontroll
PASS. Automatikus kódellenőrzés a végső headen lezárult, megállapítás nélkül.

Beolvadt: **62 CTest-csoport /87 Python-eset;496 upstreamből származó kezelő
(492 változatlan,4 korábbi adaptáció);239 kiválasztott /25 hiányzó /135 kizárt sor**.
Ez nem CPU-százalék. Decimális oracle:40000 érvényes számpár/CCR-eset,2048 regiszter-
határeset,241 szétvágás PASS;200000 eredeti referenciaszelet nulla eltéréssel.
DAA korrekció+1 kontroll seed7/slice0 felismerve. Helyi61 futtatható CTest/86 Python
PASS, ismert Windows-symlink eset kizárva. Érvénytelen BCD/flag kombinációkra nincs
állított teljes független helyességbizonyítás. [Hatókör](h8-decimal-adjust.md).
Nincs aktív motorjelölt. Ez M0-részlépés; M0 továbbra is részleges, teljes gép,
firmware/MIDI/hang/plugin elfogadás nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #88, 2026-10-29

Ellenőrzött motoralap: [#88](https://github.com/RobCZart82/yaman1x/pull/88),
merge `3c5f7dddfdf54532670d9cdddd0ade0aab7c92bd`, végső tesztelt head `fc0db2778270a5ffdc612ab46ccd7be0b0d9b4fe`.
Beolvadt: 2026-10-29 08:55:24 Europe/Budapest (07:55:24 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738076),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738102),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40837738069).
Linux UBSan: bájt/szó-osztás és korábbi szorzások PASS; eredeti unsigned-túlcsordulási
kontroll PASS. Beolvadt: **61 CTest-csoport /87 Python-eset;492 upstreamből származó
kezelő (488 változatlan,4 dokumentált adaptáció);237 kiválasztott /27 hiányzó /135 kizárt
kódolássor** (nem CPU-százalék). DIVXU.W:524288 osztó-söprés,30720 alias/CCR-eset,
20000 véletlen pár,241 szétvágás PASS. Nagy értékek független64 bites számítással;
200000 korlátozott eredeti referenciaszelet nulla eltéréssel; hányados+1 kontroll
seed9/slice26 felismerve. Helyi60 futtatható CTest/86 Python PASS, ismert Windows-symlink
eset kizárva. A q/r u32 típusmódosítás dokumentált; az eredeti osztóbit7 N-szabály
megőrzött, hardveresen nem igazolt. [Pontos hatókör](h8-word-divide.md).
Nincs aktív motorjelölt. M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás
nyitott. Az alábbi jelölt/pending bejegyzések történetiek.

## Történeti lezárás — #86, 2026-10-29

Ellenőrzött motoralap: [#86](https://github.com/RobCZart82/yaman1x/pull/86),
merge `4dd788dadae4b09a21b4611f245640a63c56dcf5`, végső tesztelt head `189eeea02372839e8cf27f02853b990ed0625652`.
Beolvadt: 2026-10-29 08:26:31 Europe/Budapest (07:26:31 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892241),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892230),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40834892231).
Linux UBSan: bájtos unsigned osztás és korábbi szorzások PASS; az eredeti unsigned
int-promóciós hibát megkövetelő kontroll is PASS.

Beolvadt: **60 CTest-csoport /87 Python-eset;490 upstreamből származó kezelő
(488 változatlan,2 korábbi adaptáció);236 kiválasztott /28 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. DIVXU.B:393216 osztandó-söprés,
2048 osztóhatáreset,30720 alias/CCR-eset,20000 véletlen pár,241 szétvágás PASS.
Eredeti referencia:200000 szelet,nulla eltérés; hányados+1 kontroll seed9/slice18
felismerve. Helyi59 futtatható CTest/86 Python PASS, ismert Windows-symlink eset kizárva.
A nullás osztó, hányadoslevágás és osztófüggő N/Z az eredeti forrással egyezik,
nem külön hardvermérés. Nincs aktív motorjelölt. [Hatókör és bizonyíték](h8-byte-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/pending bejegyzések történetiek, nem a jelenlegi állapotot jelzik.

## Történeti lezárás — #84, 2026-10-29

Ellenőrzött motoralap: [#84](https://github.com/RobCZart82/yaman1x/pull/84),
merge `2540601aa985513a39d293ffa14fd8e0b76689da`, végső tesztelt head `7aee43a86ea21711254fd3e840728534e01b6a96`.
Beolvadt: 2026-10-29 07:45:33 Europe/Budapest (06:45:33 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383282),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383154),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40830383285).
Linux UBSan: előjeles bájt/szó és unsigned szó-szorzás PASS; az eredeti unsigned
int-promóciós hibát megkövetelő negatív kontroll is PASS.

Beolvadt: **59 CTest-csoport /87 Python-eset;488 upstreamből származó kezelő
(486 változatlan,2 korábbi adaptáció);235 kiválasztott /29 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. Előjeles szorzás:65536 bájtpár,
327680 szó-söprés,28800 alias/CCR-határeset,20000 véletlen pár,241 szétvágás és
65536 prefixfolytatás PASS. Eredeti referencia:200000 szelet,nulla eltérés;
product+1 kontroll seed9/slice14 felismerve. Helyi58 futtatható CTest/86 Python PASS;
egy ismert Windows-symlink eset kizárva. Nincs aktív motorjelölt.
[Szerződés és pontos bizonyítékok](h8-signed-multiply.md). M0 részleges;
teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Az alábbi jelölt/pending
bejegyzések történetiek, nem a jelenlegi állapotot jelzik.

## Történeti lezárás — #82, 2026-10-29

Ellenőrzött motoralap: [#82](https://github.com/RobCZart82/yaman1x/pull/82),
merge `80b237ee7a9ad6b286a13c9253d076aa42bb0214`, végső tesztelt head `4f2cffc88c843cc9d431f5c0a6ec20ee2869d85b`.
Beolvadt: 2026-10-29 07:20:23 Europe/Budapest (06:20:23 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962559),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962568),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40827962569).
A Linux-lépésben a nagy szorzatok UBSan-tesztje és az eredeti kifejezés
előjeles túlcsordulását megkövetelő külön negatív kontroll is PASS.

Beolvadt: **58 CTest-csoport /87 Python-eset;482 upstreamből származó kezelő
(480 változatlan,2 dokumentáltan adaptált);233 kiválasztott /31 hiányzó /135 kizárt
profilú kódolássor**. Ez nem CPU-százalék. MULXU.W:327680 operandus-söprés,
9600 alias/CCR-határeset,20000 véletlen pár,241 szétvágás PASS;200000 biztonságos
szorzatokkal végzett eredeti referenciaszelet nulla eltéréssel. A nagy szorzatokat
független64 bites számítás ellenőrzi; product+1 kontroll seed9/slice9 felismerve.
Helyi57 futtatható CTest és86 Python PASS, egy ismert Windows-symlink eset kizárva.
A szó-szorzás C++-promóciós kockázata rendezett a dokumentált unsigned-szélesítéssel;
[pontos hatókör és bizonyíték](h8-word-multiply.md). Nincs aktív motorjelölt.
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott.
Az alábbi jelölt/CI-pending/kockázati bejegyzések történetiek.

## Történeti lezárás — #80, 2026-10-29

Ellenőrzött motoralap: [#80](https://github.com/RobCZart82/yaman1x/pull/80),
merge `bddffa8743eac2e2af8e7fc1a3145046a45e6fa5`, végső tesztelt head `5bf4e97e64fc3ebbe00a42f8215a740fbca72d86`.
Beolvadt: 2026-10-29 06:41:58 Europe/Budapest (05:41:58 UTC).
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422195),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422322),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40824422266).

Beolvadt: **57 CTest-csoport / 87 Python-eset;480 eredeti kezelő;
232 kiválasztott /32 hiányzó /135 kizárt profilú kódolássor** (nem CPU-százalék).
MULXU.B:65536 operanduspár,19200 alias/CCR-határeset,161 szétvágott futás PASS;
200000 referenciaszelet nulla eltéréssel; product+1 negatív kontroll felismerve.
Helyi56 futtatható CTest és86 Python PASS; egy ismert Windows-symlink eset kizárva.
Nincs aktív motorjelölt. A szó-szorzás int-promóciós kockázata külön nyitott;
[bekötési feltételek és bizonyítékok](h8-byte-multiply.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás továbbra is nyitott.
Az alábbi jelölt/CI-pending bejegyzések történeti előkészítési állapotok.

## Történeti lezárás — #78, 2026-10-29

Ellenőrzött motoralap: [#78](https://github.com/RobCZart82/yaman1x/pull/78),
merge `d4ef0e45bc416f06216d43559e1f73df1d39caa8`, végső tesztelt head
`ef6adefadb5b390f6822ae34c4db41edab5b1348`. Beolvadt: 2026-10-29 06:26:01 Europe/Budapest
(2026-10-29 05:26:01 UTC). [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703637),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703598),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40822703615).

Beolvadt csomag: **56 CTest-csoport, 87 Python-eset; 478 eredeti kezelő;
231 kiválasztott / 33 hiányzó / 135 kizárt profilú kódolássor**. Ez nem CPU-százalék.
Hat előjeles 16 bites eltolásos memóriautasítás-csoport készült el. Független
referencia: 200000 szelet, nulla eltérés; a szándékos előjelhiba felismerve.
Helyi Windows: 55 futtatható CTest és 86 Python PASS; egy ismert symlink-jogosultsági
eset kizárva. A CI a teljes csomagot ellenőrizte. M0 részleges; teljes AN1x-gép,
firmware-es MIDI, hang és plugin elfogadása nyitott. Nincs aktív motorjelölt.
A korábbi jelölt/CI-pending bejegyzések történetiek; lezárásuk itt és a JSON-naplóban szerepel.

## Történeti lezárás — #77, 2026-10-29

Ellenőrzött motoralap: [#77](https://github.com/RobCZart82/yaman1x/pull/77),
merge `8993a6024b0cedfe0d3f5c48a2819009c98643f1`, végső tesztelt head
`fb2bc0935aad976706b0652d21275e5a55d0efd8`. Beolvadt: 2026-10-29 00:49:52 Europe/Budapest
(2026-10-28 23:49:52 UTC). [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788596091),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788595788),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40788595917).

Beolvadt csomag: **55 CTest-csoport, 87 Python-eset; 466 eredeti kezelő;
225 kiválasztott / 39 hiányzó / 135 kizárt profilú kódolássor**. Ez nem CPU-százalék.
A fix profilú IRQ/NMI/TRAPA és a szintetikus CPU–SCI RX/TX/hibakezelés elkészült;
a teljes AN1x-gép, firmware-es MIDI, hang és plugin elfogadása nyitott. M0 részleges.
Az alábbi korábbi jelölt/CI-pending bejegyzések történetiek. Az új eltolásos
memóriautasítások külön jelöltet alkotnak, nem írják felül ezt a lezárt motoralapot.

## Történeti lezárás — #57, 2026-10-28

Ellenőrzött motoralap: [PR #57](https://github.com/RobCZart82/yaman1x/pull/57), merge
`f8b6aa20834f209931e3e9bb9463cc47b5b7a540`. Végső tesztelt head:
`bf3e7158a9a4dac6edb36ea4deb659bdf3c54cb1`.
[Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365642),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365647),
[MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40685365644).
A beolvadt csomag **39 CTest-csoportot és 84 Python-esetet** tartalmaz.
Helyi Windows-ellenőrzés: 38 futtatható CTest-csoport és 83 Python-eset PASS;
egy symlink-jogosultsági eset kizárva (WinError 1314). A CI a teljes csomagot futtatta.

A normál, rögzített H8H-profil verem-/vezérlésátadási családjai elkészültek;
366 eredeti full/partial kezelő szerepel a részhalmazban. Az RTE szintetikus
veremkép-visszaállítása tesztelt, az IRQ/TRAPA-belépés még nincs bekötve.
M0 részben teljesült; M1 nincs elfogadva; M2–M6 nyitott. Nincs igazolt AN1x-hang
vagy REAPER-plugin. A következő motorfeladatok: hiányzó H8H-utasítások/címzések
leltára, IRQ/TRAPA-belépés és szűrők, majd közös CPU/periféria-ütemezés és gépintegráció.

A korábbi „aktív”, „jelölt”, „CI pending” és „következő” bejegyzések az akkori
állapotot őrzik; a jelenlegi készültségre a fenti #116 lezárás és az aktuális mérföldkő-tábla
irányadó. A dokumentáció mostani javítása nem új motorfejlesztés vagy elfogadás.

## Aktuális mérföldkő-állapot — 2026-10-29

| Mérföldkő | Aktuális állapot | Következő elfogadási feltétel |
|---|---|---|
| M0 | **Részben teljesült.** Architektúra és forrás-/licencnyilvántartás; tesztelt GPIO-, IRQ- és SCI-komponensek; működő önálló H8H-részhalmaz 562 upstreamből származó kezelővel (558 változatlan,4 adaptált), rögzített SYSCR-es IRQ/NMI/TRAPA-belépéssel, buszidőbélyeggel és szintetikus CPU–SCI RX/TX/hibakezeléssel. | A264 leltározott H8H-kódolássor kezelője kiválasztva; a tesztelt CPU/periféria-ütemezés teljes AN1x címtér/MMIO-gépbe integrálása és gépizoláció. A részhalmaz már fordul és fut; a teljes CPU és a terjesztési licenc lezárása nyitott. |
| M1 | **Előkészítve, nem teljesült.** A külön macOS MAME-build és a párosított helyi futtató rendelkezésre áll. | Friss Mac-firmware-próba: SCI0 tényleges bájtfogadás és note-on/note-off feldolgozás bizonyítása; PKS-kézfogás feltárása. |
| M2–M3 | **Kutatási feladatok nyitva.** Nincs igazolt referenciahang. | AN1x órajel, táblacímzés, EG és soros hangút bizonyítása, majd megismételhető offline hang. |
| M4–M6 | **Tervezett.** Nincs elkészült beágyazható hangszer vagy REAPER-elfogadás. | Validált offline referencia után audio-blokkmotor, VST3 és helyi REAPER-próba. |

Az önálló komponens tesztje nem teljes CPU-/firmware-elfogadás. A jelenlegi
állapotot ez a táblázat és az M0/M1 részletes szakasza mutatja. A lezárási tábla
az ellenőrzött végső PR-revíziókat és CI-futásokat rögzíti; a korabeli napló
az akkori munkamenet állapotát őrzi.

Korábbi, #36-hoz tartozó platformeredmény: [#36](https://github.com/RobCZart82/yaman1x/pull/36),
`6f3fbf6c22b9f3bfc28f78d0fc1bad55ed98ed8c`: Linux/macOS/Windows platformonként
**22/22 tesztcsoport PASS**, MSan és teljes macOS MAME-build PASS. A konkrét
futások a lezárási táblában vannak. Helyben Windows alatt 21 futtatható csoport
PASS; a Python unittest-csoportot a korábban igazolt symlink-jogosultsági korlát
miatt kizártuk a helyi futásból. A teljes csoport CI-ben sikeres. Ez nem teljes
helyi tesztcsomag-PASS, és nem firmware-es MIDI-elfogadás.

## Ellenőrzött lezárások — #39–#44

Ellenőrizve: 2026-10-27. Mind a hat PR beolvadt; mindegyik végső headjén öt ellenőrzés PASS. Az alábbi lezárás az irányadó a korábbi, készítéskori „CI pending / review alatt” megjegyzésekkel szemben. Azok történeti pillanatképek, nem aktuális akadályok. A PR-head CI nem hardver-/firmware-/audioelfogadás.

| PR | Ellenőrzött head | Linux/macOS/Windows | MSan | MAME |
|---|---|---|---|---|
| [#39](https://github.com/RobCZart82/yaman1x/pull/39) | `9459a805af0834c5ad66329f0f1b037a9354b08d` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292566377) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292566528) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40533549011/job/122292565993) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549567/job/122292568462) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40533549032/job/122370894035) |
| [#40](https://github.com/RobCZart82/yaman1x/pull/40) | `34cec0074577a1add862772bcf3bc289de735b68` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453074) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453726) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40557237913/job/122376453515) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40557238091/job/122376454019) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40557237934/job/122376453689) |
| [#41](https://github.com/RobCZart82/yaman1x/pull/41) | `387c5e64c0ee9759b3d41dfa58bcda1b35594731` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943734) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943358) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40566011632/job/122407943754) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40566011629/job/122407456482) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40566012882/job/122408084911) |
| [#42](https://github.com/RobCZart82/yaman1x/pull/42) | `a67b599f880f0981d68e04b276b0d904ea3f7d29` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580997) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580920) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40573072777/job/122432580479) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072700/job/122432444681) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40573072611/job/122432149653) |
| [#43](https://github.com/RobCZart82/yaman1x/pull/43) | `83caf332df3539037187888c60bc35ae422dc2a3` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457473798) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457474477) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40580107525/job/122457474285) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107561/job/122456981421) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40580107642/job/122457680585) |
| [#44](https://github.com/RobCZart82/yaman1x/pull/44) | `1bed694bdc066a66bcebb747ed74a052a4650c25` | [Linux](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074548) / [macOS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074297) / [Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40586879805/job/122481074723) PASS | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879809/job/122480613181) | [PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40586879880/job/122480584462) |


## A terv folyamatos vezetése

Ez a dokumentum a fejlesztés során folyamatosan karbantartandó terv.
Minden, a tervet érintő fejlesztési PR ugyanabban a változtatásban frissítse
az érintett mérföldkövet és az alábbi előrehaladási naplót. Rögzíteni kell:

- a dátumot, a konkrét eredményt és a kapcsolódó PR-t vagy commitot;
- az állapotot: tervezett, folyamatban, részben teljesült vagy igazoltan kész;
- a végrehajtott ellenőrzéseket és eredményüket, a platformot és a vizsgált
  revíziót; a nem futtatott és környezeti okból akadályozott próbákat külön;
- a megmaradó feltételeket, függőségeket és a következő konkrét lépést.

A mérföldkő csak a saját elfogadási feltételeinek bizonyítása után jelölhető
késznek. A ROM-mentes CI sikere nem helyettesíti a hardver-, firmware-, audio-
vagy REAPER-próbát. Tervváltozásnál az indokot és az új sorrendet is rögzíteni
kell. A történeti eredményeket megőrizzük dátumozott bizonyítékként;
az aktuális állapotot az új bejegyzés és az érintett mérföldkő együtt mutatja.

A PR készítésekor a végső CI még nem ismert. Beolvasztás után a következő
nyilvántartási frissítésben kötelező a merge SHA, végső head és tényleges CI-eredmény
lezárása; az előző jelölt nem maradhat aktuális aktív fejlesztésként. A strukturált
nyilvántartás motoralapja az utolsó ellenőrzött motorcommit, nem szükségszerűen a
legújabb dokumentációs commit; `active_change: null` azt jelenti, nincs nyilvántartott
nyitott motorjelölt. A dokumentációs PR saját CI-jét a PR lezárása rögzíti.

ROM, bank és privát trace helyett csak publikálható azonosító és összefoglaló
kerülhet a tervbe. A részletes mérési jegyzőkönyvekre hivatkozás mutasson.

Az új szerkezet vezetési szabálya: minden új fejlesztésnél az aktuális
állapottáblát és az érintett részletes mérföldkövet együtt kell frissíteni.
A munkanapló új sora kapjon dátumot, PR-hivatkozást és „Korabeli állapot” jelölést.
A végső ellenőrzések lezárásakor a PR-lezárási táblába a valóban tesztelt
PR-revízió és annak saját futásai kerüljenek; más revízió CI-je nem helyettesíti.
A korabeli mérési számokat és korlátokat megőrizzük, a már teljesült előfeltételt
pedig az aktuális feladatlistából átvezetjük az elkészült eredmények közé.

## Előrehaladási napló

### Repository-vizsgálat és központi változásjegyzék — 2026-10-27

Az `AUD-2026-10-27` vizsgálat a #37 merge állapotáról indult. Elkészült a
[repository-audit](REPOSITORY_AUDIT_2026-10-27.md), a PR-eket indexelő
[fejlesztési napló](DEVELOPMENT_LOG.md) és a hozzá tartozó strukturált leltár.
A korabeli bejegyzések és a fenti végső PR-ellenőrzési táblázat megmaradnak.
Javítás készült a bridge-auditor mai register-only fejlécének kezelésére;
ez nem változtatja a DSP-modellt vagy M0/M1 készültségét. A vizsgálat és a
vezetési szabály a naplóban szerepel, a jelölt végső ellenőrzései a saját PR-ben.

### Dokumentációs egységesítés — 2026-10-27

[#37](https://github.com/RobCZart82/yaman1x/pull/37): az M0/M1 részletes állapota,
a lezárt PR-ek bizonyítékai és a korabeli pillanatképek értelmezése egységesítve.
Az ellenőrzés élő GitHub PR-/workflow-/jobadatokra és a fenti kódbázisra épült.
A módosítás csak dokumentáció; M0/M1 készültségét és az elfogadási feltételeket
nem változtatja meg. A dokumentációs PR saját CI-je a saját checkjeinél követhető.

### Ellenőrzött PR-lezárások — legutóbbi elöl

A #16–#36 PR-ek mind **beolvadtak**. Az alábbi SHA a végső tesztelt PR-head,
nem a squash után keletkezett `main` commit; a beolvasztás a PR hivatkozásánál
ellenőrizhető. Minden felsorolt alap-CI mindhárom platformjobja sikeres.
`—`: az adott PR végső revízióján nem volt ilyen PR-triggerelt workflow-futás;
nem FAIL és nem más revízió eredményének átvétele. A korábbi hibakereső vagy
köztes reprodukciós revíziók a korabeli naplóban megmaradnak.

| Beolvadt PR / eredmény | Végső tesztelt PR-revízió | Lezárt ROM-mentes ellenőrzések |
|---|---|---|
| [#36](https://github.com/RobCZart82/yaman1x/pull/36) — Közös aszinkron SCI-csatorna | `6f3fbf6c22b9f3bfc28f78d0fc1bad55ed98ed8c` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40500706565); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40500706683); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40500706500) |
| [#35](https://github.com/RobCZart82/yaman1x/pull/35) — Aszinkron RX | `eeceb8282e920e163c5ff649068b90952d753671` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40494506274); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40494506107); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40494506093) |
| [#34](https://github.com/RobCZart82/yaman1x/pull/34) — Aszinkron TX | `da5fcd3dd9301f06e68c489c879989661a3b3428` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40491811757); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40491811799); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40491811994) |
| [#33](https://github.com/RobCZart82/yaman1x/pull/33) — LICENSE szövegtördelése; státusz változatlan | `22cb57c8f1a617b6a9ddca90bad6fc8962671327` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40489564815); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40489564814); MAME: — |
| [#32](https://github.com/RobCZart82/yaman1x/pull/32) — Licenclefedettség-ellenőrzés | `fc6f4ed47774ba2b1a17394255a1427e2e7249fb` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40488326442); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40488326433); MAME: — |
| [#31](https://github.com/RobCZart82/yaman1x/pull/31) — Cserélhető PKS-megvalósítás követelménye | `41ff1d08c834632417efb4c6bbdca2a5c3754c24` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484817843); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484817846); MAME: — |
| [#30](https://github.com/RobCZart82/yaman1x/pull/30) — PKS ötletfeljegyzés | `955dd73afd3760df1535de60a94503c016161925` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484348640); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40484348645); MAME: — |
| [#29](https://github.com/RobCZart82/yaman1x/pull/29) — SCI SSR-latch | `b169bbe9b3e995328b75d6190f7458138c154722` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40482540061); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40482540084); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40482540083) |
| [#28](https://github.com/RobCZart82/yaman1x/pull/28) — SCI ciklusóra | `747cd98fac4ae300216f0bd5021dbd0cc2eae6ea` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40477835302); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40477835203); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40477835254) |
| [#27](https://github.com/RobCZart82/yaman1x/pull/27) — IRQ-regiszter/láb adapter | `d33a27bdef963ddc8b1385d87fa324878af51f89` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40474786571); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40474786580); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40474786569) |
| [#26](https://github.com/RobCZart82/yaman1x/pull/26) — Belső IRQ-választó | `d5d80e49f4664ce00f5df33d2d3408e74c2aa64a` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40470767512); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40470767565); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40470767326) |
| [#25](https://github.com/RobCZart82/yaman1x/pull/25) — H8 CI-bizonyíték dokumentálása | `a917c9779f64ce5afacae5fa2fc615d194c8abaa` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40469158573); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40469158626); MAME: — |
| [#24](https://github.com/RobCZart82/yaman1x/pull/24) — H8-leltár, generátor és GPIO | `896bd7882309bb03868c5c3816f4f9b7b068b858` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40468100477); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40468100462); [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40468100442) |
| [#23](https://github.com/RobCZart82/yaman1x/pull/23) — Mac párosított próba előkészítése | `e0ad4df4884af8d0926cd68e451ebcbc280ed4d0` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40463469779); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40463469732); MAME: — |
| [#22](https://github.com/RobCZart82/yaman1x/pull/22) — VOP3 konstruktorjavítás | `d363d6a530f03aef5e6167759357874d7b4650b0` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40461780030); [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40461779887); MAME: — |
| [#21](https://github.com/RobCZart82/yaman1x/pull/21) — Kutatási buildbizonyíték dokumentálása | `cfb87b521f601daff01c15befb09af88066239e4` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40459057121); MSan: —; MAME: — |
| [#20](https://github.com/RobCZart82/yaman1x/pull/20) — Rögzített kutatási forrás és build | `31664c74f2726091eb9f403c1621ef949479ddbb` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40457998062); MSan: —; [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40457998036) |
| [#19](https://github.com/RobCZart82/yaman1x/pull/19) — SCI-hozzáférés-megfigyelő | `748d8495b1e75ed43bd69d456bfd519ffca85557` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40455618012); MSan: —; MAME: — |
| [#18](https://github.com/RobCZart82/yaman1x/pull/18) — Tervvezetési szabályok | `e503321ad31b2d0a36e0ae419215034cbf82b74f` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40454887762); MSan: —; MAME: — |
| [#17](https://github.com/RobCZart82/yaman1x/pull/17) — ADR és függőségalap | `bd2262fd7c06c91f7e40ea2aaa3ea52dceecedc1` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40454539768); MSan: —; MAME: — |
| [#16](https://github.com/RobCZart82/yaman1x/pull/16) — Python -O / AsmJit javítások | `ac56d8c7144ef8cb478f3a714578203c440b841e` | [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40454166792); MSan: —; MAME: — |

A zöld CI a forrás-/komponenstesztek és a jelzett kutatási build eredménye.
Nem igazol teljes H8-portot, friss firmware-es MIDI-vételt, hardverazonosságot,
audio- vagy REAPER-elfogadást, és nem jelent jogi/licenc- vagy bináris kiadási
jóváhagyást. A csak dokumentációs PR-ek sem új működési bizonyítékok.

### Korabeli munkanapló — PR-sorrendben

Az alábbi sorok az adott fejlesztési lépés **akkori** állapotát és akkori
következő feladatát rögzítik. A `NOT RUN`, „PR megnyitva”, „platform-CI a PR-ben”
és a még külön TX/RX modellekre utaló mondatok itt történeti állapotok.
Platformlezáráshoz a fenti táblát, jelenlegi feladathoz az aktuális M0/M1 részt
kell olvasni. A firmware-/hardver-/audio-elfogadás hiányát nem írja felül az,
hogy egy korábban még nem futott ROM-mentes platformteszt azóta sikeres lett.
A helyi szintetikus és upstream-összehasonlító mérések eredeti számai megmaradnak.

| Dátum | Korabeli változás és állapot | Akkori ellenőrzés / bizonyíték | Akkori következő lépés |
|---|---|---|---|
| 2026-10-27 | **Korabeli állapot:** [#16](https://github.com/RobCZart82/yaman1x/pull/16), beolvadt: Python `-O` mellett is aktív ABI-ellenőrzések, AsmJit tesztforrások kizárása, harmadik fél patch-darabszám javítása | `ac56d8c7144ef8cb478f3a714578203c440b841e`: Linux x64, macOS arm64, Windows x64 CI 11/11 tesztcsoport PASS; [futás](https://github.com/RobCZart82/yaman1x/actions/runs/40454166792). Helyi Windows: build PASS, CTest 10/11; egy meglévő symlinkteszt WinError 1314 miatt akadályozott. [Auditkövetés](AUDIT_FOLLOWUP_2026-10-27.md). | A VOP3 inicializálás/`memcmp` kockázat célzott vizsgálata; EG target-hold változtatás csak hardveres bizonyíték után. |
| 2026-10-27 | **Korabeli állapot:** [#17](https://github.com/RobCZart82/yaman1x/pull/17), beolvadt: [ADR-0001](ADR-0001-engine-boundary.md), függőségazonosítók és [buildalap](BUILD_BASELINE.md). **M0 részben teljesült.** | `bd2262fd7c06c91f7e40ea2aaa3ea52dceecedc1`: mindhárom platform CI PASS; [futás](https://github.com/RobCZart82/yaman1x/actions/runs/40454539768). Helyi CMake-konfiguráció és forrásaudit PASS, 201 változatlan upstream fájl ellenőrizve. | H8-port függőség- és licenclezárása, példányizolációs/időléptetési próbák, teljes referencia-toolchain rögzítése; saját terjesztési licenc döntése. M1 a külön MAME-kutatási úton folytatható. |
| 2026-10-27 | **Korabeli állapot:** [#19](https://github.com/RobCZart82/yaman1x/pull/19): **M1 folyamatban:** időbélyeges SCI-hozzáférés-megfigyelő és naplóellenőrző elkészült; firmware-es elfogadás még nem futott. [Részletek](m1-sci-access-milestone.md). | Natív megfigyelő és célzott Python-tesztek Windows x64-en PASS. A teljes Python-csomag meglévő symlinktesztjét WinError 1314 akadályozza. Tizenegy patch alkalmazása a rögzített forrásra PASS; teljes új MAME-build és friss firmware-próba NOT RUN. | Külön macOS kutatási build, SCI-beállítások és rövid engedélyezési ablakok rögzítése, majd a PC-k követése privát disassemblyvel. PKS-válasz továbbra sem feltételezhető. |
| 2026-10-27 | **Korabeli állapot:** [#20](https://github.com/RobCZart82/yaman1x/pull/20), beolvadt: **M0/M1 folyamatban.** Upstream forrásazonosítók visszanyerése, ismételhető patch-előellenőrző és teljes macOS MAME-build CI. [Bizonyíték és határok](source-identity-recovery.md#verified-result). | `31664c74f2726091eb9f403c1621ef949479ddbb`: [Linux/macOS/Windows alapcsomag](https://github.com/RobCZart82/yaman1x/actions/runs/40457998062) 12/12 csoport PASS platformonként; [teljes MAME-build és arm64/regisztrációs próba](https://github.com/RobCZart82/yaman1x/actions/runs/40457998036) PASS Xcode 16.2-vel. 11 patch, 4 egyező projektfejléc; firmware/SCI/MIDI-elfogadás NOT RUN. A kezdeti fordító- és Windows-sorvéghiba dokumentálva. | Friss, helyi firmware-felvétel és SCI-vezérlésiút-elemzés. A teljes kutatási build előfeltétele teljesült; H8-port és példányizoláció továbbra is nyitott. |
| 2026-10-27 | **Korabeli állapot:** [#22](https://github.com/RobCZart82/yaman1x/pull/22): **M0 alapstabilitás, bizonyított konstruktorhiba javítása.** VOP3 `ops` inicializálása az első dekódolás előtt; [bizonyíték és visszafordítható provenance](vop3-initial-decode-fix.md). | `44d5e8c8d2708e6736ce88e8cbe48be67a7b314c`: az eredeti fejléc inicializálatlan `memcmp` olvasását a [MemorySanitizer reprodukálta](https://github.com/RobCZart82/yaman1x/actions/runs/40461084899), az inicializált jelölt tiszta. Natív konstrukció/dekódolás és provenance-regressziók hozzáadva; végső platform- és sanitizer-eredmények a PR checkjeinél. | H8-port és friss helyi firmware/SCI/MIDI-elfogadás továbbra is nyitott. Az EG target-hold változatlan, hardveres bizonyíték szükséges. |
| 2026-10-27 | **Korabeli állapot:** [#23](https://github.com/RobCZart82/yaman1x/pull/23): **M1 Mac-folytatás előkészítve.** Generált két másodperces MIDI-hang és két friss, egyetlen státuszprofilban eltérő firmware-megfigyelés futtatója; [Mac M1 átadás](MAC_M1_HANDOFF.md). | Szintetikus MIDI-időzítés, párosítási és hibakezelési próbák helyben PASS; forrásaudit PASS. A végső platform- és sanitizer-eredmények a PR checkjeinél. Valódi firmware/SCI/MIDI-futás NOT RUN. | A Macen eszköz- és forrásellenőrzés, build, majd a helyi párosított próba. A befejezett suite nem jelent automatikusan igazolt MIDI-feldolgozást vagy bootot. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): **M0 H8-port előkészítés:** 30 fájlos, hash-elt include/implementáció leltár és generált/keretrendszer-határok. [Részletek](h8-port-source-boundary.md). | A rögzített archívum read-only feldolgozása PASS; ciklus-, párosítás-, hiányzó forrás-, hash- és útvonal-regressziók PASS. Ez nem teljes fordítói függőségzárás vagy licenclezárás; beágyazott H8-build NOT RUN. | Generátor/lista/buildszabály rögzítése, framework-szolgáltatások feltárása és minimális hostadapter; a Macen külön friss firmware-próba. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): **M0 H8-generátor előkészítés:** három buildbemenet és négy generált fájl hash-rögzítése; [ismételt generálási próba](h8-port-source-boundary.md#repeated-generation-experiment). | Windows Python 3.13.3: négy kimenet két független generálása PASS; bemenetazonosság- és kimenettartalom-regressziók PASS; forrásaudit PASS. Ág: `feat/h8-source-inventory`; PR megnyitva; végső platform-CI a #24 checkjeinél. | Keretrendszer-szolgáltatások és generált kód függőségeinek feltárása, minimális hostadapter és fordítás. CPU/firmware-végrehajtás nem történt. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): **M0 első önálló H8-periféria:** MAME-független digitális portmodell; [forrásazonosság, határok és teszt](h8-digital-port.md). | MSVC 19.44 Release build és natív teszt PASS: 786432 DDR/adat/maszk/forced-input kombináció négy bemeneti mintával; értesítéssorrend, reset és példányizoláció PASS. Platform-CI, firmware-integráció és hardverpróba NOT RUN; ág: `feat/h8-source-inventory`. | PR és háromplatformos ellenőrzés; utána a SCI/IRQ/timer időzítési és hostszolgáltatás-határának kialakítása. A CPU-port még nincs kész. |
| 2026-10-27 | **Korabeli állapot:** [#24](https://github.com/RobCZart82/yaman1x/pull/24): digitális port helyi upstream összehasonlítása. | A rögzített MAME 11 eredeti regiszter/reset metódustörzsével 600000 determinisztikus művelet: **0 eltérés**, MSVC Release. [Próba határai](h8-digital-port.md#validation). | Végső platform-CI és teljes kutatási build ellenőrzése. Teljes MAME-eszköz-, CPU- és hardvervalidáció továbbra sem történt. |
| 2026-10-27 | **Korabeli állapot:** **#24 beolvadt, H8-periféria platformellenőrzés lezárva.** [Végső bizonyíték](h8-digital-port.md). | `896bd7882309bb03868c5c3816f4f9b7b068b858`: [Linux/macOS/Windows](https://github.com/RobCZart82/yaman1x/actions/runs/40468100477), [MSan](https://github.com/RobCZart82/yaman1x/actions/runs/40468100462) és [teljes kutatási MAME-build](https://github.com/RobCZart82/yaman1x/actions/runs/40468100442) PASS. A korábbi NOT RUN platformbejegyzések történeti állapotok. | SCI/IRQ/timer hostadapter és CPU-leválasztás. Az új GPIO firmware-integrációja és a friss Mac-firmware-próba továbbra is NOT RUN. M0/M1 még nem teljesült. |
| 2026-10-27 | **Korabeli állapot:** [#26](https://github.com/RobCZart82/yaman1x/pull/26): **M0 H8H belső IRQ-választó elkészült külön komponensként.** [Szemantika és hiányzó adapter](h8-internal-irqs.md). | Helyi MSVC Release build/teszt PASS: 11 prioritáscsoport és 44 belső vektor, 990 vektorpár, DMA-elfogyasztás, szűrők, NMI-latch, reset és izoláció. Platform-CI a külön PR-ben; CPU-/SCI-/firmware-integráció NOT RUN. | Teljes IRQ-adapter, külső lábak és CPU-szűrők bekötése; időzített SCI-modell és friss Mac-firmware-próba. Az NMI-élt és időfeltételt a tulajdonosnak kell igazolnia. |
| 2026-10-27 | **Korabeli állapot:** [#27](https://github.com/RobCZart82/yaman1x/pull/27): **M0 H8H IRQ-regiszter/láb adapter elkészült.** [Szemantika, ICR-kockázat és inbox-értékelés](h8h-irq-controller.md). | Helyi MSVC Release tesztek PASS; az eredeti upstream metódustörzsekkel 300000 determinisztikus művelet, **0 eltérés**. A PIN/NMI/IER/ISR/ISCR/ICR és publikációs határok megőrizve; CPU/SCI/firmware-bekötés NOT RUN. | Platform-CI és kutatási build; utána időzített SCI- és CPU-adapter. Az új PKS-HLE/dummy-ROM jegyzet nem tekinthető protokollbizonyítéknak; feltételezett választ nem vezetünk be. |
| 2026-10-27 | **Korabeli állapot:** [#28](https://github.com/RobCZart82/yaman1x/pull/28): **M0 SCI belső ciklusóra-adapter.** [Részletes szerződés, tesztek és következő lépés](h8-sci-cycle-clock.md). | Helyi MSVC Release próba: 1024 osztó, TX/RX ciklusnézet, blokkfelosztás, standby és hibahatárok PASS. Eredeti upstream ütemezési metódusokkal 200000 determinisztikus művelet, **0 eltérés**. Végső platform-CI a külön PR-ben; bájtállapotgép és CPU-/firmware-integráció NOT RUN. | SCI TX/RX és SSR/RDR mellékhatások, IRQ-kapcsolat, majd közös gépütemező és tényleges H8-végrehajtás. Külső SCI-órás módok még nem támogatottak. |
| 2026-10-27 | **Korabeli állapot:** [#29](https://github.com/RobCZart82/yaman1x/pull/29): **M0 részben teljesült:** önálló [SCI SSR státusz-latch](h8-sci-status.md), olvasás utáni törlés, debugger/DMA mellékhatások és upstream reset-viselkedés. A teljes UART még nincs bekötve. | Windows MSVC Release: 33 554 432 átmenet független bitenkénti orákulummal PASS; külön szekvenciatesztek PASS. A végső revízió és platformeredmények a kapcsolódó PR checkjeinél. Firmware/MIDI/hardver: NOT RUN. | SCI TX/RX bájtállapotgép, TDR/RDR és megszakítások; az SSR-write utáni tx_start/sync_rx_start tulajdonosi lépések és ciklusóra összekötése, majd CPU-bekötés. |
| 2026-10-27 | **Korabeli állapot:** [#30](https://github.com/RobCZart82/yaman1x/pull/30): **Ötletfeljegyzés:** PKS HLE, fokozatos protokollbizonyítás, dummy ROM korlátai és távlati hardveres helyettesítés lehetőségei rögzítve az M1 alatt. | Dokumentációs értékelés; új működési vagy hardveres bizonyíték nincs, M0/M1 állapota változatlan. | Firmware-elvárások és tényleges kommunikáció bizonyítása; csak utána döntés egy külön tesztelhető PKS-modellről. |
| 2026-10-27 | **Korabeli állapot:** [#31](https://github.com/RobCZart82/yaman1x/pull/31): **Architekturális követelmény:** cserélhető PKS HLE/firmware megvalósítás, közös időzített perifériafelület és verziózott mentett állapot; áttérési út az M1 alatt és ADR-0001-ben. | Dokumentációs döntés; nincs megvalósított backend vagy új dump, M0/M1 változatlan. | A bizonyított protokoll alapján interfész és szintetikus tesztek; dump esetén MCU-integráció és összehasonlító elfogadás. |
| 2026-10-27 | **Korabeli állapot:** [#32](https://github.com/RobCZart82/yaman1x/pull/32): **M0 licencnyilvántartás megerősítve:** javított harmadikfél-összesítő; 219 fájl, 5 licencszöveg nyilvántartása és CTest-ellenőrzése. [Hatókör és nyitott feltételek](../THIRD_PARTY.md#reviewed-sourcenotice-coverage). | Helyi Windows: 7 célzott pozitív/negatív teszt PASS; forrás-/értesítés-lefedettség PASS. A végső revízió és háromplatformos eredmények a kapcsolódó PR checkjeinél. Ez nem jogi vagy bináris kiadási jóváhagyás. | Új átvételnél kötelező nyilvántartás és licencvizsgálat; saját projektlicenc döntése, teljes H8/SDK függőségzár és tényleges kiadási csomag értesítéseinek ellenőrzése továbbra is nyitott. |
| 2026-10-27 | **Korabeli állapot:** [#34](https://github.com/RobCZart82/yaman1x/pull/34): **M0 részben teljesült:** [aszinkron SCI TX](h8-sci-async-tx.md), TDR/puffer, SSR/ciklusóra összekötése, adási vonal és TXI/TEI események. RX/CPU még nincs bekötve. | Windows MSVC Release: 4096 adat/keretezés kombináció és célzott tulajdonosi szekvenciák PASS; eredeti upstream metódusokkal 200000 művelet, 82931 TX-tick, 0 eltérés. Végső revízió/platformok a kapcsolódó PR checkjeinél. Firmware/hardver/audio NOT RUN. | Aszinkron RX/RDR és hibák, közös TX/RX óra/státusz, IRQ/CPU MMIO; utána friss firmware-es MIDI-elfogadás. Paritás upstream szerint megőrizve, nem igazolt hardvertény. |
| 2026-10-27 | **Korabeli állapot:** [#35](https://github.com/RobCZart82/yaman1x/pull/35): **M0 részben teljesült:** [aszinkron SCI RX](h8-sci-async-rx.md), vonalmintavétel, RDR és RXI/ERI kérések, túlcsordulás/paritás/keretezési hibák. Külön RX-tulajdonos; közös TX/RX és CPU még nincs bekötve. | Windows MSVC Release: 4096 adat/keretezés kombináció és célzott hibás szekvenciák PASS; eredeti upstream metódusokkal 896623 összehasonlítás, 663805 RX-tick, 0 eltérés. Végső revízió/platformok a kapcsolódó PR checkjeinél. Firmware/hardver/audio NOT RUN. | Közös TX/RX regiszter/óra/státusz és azonos idejű események; IRQ/CPU MMIO; utána friss firmware-es MIDI-elfogadás. A külön TX/RX osztályok együtt még nem teljes SCI-csatorna. |
| 2026-10-27 | **Korabeli állapot:** [#36](https://github.com/RobCZart82/yaman1x/pull/36): **M0 részben teljesült:** [közös aszinkron SCI-csatorna](h8-sci-async-channel.md), egy regiszter/óra/SSR-tulajdonos TX/RX állapotgépekkel. Régi felületek kompatibilitási rétegként megmaradtak, a bitlogika közös. | Windows MSVC Release: 4096 teljes duplex adat/keretezés kombináció és célzott sorrend-/státusz-/izolációs tesztek PASS; eredeti upstream óra/TX/RX metódusokkal 1153468 összehasonlítás, 804316 közös él, 0 eltérés. Végső revízió/platformok a kapcsolódó PR checkjeinél. CPU/firmware/hardver/audio NOT RUN. | Négy SCI IRQ-kérés útvonalazása és hatregiszteres MMIO-adapter, két SCI-csatorna tesztje; majd CPU/periféria-ütemező és friss firmware-es MIDI-elfogadás. |

## A cél pontos meghatározása

Az első cél egy macOS Apple Siliconon futó **VST3 hangszer**, amely REAPER-ben
betöltve, MIDI note-on hatására egy AN1x-hangszínt megszólaltat, note-off után
pedig annak elengedését végrehajtja. A hangot az eredeti, változatlan helyi
firmware által vezérelt DSP-program állítja elő. Egy scene és egy egyszerre
megszólaló hang elegendő az első bemutatóhoz. A szükséges chipek számát a
feltárt hangút határozza meg; a kétchipes működést nem tekintjük késznek.

A demonstrációhoz rögzített firmware-, hangszín- és konfigurációazonosító,
ismételhető indítás és helyi REAPER-hangfelvétel tartozik. A kísérleti státusz-,
EG- és órajelprofil minden eredmény mellett szerepel. Egy sikeres megszólalás
önmagában nem igazolja az AN1x teljes bootját vagy hangzásazonosságát.

Az első változathoz minimális beállítási lehetőség kell a helyi ROM-fájlok
kiválasztására és az indítás állapotának jelzésére. Az egyedi grafikus skin,
AU, Windows, teljes polifónia, bankböngésző és MEG-effektek későbbi feladatok.

## Jelenlegi helyzet és kutatási előzmények

A ROM-mentes motoralap mai állapota a fenti M0-táblában és a #36 lezárásában
szerepel. Az alábbi firmware-/DSP-megfigyelések a hivatkozott kutatási
jegyzőkönyvek korábbi felvételei; a dokumentáció egységesítésekor nem futtattunk
új firmware-t vagy hardvermérést. Nem tekinthetők a közös SCI-adapter friss
firmware-integrációs bizonyítékának.

| Terület | Bizonyított eredmény | Nyitott feladat |
|---|---|---|
| H8 és firmware | A kutatási MAME-target futtatja a helyi 1.04 ROM-ot; kísérleti státuszprofil mellett mindkét DSP programját feltölti | Megismételhető indulás a hangszín- és MIDI-kezeléshez szükséges állapotig |
| MIDI és PKS | RXD0 a MIDI bemenet; a jel eléri a modellt. SCI1-en 30 TDR-írás és nulla RDR-olvasás látszik a párosított próbákban | A PKS-protokoll és az SCI0 bekapcsolásának vezérlési útja; tényleges MIDI-bájtvétel |
| DSP és időzítés | Két VOP3-modell és mintaléptetés működik; a feltöltött mezők auditálhatók | Órajel, EG-viselkedés, feltöltés alatti futás, táblarom-hozzáférés és soros hangút |
| Táblarom | Külső 128 KiB-os kép egyes táblái egyeznek a CPU-ROM-ban lévő másolattal | Indexskála, modellbe kötés és a második panel ROM-jának azonossága |
| Hang és plugin | Szintetikus DSP-soros próba és ROM-mentes CI van | AN1x offline hang, termékmotor, VST3 és REAPER-próba |

Részletek: [STATUS](STATUS.md), [SCI-megfigyelések](sci-state-milestone.md),
[táblarom és órajel](an1x-clock-and-table-rom.md),
[időzített DSP-kísérlet](timed-vop3-experiment.md).

## Mérföldkövek és elfogadási feltételek

### M0 — Rögzített fejlesztési alap és motorarchitektúra

2026-10-27: **részben teljesült**. A következő alapok elkészültek:

- Az [ADR-0001](ADR-0001-engine-boundary.md) kiválasztotta a motorhatárt, a
  külön MAME kutatási utat és a tervezett VST3 SDK-t. Az architekturális választás
  rögzített; a CPU/periféria-port végrehajtása és elfogadása még hátravan.
- A [függőségazonosítók](engine-dependencies.json), a forráslockok, a H8-leltár
  és a generátor bemenetei/kimenetei rögzítettek. A teljes fordítói/licenc-
  függőségzár ezzel még nincs lezárva.
- A [buildalap](BUILD_BASELINE.md) dokumentálja a helyi Windows referencia-
  környezetet és a külön teljes macOS arm64 MAME-build rögzített adatait.
  A kutatási build előfeltétele a #20 óta teljesült, a #36-nál is PASS.
  A hosztolt CI-runner verziói ettől nem válnak állandó toolchain-lockká.
- Önálló GPIO, IRQ-adapter és [közös aszinkron SCI TX/RX-csatorna](h8-sci-async-channel.md)
  áll rendelkezésre. Komponensszintű időléptetési/izolációs tesztjeik sikeresek;
  ez még nem teljes CPU-/gépizolációs vagy firmware-elfogadás.
- A forráseredet és a [licenc-/értesítés-lefedettség](../THIRD_PARTY.md#reviewed-sourcenotice-coverage)
  ellenőrzése automatizált. A saját projektlicenc és a tényleges kiadási csomag
  megfelelőségének ellenőrzése továbbra is nyitott.

A [kétcsatornás SCI MMIO/IRQ-adapter](h83002-sci-mmio-irq.md) szintén elkészült: bájt-/szóhozzáférés, DMA-elsőbbség és csatornánkénti vektorkiosztás.

Az önálló SCI-idővonal (#41), CPU-hoszthatár (#42), buszsegédek (#43),
cikluskeret (#44) és [végrehajtási sorrend](h8-execution-runner.md) elkészült.
A [beolvadt H8H-részhalmaz](h8-first-instructions.md) már fordul és eredeti
full/partial kezelőket hajt végre. A #57-kori 366 kezelő támogatott műveletei (történeti részhalmaz):
memória és prefixek, rel8/rel16 feltételek, kiválasztott aritmetika/logika,
egybites shift/rotate, regiszteres/indirekt/abszolút bitműveletek, normál
[veremkezelés és vezérlésátadás](h8-stack-control-flow.md). Ez részhalmaz,
nem teljes CPU. A korábbi bővítések és mérések az előrehaladási naplóban maradnak.

Nyitott feladatok, végrehajtási sorrendben:

1. Lezárni a leltárban rögzített 31 hiányzó H8H-kódolássort. A rögzített
   SYSCR-es IRQ/TRAPA-belépés és CPU-szűrés már tesztelt; a további SYSCR-módok nyitottak. Az önálló
   részhalmaz már fordul és fut; a teljes CPU lefedettsége még nem igazolt.
   Ezután a közös CPU/periféria-ütemező és a teljes címtér/MMIO-gép összekötése,
   az azonos idejű külső események sorrendjének külön igazolásával.
2. A teljes gép CPU-/RAM-/periféria-/DSP-/MIDI-állapotának példányizolációját,
   resetjét és blokkfelosztástól független időléptetését igazolni.
3. Az újonnan ténylegesen beépített függőségek licenceit lezárni; a saját kód
   licencéről a bináris terjesztés előtt dönteni. A kiválasztott SDK tényleges
   integrációját és összekapcsolt részeit akkor ellenőrizni; az SDK még tervezett.
4. Megőrizni a ROM-mentes alapbuildet és a külön kutatási utat; a beágyazott
   motorhoz saját, revízióhoz kötött referencia-buildet rögzíteni.

**Kész, ha:** van rövid architektúradöntés, függőséglista és megismételhető
buildutasítás. A kiválasztott motor CPU-, periféria-, DSP- és MIDI-állapota
példányonként elkülöníthető, időléptetése külső audiofolyamhoz illeszthető.

### M1 — Indulási út, PKS és valódi MIDI-vétel

2026-10-27: **előkészítve, nem teljesült**. Elkészült az
[időbélyeges SCI-hozzáférés-megfigyelő](m1-sci-access-milestone.md), a naplóellenőrzés
és a [Mac M1 párosított helyi próba futtatója](MAC_M1_HANDOFF.md). Ezek szintetikus
szervezési/hibakezelési tesztjei sikeresek; nem friss firmware-es elfogadások.

**Teljesült előfeltétel:** a [rögzített forrású teljes kutatási build](source-identity-recovery.md)
#20 óta sikeres, és a #36 külön macOS buildje/regisztrációs próbája is PASS.
A build sikere önmagában nem bizonyít tényleges SCI-bájtvételt.
Az önálló közös SCI-modell komponensbizonyítéka az M0 része; a fő CPU-hoz még
nincs bekötve.

**Következő elfogadási lépés:** a Mac M1 átadási útmutató szerint friss helyi
firmware-futást és párosított megfigyelést készíteni, majd a tényleges SCI0
bájtfogadást és a note-on/note-off feldolgozását bizonyítani. A friss elfogadáshoz
nincs rögzített eredmény; dokumentált státusza **NOT RUN**. A PKS-kézfogás és az
SCI0 engedélyezésének okozati útja továbbra is kutatási feladat; a ROM-mentes CI
nem váltja ki.

- Visszakövetni a firmware SCI1/PKS-inicializálását és az SCI0 engedélyezését.
  A mostani megfigyelés alapján a PKS-válasz hiánya lehetséges akadály, de
  okozati összefüggését még igazolni kell.
- Megállapítani a szükséges PKS-kézfogást firmware-elemzéssel és elérhető
  hardveres bizonyítékokkal. Ha a hiányzó ROM helyett HLE szükséges, annak
  minden válaszát, állapotát és időzítési feltételezését dokumentálni.
- Ellenőrizni az SCI0 konfigurációját, a beérkezett bájtokat és a firmware
  eseménykezelőjéig vezető utat; az RXD-vonalátmenetek önmagukban nem elegendők.
- Rögzíteni egy determinisztikus induló hangszínt és rendszerállapotot.
  Az üres NVRAM, gyári inicializálás és normál indulás útját külön tisztázni.

**Kész, ha:** friss indulás után a firmware bizonyíthatóan fogad egy ismert
note-on/note-off bájtsorozatot és annak megfelelő voice/EG/regiszterváltozást
végez. A próba megismételhető, a használt perifériaprofilok rögzítettek.

**Ha elakad:** a hiányzó protokollt vagy hardvermérést konkrét feladatként
rögzíteni. A további offline DSP-kutatás folytatható, de egy közvetlenül
feltöltött DSP-kép megszólaltatása még nem teljesíti a firmware-es MIDI-célt.

#### Kutatási ötletek: a hiányzó PKS-program lehetséges helyettesítése

Feljegyezve: 2026-10-27, az `AN1x_DUMP-hiánypótlás.txt` ötleteinek
értékelése alapján. **Lehetőségek és hipotézisek; nem igazolt működés,
nem megvalósított funkciók és nem új elfogadási bizonyítékok.** M1 állapota
és elfogadási feltételei ettől nem változnak.

- **Minimális PKS HLE-modell lehetősége.** Elképzelhető, hogy az eredeti
  HD63B05-program futtatása helyett a fő CPU által megfigyelhető kommunikáció
  modellezése elegendő. Előbb azonosítani kell a kéréseket, válaszbájtokat,
  állapotátmeneteket, reset- és időzítési szabályokat. A PKS-válasz hiánya
  jelenleg csak lehetséges indulási akadály; az okozati kapcsolat nem igazolt.
- **Fokozatos bizonyítás.** Először firmware-elemzéssel követni a fő CPU
  elvárásait; lehetőség szerint működő hangszer privát buszmegfigyelésével
  ellenőrizni. Ezután kis, külön kapcsolható protokollmodellt és pozitív/negatív
  teszteket készíteni. Meg kell különböztetni a minimális indulási kézfogást
  a teljes kezelőpanel-viselkedéstől. A siker mércéje továbbra is a tényleges
  firmware-es bájtfogadás és eseményfeldolgozás, nem egy kényszerített státusz.
- **Dummy ROM csak betöltési kísérletként merülhet fel.** Egy 4096 bájtos
  üres fájl nem helyettesíti a programot vagy a protokollt. Csak elkülönített,
  egyértelműen szintetikus tesztadatként lenne értelme, ha konkrét betöltési
  kérdést vizsgálunk; nem nevezhető valódi dumpnak és nem bizonyít bootot.
  Előnyösebb lehet egy kifejezetten ROM nélküli HLE-mód, ha a protokoll már
  bizonyított. Jelenleg egyik megoldás bevezetéséről sincs döntés.
- **Fizikai mikrokontrolleres helyettesítés: külön távlati irány.** Csak
  bizonyított protokoll, elektromos illesztés és időzítés alapján mérlegelhető.
  A szervizrajz önmagában nem tárja fel a program működését; a javasolt Hitachi
  változatok láb-, programozási és működési kompatibilitása nincs igazolva.
  Ez nem előfeltétele a jelenlegi szoftveres fejlesztésnek.

**Továbbfejleszthetőségi követelmény — 2026-10-27.** Ha bizonyított HLE
áthidalást vezetünk be, azt cserélhető PKS-megvalósításként kell tervezni.
A későbbi eredeti dumpot futtató MCU-modell ugyanahhoz az időbélyeges
perifériafelülethez kapcsolódjon; a plugin és a fő H8 firmware ne függjön a
HLE belső megoldásaitól. Ez tervezési követelmény, nem elkészült funkció.
Részletek: [ADR-0001: cserélhető PKS-megvalósítás](ADR-0001-engine-boundary.md#replaceable-pks-implementation-and-future-dump-integration).

Tervezett áttérési út és elfogadási feltételek:

1. Bizonyított protokoll alapján rögzíteni a közös bemenet/kimenet, reset és
   időléptetés szerződését; szintetikus tesztekkel ellenőrizni az izolációt.
2. A HLE-t külön azonosított kísérleti profilként megtartani. A mentett állapot
   tartalmazza a megvalósítás/profil azonosítóját, sémaverzióját és a szükséges
   ROM tartalmi azonosítóját; Yamaha bájtokat vagy helyi abszolút utat ne.
3. Ha elérhető lesz a dump, ellenőrizni az eredetét/azonosítóját, a pontos MCU-t,
   memóriatérképet és perifériákat; csak ezután készíteni firmware-t futtató
   modellt. Az eredeti program megléte önmagában nem bizonyít hardverhűséget.
4. Azonos időzített ingerekkel összevetni a HLE-t és az új modellt; indulás,
   kezelőpanel, reset, firmware-es MIDI és audio elfogadását újrafuttatni.
   A bizonyíték alapján tudatosan kijelölni az új alapértelmezett profilt.
5. Váltáskor ellenőrzött újrainicializálást használni; futó audio közbeni csere
   nem követelmény. Régi HLE-állapotot tilos észrevétlenül MCU-állapotként
   betölteni. Hiányzó ROM/profil esetén egyértelmű jelzés szükséges; migráció
   csak külön dokumentált és tesztelt támogatásként ígérhető.

Korlátok: a jelenlegi LCD-buszmodell és SysEx-eszközök nem helyettesítik a
PKS-kézfogást. A PKS HLE ötlete a külön VOP3 ROM-ok hiányát nem oldja meg.
Nem vezetünk be feltételezett sikeres válaszokat vagy firmware-módosítást
pusztán az ötletjegyzet alapján. A hardveres bizonyítékhoz kötött EG target-hold
szabály változatlan. A feljegyzés nem teszi feladattá ROM, bank vagy privát
trace repository-ba másolását.

### M2 — Egy hangszínhez elegendő DSP- és táblarom-modell

- Külső bemenetként betölteni és azonosítani a szükséges táblaromot; rögzíteni
  az előjel-, szó- és címezési szabályokat. A DMS ROM azonosságát külön kezelni.
- Feltárni a választott hangszínhez szükséges indexskálát és táblahasználatot.
  A táblák tartalmi egyezése nem helyettesíti a címezés ellenőrzését.
- Megvizsgálni az EG indítását, célkeresztezését, tartását és elengedését,
  valamint a firmware által használt 0x21/0x22 visszaolvasásokat.
- Rendezni az upload alatti megállítás/újraindítás és a CPU–DSP eseménysorrend
  modelljét. A regiszterfeltöltés sikere nem jelenti ezek helyességét.
- Meghatározni az első demonstráció motoróráját. Ha csak feltételezett érték
  áll rendelkezésre, névvel jelölt kísérleti profilt használni, és ebből a
  megfelelő hangmagasságra vagy hardveres időzítésre nem következtetni.

**Kész, ha:** a kiválasztott voice determinisztikusan fut, az érintett DSP-állapotok
végesek, note-on és note-off hatására a szintek változnak. Minden még feltételezett
órajel-, EG- és címzési szabály szerepel a mérési jegyzőkönyvben.

### M3 — Első száraz hang offline rendereléssel

- Követni a voice kimeneti soros slotjait és az esetleges chipközi kapcsolatot.
  A szükséges útvonalat a rajz, a firmware és a DSP-program alapján azonosítani.
- Kialakítani egy megfigyelhető száraz kimeneti pontot. A MEG elhagyása csak
  dokumentált kutatási száraz út lehet; nem állítjuk róla, hogy a teljes gyári
  kimenet másolata. Ha a hanghoz további chip kötelező, azt be kell kötni.
- Meghatározni a szóformátumot, skálázást, jelkésést és a csatornák jelentését.
  Az upstream FS1R DAC-segédfüggvény AN1x-re való alkalmazását külön igazolni.
- Ugyanazon firmware/MIDI útvonalból helyi WAV-fájlt renderelni: kezdeti csend,
  note-on, tartott hang, note-off és lecsengés. Diagnosztikához hibás számértéknél
  némítani; kezdetben alacsony kimeneti szintet használni.

**Kész, ha:** a WAV-ban hallható, nemnulla, véges jel van; a hang indulása és
elengedése a MIDI-eseményekhez kötődik. Nincs tartós túlvezérlés vagy váratlan
egyenkomponens. Azonos bemenet ismételt renderelése egyezik az előre rögzített
numerikus tolerancián belül. A felvételhez profil-, ROM- és buildazonosító tartozik.

### M4 — Beágyazható, audio-blokkokkal működő motor

- Kialakítani az init, reset, időbélyeges MIDI-bemenet, render és állapotkezelés
  felületeit. A firmware indítása és a fájlbetöltés az audiofeldolgozáson kívül történjen.
- Egy idővonalra illeszteni a CPU-t, perifériákat és DSP-ket. A DAW-események
  blokkon belüli helyét megőrizni; a modellezett soros átvitel késleltetését is kezelni.
- A belső motoróra és a hoszt mintavételi frekvenciája közé szükség esetén
  mintavétel-konverziót építeni; a hoszt frekvenciája ne írja át észrevétlenül a DSP-modellt.
- Előre lefoglalni a szükséges memóriát. Az audioútból eltávolítani a fájlműveletet,
  dinamikus foglalást, blokkoló várakozást és korlátlan naplózást.
- Kezelni az all-notes-off, reset, leállítás és újraindítás eseteit, valamint a
  MIDI-eseménysor túlcsordulását dokumentált hibaviselkedéssel.

**Kész, ha:** a beágyazott motor az M3 referenciafolyamát különböző blokkfelosztásokkal
is reprodukálja. Az első célbeállításon, például 48 kHz/256 mintás hosztblokkal,
tartósan az audiohatáridőn belül fut. A mért idő és tartalék dokumentált;
44,1 kHz és más blokkméretek támogatását külön ellenőrizzük.

### M5 — Minimális VST3 hangszer

- A motorhoz VST3 adaptert készíteni a rögzített SDK/keretrendszer-verzióval;
  audio-kimenetet, MIDI-eseményfogadást és példányonkénti állapotot biztosítani.
- Minimális felületet adni a ROM-kiválasztáshoz, indítási állapothoz és hangerőhöz.
  A hiányzó/ismeretlen ROM kezelése legyen egyértelmű, a plugin maradjon némítható.
- A hoszt pluginkeresése ne indítson hosszan futó firmware-próbát. A motor
  elkészültéig a feldolgozás determinisztikus csendet adjon.
- Projektmentéshez tárolni a szükséges profilokat, hangszínállapotot és
  ROM-azonosítókat. Visszatöltéskor egyező helyi fájlokat keresni/kérni;
  a ROM-tartalom ne kerüljön a pluginba vagy a projektállapotba.
- Megépíteni a REAPER-rel egyező architektúrájú helyi `.vst3` csomagot,
  ellenőrizni a formátum validátorával, és előkészíteni a helyi telepítést.

**Kész, ha:** a VST3 betölthető, MIDI-t fogad és ugyanazt a referenciahangot adja,
mint a motor önálló renderelése. A projekt mentése/újranyitása megőrzi a szükséges
beállításokat; hiányzó ROM esetén érthető hibajelzés és csend az eredmény.

### M6 — Az első REAPER-ben megszólaló hang igazolása

1. Rögzíteni a macOS-, gép-, REAPER-, pluginbuild- és architektúra-adatokat,
   az audioeszközt, frekvenciát, blokkméretet, ROM-hasheket és a választott voice-t.
2. A helyi VST3-at felismertetni, üres projekt hangszer-sávjára betölteni,
   a helyi ROM-okat kiválasztani és megvárni az induláskész állapotot.
3. Beállítani a sáv audioútját a master kimenetre. Rövid MIDI-itemmel indítani
   egy ismert hangot, például a 60-as MIDI-hangszámot, majd note-offot küldeni.
4. Ellenőrizni a hangot lehallgatással és mérővel: indulás, tartás, elengedés,
   ismétlés és leállítás. A note-off/all-notes-off ne hagyjon beragadt hangot.
5. A REAPER-kimenetet helyi WAV-ba menteni, összevetni az offline referenciával,
   majd a projektet újranyitva megismételni a próbát. A különbségekhez rögzített
   késleltetés- és numerikus tolerancia tartozzon.

**Kész, ha:** a fenti jegyzőkönyv és felvétel alapján a REAPER-ben a YamAN1x
firmware-vezérelt DSP-hangja hallható és reprodukálható. Rögzíteni kell, hogy
mely tulajdonságok kísérletiek. A ROM-mentes forrás és a helyi kutatási adatok
publikálási határa továbbra is megmarad.

## Végrehajtási sorrend és döntési pontok

Fő függőségi lánc: **M0 → M1 → M2 → M3 → M4 → M5 → M6**.
M1 közben az M2 táblarom-/órajel-kutatása önállóan haladhat. Egy ROM-mentes
VST3 szintetikus próbája később segítheti a hosztadapter ellenőrzését, de a
végcélt a firmware-vezérelt jelút teljesíti.

Minden mérföldkőhöz kis, önálló PR, reprodukciós parancsok, elvégzett próbák és
frissített státusz tartozik. A repository CI a ROM-mentes kódot ellenőrzi;
a helyi firmware-, audio- és REAPER-próbát külön jegyzőkönyv igazolja.

A legnagyobb bizonytalanság M1–M3: PKS-protokoll, DSP-modell, táblarom-címezés
és hangút. Ha ezekhez új hardveradat kell, annak pontos kérdését és beszerzési
lépését rögzíteni kell. Naptári kiadási dátumot az offline referenciahang
megszületése előtt nem vállalunk; M3 után a motor- és plugincsomagolás becsülhető.

## 2026-10-27 — Korlátozott ciklus-eseményfuttató

PR: [#40](https://github.com/RobCZart82/yaman1x/pull/40), ág: `feat/bounded-cycle-dispatcher`, alap: main #38; a nyitott #39-től független. [Komponensszerződés és tesztek](cycle-dispatcher.md): monoton ciklusidő, befoglaló határidő, folytatható lépéskeret, hibás tulajdonos elkülönítése. 1000 eseménysor blokk-/keretfüggetlensége és határesetei helyben PASS. Ez az ütemező alapja; CPU/periféria-tulajdonos, hardveres eseménysorrend és firmware-végrehajtás továbbra is nyitott. M0 részben teljesült.

## 2026-10-27 — SCI busz- és IRQ-integráció fejlesztési bejegyzés

PR: [#39](https://github.com/RobCZart82/yaman1x/pull/39), ág: `feat/h8-sci-mmio-irq`, alap: main #37. Két SCI-csatorna hatregiszteres MMIO-ja és 52–59 IRQ-vektorai bekötve a különálló IRQ-vezérlőbe. Helyi macOS arm64 Release: 23/23 tesztcsoport PASS; 512 RX bájt/csatorna eset, TX, DMA, hibák, reset és izoláció. ASan/UBSan PASS. Részletes [komponensjegyzőkönyv](h83002-sci-mmio-irq.md). Platform-CI a kapcsolódó PR-ben ellenőrizendő. M0 részben teljesült; CPU-/firmware-/MIDI-/audioelfogadás nincs. Következő lépés: közös CPU/periféria-ütemező és tényleges CPU-port.

### #40 szinkronizálása a beolvadt #39-cel

2026-10-27: a közös build-/naplófájlok ütközése feloldva, mindkét komponens és tesztcsoport megtartva. Az egyesített csomag 24 CTest-csoportot és 60 Python-esetet tartalmaz. A korábbi 23-as eredmények történeti bejegyzések; új PR-head CI szükséges. A CPU/periféria-tulajdonos és M0 lezárása továbbra is nyitott.

## 2026-10-27 — Közös kétcsatornás SCI-idővonal

PR: [#41](https://github.com/RobCZart82/yaman1x/pull/41), ág: `feat/sci-cycle-timeline`, alap: beolvadt #40. [Komponensszerződés](sci-cycle-timeline.md): időbélyegzett RX-bemenet és két SCI-ciklusóra, korlátozott és folytatható eseményfeldolgozás. 256 kétcsatornás teljes duplex minta naplója azonos egyben, ciklusokra bontva és egyesével engedélyezett eseményekkel; 512 ismert RX-bájt ellenőrizve. Azonos ciklusban bemenetek forrássorrendben, majd SCI0/SCI1; szoftveres konvenció, nem hardveres bizonyíték. CPU/periféria-ütemezésből a CPU-rész és buszidőzítés továbbra is hiányzik; M0 részben teljesült.

Helyi ellenőrzés: macOS arm64 Release, 25/25 CTest-csoport és 60 Python-eset PASS; az új natív teszt ASan/UBSan alatt is PASS. Végső platform-CI a kapcsolódó PR-headhez kötve ellenőrizendő.

## 2026-10-27 — H8 CPU gazdaszolgáltatás-határ

PR: [#42](https://github.com/RobCZart82/yaman1x/pull/42), ág: `feat/h8-host-services-boundary`, alap: beolvadt #41. [Részletes határ és végrehajtási sorrend](h8-host-services-boundary.md). Hash-ellenőrzött leltár 30 forrásból és négy kétszer generált kimenetből; két teljes futás bájtazonos. Memóriabusz, szóigazítás, ciklustartozás/részutasítás, IRQ/DMA, idő, életciklus, standby/mentés és diagnosztika követelményei rögzítve. Ez előkészítés: a CPU még nincs lefordítva vagy végrehajtva, M0 részben teljesült.

Local validation / helyi ellenőrzés: macOS arm64 Release, 25/25 CTest groups and 68 Python cases PASS; source/notice audits PASS. Final PR-head platform CI pending.

## 2026-10-27 — H8H CPU buszhozzáférési réteg

PR: [#43](https://github.com/RobCZart82/yaman1x/pull/43), ág: `feat/h8h-bus-access`, alap: beolvadt #42. [Komponensszerződés](h8h-bus-access.md): öt H8H buszsegéd, utasítás/adat út, szóigazítás és hozzáférés előtti kétciklusos levonás. Eredeti upstream metódustörzsekkel 200000 vegyes művelet, 0 eltérés; minden 16 bites szóérték és hiba-/izolációs határ tesztelve. Ez nem CPU-utasításvégrehajtás: a részutasítás-/keretkezelés, valódi címtér és IRQ/SCI CPU-bekötés nyitott. M0 részben teljesült.

Local validation / helyi ellenőrzés: macOS arm64 Release, 26/26 CTest groups and 72 Python cases PASS; new native ASan/UBSan PASS. Final PR-head platform CI remains separate evidence.

## 2026-10-27 — H8 végrehajtási cikluskeret

PR: [#44](https://github.com/RobCZart82/yaman1x/pull/44), ág: `feat/h8-execution-budget`, alap: beolvadt #43. [Komponensszerződés](h8-execution-budget.md): ciklustartozás továbbvitele, perifériahatáridő és explicit szeletfázisok. Eredeti execute_run módszerrel 200000 szintetikus, esemény nélküli szelet; recompute_bcount módszerrel 200000 határidő, 0 eltérés. Natív buszkapcsolat 1000 felosztási történettel. Ez számtani komponens; a valódi full/partial utasításkezelők, CPU-állapot és IRQ/SCI ütemezési kapcsolata még nyitott. M0 részben teljesült.

Local validation / helyi ellenőrzés: macOS arm64 Release, 27/27 CTest groups and 74 Python cases PASS; new native ASan/UBSan and source/notice audits PASS. Final PR-head CI pending.

2026-10-27 dokumentációs lezárás: #44 beolvadt, végső head mind az öt CI-je PASS. A motor legutóbbi helyi eredménye 27/27 CTest-csoport és 74 Python-eset PASS. CPU-opcode végrehajtás, M0 teljes lezárása és M1/audio továbbra is nyitott.

## 2026-10-28 — H8 teljes/részutasítás-futtatási sorrend

PR: [#46](https://github.com/RobCZart82/yaman1x/pull/46). Ág: `feat/h8-execution-order`, alap: beolvadt #45. A [futtatókeret](h8-execution-runner.md) összeköti a ciklustartozást, perifériahatáridőket és full/partial callback-sorrendet. 200000 eseményes referencia-szelet, nulla eltérés az eredeti execute_run metódussal szintetikus kezelők mellett. Natív sorrend-/hiba-/1000 felosztási teszt. M0 részben teljesült; valódi CPU-utasítások, IRQ-belépés, firmware/MIDI/audio nem igazolt. A jelölt saját platform-CI-je külön ellenőrizendő.

## 2026-10-28 — Első valódi generált H8-kezelők

PR: [#47](https://github.com/RobCZart82/yaman1x/pull/47). Ág: `feat/h8-first-generated-handlers`, alap: beolvadt #46. [Részletes bizonyíték és korlátok](h8-first-instructions.md). Saját regiszter-/prefetch-/részutasítás-állapot, eredeti generált reset/NOP/MOV.B/BRA kód, explicit nem támogatott opcode-hiba. 200000 szintetikus szakasz összevetve az eredeti H8H-dekóderrel és futtatóval, 899400 olvasás, nulla eltérés; szándékos hibát a kontroll észleli. M0 részben teljesült. Következő: adatmemória-műveletek, feltételes elágazások, IRQ-belépés és CPU/SCI-sorrend. A teljes CPU-/firmware-/MIDI-/audioelfogadás továbbra is nyitott.

Előző lépcső lezárása: [#46](https://github.com/RobCZart82/yaman1x/pull/46): merge `00e16ce805d542e4a654923ddde2d462ebac78bb`, végső head `478294b0570b0140b0ba2c5f78b65fcc99fecaa4`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467656), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467504), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467588).

## 2026-10-28 — H8H memória és rel8 feltételek

PR: [#48](https://github.com/RobCZart82/yaman1x/pull/48). Ág: `feat/h8-memory-conditional-branches`, alap: beolvadt #47. [Részletes tesztek és korlátok](h8-memory-branches.md). 44 eredeti generált full/partial metódus, háromszavas IR, teljes ER-címek és kétciklusos adatbusz-hozzáférés. 200000 eredeti referencia-szakasz, 844255 utasításolvasás, 27571 adatolvasás, 27574 adatírás: nulla eltérés. M0 részben teljesült. Következő: szó-/hosszúszó-műveletek, aritmetikai/összehasonlítási jelzőbitek, majd IRQ és CPU/SCI-sorrend. Teljes CPU-/firmware-/MIDI-/audioelfogadás továbbra is nyitott.

Előző lépcső lezárása: [#47](https://github.com/RobCZart82/yaman1x/pull/47): merge `343f4c32c81229e89ec092b0d8e324f2a0305f8a`, végső head `6f8107ec35e511c6aa9d347295e67a882a7826d3`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640131062), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130940), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130959).

## 2026-10-28 — H8H szó-/hosszúszó és prefixállapot

PR: [#49](https://github.com/RobCZart82/yaman1x/pull/49). Ág: `feat/h8-word-long-memory`, alap: beolvadt #48. [Részletes bizonyíték és korlátok](h8-word-long-memory.md). Eredeti 0100 prefix, külön belső dekódállapot, igazított word busz és kétlépcsős long átvitel. 200000 referencia-szakaszon nulla eltérés; mid-transfer folytatás/hiba és unsigned címkörbefordulás tesztelve. M0 részben teljesült. Következő: aritmetikai/összehasonlítási jelzőbitek, szélesebb utasításkészlet, majd IRQ és CPU/SCI-sorrend. Firmware/MIDI/audio-elfogadás változatlanul nyitott.

Előző lépcső lezárása: [#48](https://github.com/RobCZart82/yaman1x/pull/48): merge `800f8b4eabfe484b54e9918a652096c0acf50986`, végső head `a2da3e66797ec5d9f617e3e85b1e6505ccf26beb`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345253), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345183), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345179). Az automatikus review a beolvasztáskor még futott; nem lezárt eredményként szerepel. A beolvasztás a kézi forrásellenőrzés, eredeti kódos kontrollok és mind az öt sikeres CI alapján történt.

## 2026-10-28 — H8H aritmetika és összehasonlítás

PR: [#50](https://github.com/RobCZart82/yaman1x/pull/50). Ág: `feat/h8-arithmetic-flags`, alap: beolvadt #49. [Részletes bizonyíték és korlátok](h8-arithmetic-flags.md). Byte ADD/CMP immediate, byte ADD/SUB/CMP register, word/long ADD/SUB/CMP immediate; H/N/Z/V/C és CMP nem-írás ellenőrizve. 200000 eredeti referencia-szakasz, nulla eltérés; független signed-range/maradékos orákulum és half-carry negatív kontroll. M0 részben teljesült. Következő: további regiszteres/carry-input aritmetika, logika/shift, stack/IRQ és CPU/SCI-sorrend. Firmware/MIDI/audio-elfogadás változatlanul nyitott.

Előző lépcső lezárása: [#49](https://github.com/RobCZart82/yaman1x/pull/49): merge `4567d7bfe852100f5483a8104bcba81b50ae26bb`, végső head `f69503487de2fbbf3c575d81f54ca5a91e965f90`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971856), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971826), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971835). Az automatikus review a `5aff015d6157f166d9f26d0591e7b13d067cd171` forrásrevízión hibát nem jelzett; a követő commit csak PR-metaadatot rögzített a dokumentációban.

## 2026-10-28 — Carry, regiszteres aritmetika és logika

Ág: `feat/h8-carry-register-logic`, alap: beolvadt #50. [Részletes bizonyíték és korlátok](h8-carry-register-logic.md). ADDX/SUBX bemenő carry és sticky Z; wide register ADD/SUB/CMP; byte és wide immediate logika; LDC CCR. 200000 eredeti referencia-szakasz: nulla eltérés; több mint egymillió carry/Z és 20000 többbyte-os lánc matematikai kontrollal. M0 részben teljesült. Következő: wide register logika, shift/rotate és bitműveletek, majd stack/IRQ és CPU/SCI. Firmware/MIDI/audio-elfogadás továbbra is nyitott.

Előző lépcső lezárása: [#50](https://github.com/RobCZart82/yaman1x/pull/50): merge `0f8d3ae96eb32dcabb3a039f40e5c0fd0315a1b9`, végső head `4bbea3449f5d73f15031332b537919b493a5defc`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119562), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119555), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119545). Automatikus review a `ddc8d51221838e64c9e849101756c1aeca260dce` forrásrevízión: kész, nincs találat; a követő commit csak PR-metaadatot rögzített.

Korabeli jelöltbejegyzés: carry/regiszter/logika fejlesztés: [PR #51](https://github.com/RobCZart82/yaman1x/pull/51), `feat/h8-carry-register-logic`. Helyben 32 CTest-csoport és 83 Python-eset PASS; egy symlink-jogosultsági eset kihagyva. A végső head teljes CI-je külön ellenőrzendő; M0 részben teljesült.

## H8-WIDE-LOGIC-2026-10-28 — széles regiszteres logika

Előző lépcső lezárása: [#51](https://github.com/RobCZart82/yaman1x/pull/51), merge `ba2c91d8a156972bcf3c7b9adab98e36f7279b0c`, végső head `931a5a50dfe57c575d45d9d8b6b817889501a5a0`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821739), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821601), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821621). A forrásrevízió (`df6ce5574346e32b57a81dffb5c5b5f0e7929bbf`) automatikus review-ja kész, nincs találat; a követő commit dokumentációs PR-metaadat. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-wide-register-logic`, alap: beolvadt #51. 138 eredeti generált full/partial kezelő: word/long regiszteres OR/XOR/AND és eredeti 01f0 prefix. 196608 word-eset, 245760 regiszter/CCR-kombináció, 65536 prefixkiterjesztés és 1000 felosztás PASS. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; XOR helyett OR hibát a kontroll észleli. [Szerződés és korlátok](h8-wide-register-logic.md). Helyben 33 natív CTest-csoport PASS; a teljes 34 csoport / 84 Python-eset végső CI-je külön ellenőrizendő. M0 részben teljesült; shift/rotate, bit, stack/IRQ és CPU/SCI-integráció nyitott.

Korabeli jelöltbejegyzés: széles regiszteres logika: [PR #52](https://github.com/RobCZart82/yaman1x/pull/52), `feat/h8-wide-register-logic`. Helyi 33 natív csoport / 83 Python-eset PASS; a teljes végső head CI-je külön ellenőrizendő.

## H8-SHIFT-ROTATE-2026-10-28 — egybites eltolás és forgatás

Előző lépcső lezárása: [#52](https://github.com/RobCZart82/yaman1x/pull/52), merge `88e88bd8551f78bc312230d459ea3c92d7b66f01`, végső head `eac6be6a95e22f1266730c6941efa0c766658913`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124816), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124527), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124813). A `c726582b72c43bc87cc917f1ff60c10f762a768a` forrásrevízió review-ja kész, nincs találat; a későbbi dekódersorrend-védelmet célzott és eredeti kódos teszt igazolja. Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-shift-rotate`, alap: beolvadt #52. 186 eredeti generált full/partial kezelő; SHLL/SHAL/SHLR/SHAR/ROTXL/ROTL/ROTXR/ROTR 8/16/32 biten. Független bitenkénti orákulum, signed-range overflow, 1744896 operandus/CCR/regisztersáv-eset, 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; a megerősített explicit carry-program a kihagyott ROTXR carry-t seed 2/slice 8 helyen észleli. [Szerződés és korlátok](h8-shift-rotate.md). M0 részben teljesült; kétbites változatok, bitműveletek, stack/IRQ és CPU/SCI-integráció nyitott. A jelölt teljes CI-je külön ellenőrizendő.

Korabeli jelöltbejegyzés: egybites shift/rotate fejlesztés: [PR #53](https://github.com/RobCZart82/yaman1x/pull/53), `feat/h8-shift-rotate`. Helyi 34 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-REGISTER-BITS-2026-10-28 — regiszteres bitműveletek

Előző lépcső lezárása: [#53](https://github.com/RobCZart82/yaman1x/pull/53), merge `806ddf86a6373731f7b138c985a55ed41c583d6a`, végső head `b234b460a19d63d9a6c80e2086a4172a9a313a26`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288385), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288495), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40665288416). A `8ea376e0ba91bbc83ff94b521c5f0082bb093811` forrás review-ja kész, nincs találat; követő commit csak PR-metaadat. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-register-bit-operations`, alap: beolvadt #53. 222 eredeti generált full/partial kezelő; regiszteres/immediate BSET/BNOT/BCLR/BTST, BST/BIST és carry logikai/load formák. 8126464 független operandus/bitindex/CCR/átfedési eset, 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz: nulla eltérés; a BNOT helyett BSET hibát seed 3/slice 51 helyen felismeri a kontroll. [Szerződés és korlátok](h8-register-bit-operations.md). **Tervpontosítás:** a kétbites shift/rotate az eredeti listában `s20` (H8S/2000), a H8H-dekóderben nincs; nem H83002-fejlesztési feladat. Memóriás bitműveletek, stack/control flow és IRQ/SCI-integráció nyitott; M0 részben teljesült. Végső jelölt-CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: regiszteres bitműveleti fejlesztés: [PR #54](https://github.com/RobCZart82/yaman1x/pull/54), `feat/h8-register-bit-operations`. Helyi 35 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-MEMORY-BITS-2026-10-28 — indirekt memóriás bitműveletek

Előző lépcső lezárása: [#54](https://github.com/RobCZart82/yaman1x/pull/54), merge `efdb6df59ba53e8497d313fb33c7b40dcd9d76c4`, végső head `f9cab7c831a6aa2e880e2131b52a3623c049347c`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059555), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059577), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40672059727). Automatikus review-eredmény a lezáráskor nem volt; helyi forrás-/független/eredeti kódos kontroll ellenőrizve. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-memory-bit-operations`, alap: beolvadt #54. 262 eredeti generált full/partial kezelő: 7c/7d prefix és 18 indirekt bájtos bitművelet. 7340032 immediate eset, 131072 cím/indexátfedés, 131072 prefixkiterjesztés, 31 megszakítási pont és 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; read/prefetch/write sorrend és pontosan egyszeri mellékhatás ellenőrizve. [Szerződés és korlátok](h8-memory-bit-operations.md). M0 részben teljesült; abszolút memóriás formák, stack/control flow, IRQ és CPU/SCI-integráció nyitott. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: indirekt memóriás bitműveleti fejlesztés: [PR #55](https://github.com/RobCZart82/yaman1x/pull/55), `feat/h8-memory-bit-operations`. Helyi 36 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-ABSOLUTE-BITS-2026-10-28 — abszolút memóriás bitműveletek

Előző lépcső lezárása: [#55](https://github.com/RobCZart82/yaman1x/pull/55), merge `e53b6fe7e2906b472e898c57d59ccfe1b791fa96`, végső head `452f9585afe1e67ceb57cd9cc8ac258202cd3cf7`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897203), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897191), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40677897202). Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-absolute-bit-operations`, alap: beolvadt #55. 302 eredeti generált full/partial kezelő; 7e/7f prefix és 18 abs8 bitművelet. 7340032 immediate eset, 262144 cím/regisztersáv eset, 131072 prefixkiterjesztés, 31 futtatási vágáspont és 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés. [Szerződés és korlátok](h8-absolute-bit-operations.md). M0 részben teljesült; stack/control flow, IRQ és CPU/SCI-integráció nyitott. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: abszolút memóriás bitműveleti fejlesztés: [PR #56](https://github.com/RobCZart82/yaman1x/pull/56), `feat/h8-absolute-bit-operations`. Helyi 37 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő.

## H8-STACK-CONTROL-2026-10-28 — veremkezelés és vezérlésátadás

Előző lépcső lezárása: [#56](https://github.com/RobCZart82/yaman1x/pull/56), merge `a3c2eb8ba25fc40eaa5057cc388260e4fc58b9df`, végső head `dee08721b3c2d3ccb16e1dfa4b4475625889c962`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502703), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502638), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40682502838). Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-stack-control-flow`, alap: beolvadt #56. 366 eredeti full/partial kezelő; predecrement/postincrement MOV byte/word/long, JMP/JSR mindhárom címzése, BSR rel8/rel16, mind a 16 rel16 feltétel, RTS és szintetikus RTE-veremkép-visszaállítás. Független előjeles eltolás-/alias-/címhatár-/CCR-próbák, 201 beágyazott és 388 további hívás/RTE vágáspont, buszhiba-kontroll. Eredeti H8H-kóddal 200000 szakasz: nulla eltérés; a hibás RTS SP+2 értéket seed 6/slice 5 helyen felismeri a kontroll. [Hatókör, bizonyíték és fennmaradó korlátok](h8-stack-control-flow.md). A normál verem/vezérlés utasításcsaládok ebben a rögzített H8H-profilban megvalósítva; IRQ/TRAPA-belépés, teljes CPU és CPU/SCI-integráció nyitott, M0 részben teljesült. Végső CI külön ellenőrizendő.

Korabeli jelöltbejegyzés: verem/vezérlésátadási fejlesztés: [PR #57](https://github.com/RobCZart82/yaman1x/pull/57), `feat/h8-stack-control-flow`. Helyi 38 natív csoport / 83 Python-eset PASS; a végső head teljes CI-je külön ellenőrizendő. Normál utasításcsaládok készültek el; IRQ/TRAPA-belépés továbbra is nyitott.

## DOC-RECONCILE-2026-10-28 — aktuális és történeti állapot egységesítése

A #57 lezárása a motoralaphoz rögzítve, a strukturált nyilvántartásban nincs nyitott motorjelölt. STATUS és roadmap egységesítve; korábbi jelöltbejegyzések megőrizve és történetiként jelölve. Az M0 következő feltétele már a hiányzó lefedettségre és gépintegrációra vonatkozik, nem az első CPU-fordításra. Dokumentációs javítás, új motor-/firmware-/audioelfogadás nélkül. Ellenőrzés: élő #57 merge/head és mind az öt CI-feladat, JSON-lezárás és egyedi PR-azonosítók, UTF-8 és helyi fájlhivatkozások, forrás- és értesítéslefedettségi audit.

A dokumentációs egységesítés PR-je: [#58](https://github.com/RobCZart82/yaman1x/pull/58). A motoralap továbbra is a lezárt #57; a dokumentációs PR saját aktuális CI-/merge-állapotát a hivatkozott PR mutatja.

## 2026-10-28 — repository-audit a #58 főágon

[Auditjelentés](REPOSITORY_AUDIT_2026-10-28.md): a 39 csoportos/84 Python-esetes csomag Macen teljesen PASS, 200000 eredetimetódus-szelet eltérés nélkül, célzott CPU ASan/UBSan PASS. Az ellenőrzés nem igazol firmware-, hardver- vagy audioelfogadást; M0/M1 állapota változatlan. A következő feladatok a megmaradt utasítás/címzés-lefedettség, IRQ/TRAPA és a közös gépütemezés.

## 2026-10-28 — lefedettség és CCR

[Jegyzőkönyv](h8-coverage-and-ccr.md): forrásazonosított kódolásleltár (181 kiválasztott / 83 hiányzó / 135 másprofilú sor), STC/LDC regiszteres és ORC/XORC/ANDC immediát család. Ez nem ISA-készültségi százalék és nem IRQ-belépési bizonyíték. Következő a fennmaradó utasítás/címzéscsaládok, majd IRQ/TRAPA és gépütemezés.

## 2026-10-28 — regiszterközi MOV

[Jegyzőkönyv](h8-register-transfers.md): mindhárom szélesség, teljes lane/alias megőrzés és N/Z/V szemantika. A forrásleltárban 80 hiányzó H8H-sor marad. CPU IRQ/TRAPA és gépütemezés továbbra is nyitott.

## 2026-10-28 — INC/DEC és kis cím-aritmetika

[Jegyzőkönyv](h8-small-arithmetic.md): 16 csoport, flagmegőrzés és részutasítás-felosztás tesztelve. A leltárban 64 hiányzó H8H-kódolási sor marad. M0/M1 készültsége ettől nem teljes.

## 2026-10-28 — unáris műveletek és kiterjesztés

[Jegyzőkönyv](h8-unary-extension.md): tíz új csoport, 434 full/partial kezelő; 54 hiányzó H8H-sor marad. Az eredetimetódus-próbák új családjai elérhető ciklusban és bejárási őrfeltétellel futnak. A mérföldkő nem IRQ-, firmware- vagy hangelfogadás.

## 2026-10-28 — szinkron TRAPA-belépés

[Jegyzőkönyv](h8-trapa-entry.md): default SYSCR-profil, négy vektor, eredeti stack/busz/időzítés. Külső IRQ/NMI, szűrők és gépütemezés továbbra is nyitott; hardveres trap-visszatérés nincs igazolva.

## H8-DISPLACEMENT16-2026-10-29 — 16 bites eltolású memóriaátvitel

Ág: `feat/h8-displacement16-memory`, alap: beolvadt #77. Hat eredeti MOV byte/word/long
load/store csoport, 12 változatlan full/partial metódus: 478 összesen;
231 kiválasztott / 33 hiányzó / 135 kizárt kódolássor. Mind a 65536 eltolás,
regiszterátfedések, teljes ER-címkörbefordulás, CCR és szóigazítás tesztelve.
201 felosztás és kétlépcsős store-hiba; eredeti dekóderrel 200000 szakasz nulla eltéréssel.
Mind a hat új forma és a korábbi referencia-családok elérésellenőrzése megmarad.
[Részletes bizonyíték és korlátok](h8-displacement16-memory.md). A jelölt teljes csomagja
56 CTest / 87 Python; firmware/audio-elfogadás továbbra sincs. Végső PR-CI külön bizonyíték.

Az eltolásos memóriautasítások PR-je: [#78](https://github.com/RobCZart82/yaman1x/pull/78). A készítéskori jelöltállapot és a végső CI-/merge-bizonyíték külön kezelendő.

## 2026-10-29 — Előjel nélküli bájtszorzás jelölt

Az eredeti MULXU.B full/partial kezelők kiválasztva; fix H8H MAC nélküli időzítés.
65536 operanduspár,19200 alias/CCR-határeset és161 szétvágott futás PASS;
200000 eredeti referencia-szelet nulla eltéréssel; szándékos product+1 hiba felismerve.
Helyi56 CTest/86 Python PASS, ismert Windows-symlink eset kizárva; távoli CI függőben.
Jelölt:57 CTest/87 Python,480 eredeti módszer,232 kiválasztott/32 hiányzó/135 kizárt sor.
A szó-szorzás külön nyitott: az eredeti u16 operandusok int-promóciója nagy szorzatnál
C++-túlcsordulási kockázatot jelent. Bekötés előtt független nagyértékű teszt,
UBSan és dokumentált unsigned-szélesítés felülvizsgálata szükséges.
Ez lehetőség és igazolandó javítási feladat; nem hardveres hibamegállapítás.
Részletek: [szerződés és bizonyíték](h8-byte-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli szó-szorzás jelölt

MULXU.W: két eredeti eredetű full/partial kezelő minimális, dokumentált javítással:
szorzás előtt u32-szélesítés, hogy a u16 operandusok int-promóciója ne csorduljon túl.
A korábbi480 kezelő változatlan;482 eredeti eredetű kezelőből2 adaptált.
327680 operandus-söprés,9600 alias/CCR-határeset,20000 véletlen pár,241 szétvágás PASS.
200000 eredeti referenciaszelet nulla eltéréssel, csak biztonságos szorzatokkal;
a nagy szorzatokat független64 bites oracle ellenőrzi. Product+1 kontroll:seed9/slice9.
Helyi57 CTest/86 Python PASS, ismert Windows-symlink eset kizárva. Jelölt:58 CTest/87 Python,
233 kiválasztott/31 hiányzó/135 kizárt kódolássor. Linux UBSan és külön eredeti-hibakontroll
bekötve; tényleges távoli eredmény még függőben. A szó-szorzás nyitott kockázata
csak ezek sikeres lezárása után tekinthető rendezettnek; ez nem hardverelfogadás.
[Szerződés, pontos adaptáció és bizonyítékok](h8-word-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjeles bájt- és szó-szorzás jelölt

Eredeti01c0 prefix és MULXS.B/MULXS.W full/partial kezelők; külön őrzött folytatás.
Az eredeti set_nz16/set_nz32 csak N/Z-t módosít. Független64 bites számítás:
65536 bájtpár,327680 szó-söprés,28800 alias/CCR-határeset,20000 véletlen pár PASS;
241 szétvágott futás és65536 prefixfolytatás ellenőrzött. Eredeti referenciaszeletek:
200000,nulla eltérés; mindkét forma elérése kötelező. Product+1 kontroll seed9/slice14.
Jelölt:59 CTest/87 Python,488 upstreamből származó kezelő (486 változatlan,2 korábbi
unsigned-adaptáció);235 kiválasztott/29 hiányzó/135 kizárt kódolássor. Linux UBSan
mindkét új formát is teszteli; távoli CI függőben, M0 továbbra is részleges.
[Szerződés és részletes bizonyítékok](h8-signed-multiply.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli bájtos osztás jelölt

Eredeti DIVXU.B full/partial kezelők, külön5100/ff00 dekóderág. Nullás osztónál
célregiszter-megőrzés; hányados alacsony8 bitje és maradék külön bájtban; N/Z
az eredeti osztófüggő szabályt követi. Ez forrásegyezés, nem hardveres bizonyíték.
393216 osztandó-söprés,2048 osztóhatáreset,30720 alias/CCR-eset,20000 véletlen pár
és241 szétvágás PASS;200000 referenciaszelet nulla eltéréssel. Hányados+1 kontroll
seed9/slice18 felismerve. Jelölt:60 CTest/87 Python,490 upstreamből származó kezelő
(488 változatlan,2 korábbi adaptáció);236 kiválasztott/28 hiányzó/135 kizárt sor.
Linux UBSan kiterjesztve osztásra is; távoli CI függőben. M0 továbbra is részleges.
[Szerződés és ellenőrzési hatókör](h8-byte-divide.md). Yamaha-adat nem került be.

## 2026-10-29 — Előjel nélküli szó-osztás jelölt

DIVXU.W, külön5300/ff08 dekóderág. A hányados/maradék u32 típusa explicit;
csomagolás unsigned32 marad. Két dokumentált típusadaptáció, nem C++20 UB-mérés.
Az eredeti N az osztó7. bitjét követi, nem a15.-et; hardvermérés nélkül ezt megőrizzük.
524288 osztó-söprés,30720 alias/CCR-eset,20000 véletlen pár,241 szétvágás PASS;
200000 biztonságosan korlátozott referenciaszelet nulla eltéréssel. Nagy értékek
független64 bites számítással ellenőrizve; hányados+1 kontroll seed9/slice26.
Jelölt:61 CTest/87 Python,492 upstreamből származó kezelő (488 változatlan,4 adaptált);
237 kiválasztott/27 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve szó-osztásra;
távoli CI függőben, M0 továbbra is részleges. [Szerződés](h8-word-divide.md).
Yamaha-adat nem került be; firmware/MIDI/hang/plugin elfogadás nincs.

## 2026-10-29 — M0 decimális korrekciós részlépés: DAA/DAS jelölt

Négy változatlan eredeti full/partial kezelő, külön0f00/fff0 és1f00/fff0 dekóderág.
Független tízes számrendszerű számítás: minden0–99 operanduspár összeadás/kivonás,
két CCR-minta (40000 eset);2048 regiszterhatáreset és241 szétvágott futás PASS.
Érvénytelen BCD/flag kombinációkra nem állítunk teljes független helyességbizonyítást.
200000 eredeti referenciaszelet nulla eltéréssel, mindkét új forma és korábbi családok
elérése kötelező; DAA korrekció+1 kontroll seed7/slice0 felismerve. Jelölt:62 CTest/87 Python,
496 upstreamből származó kezelő (492 változatlan,4 korábbi adaptáció);239 kiválasztott/
25 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve. M0 továbbra is részleges;
teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. [Szerződés](h8-decimal-adjust.md).
Yamaha-adat nem került be.

## 2026-10-29 — M0 előjeles bájtos osztás jelölt

Változatlan eredeti01d0 prefix és DIVXS.B kezelők, őrzött0xf0000 folytatás.
Független64 bites számítás:393216 osztandó-söprés,2048 osztóhatáreset,36864 alias/CCR,
20000 véletlen pár,241 szétvágás és65536 prefixfolytatás PASS. A hányados nullához
csonkolódik,maradék az osztandó előjelét követi; N a teljes hányados előjele,Z a nullás
osztó. Eredeti kommentek helyett a matematikát és futó kódot ellenőrizzük; hardverigazolás nincs.
200000 referenciaszelet nulla eltéréssel; hányados+1 kontroll seed7/slice3 felismerve.
Jelölt:63 CTest/87 Python,500 upstream-kezelő (496 változatlan,4 korábbi adaptáció),
240 kiválasztott/24 hiányzó/135 kizárt sor. Linux UBSan kiterjesztve; CI függőben.
Szó-osztás INT32_MIN/-1 és csomagolás külön nyitott vizsgálat. [Szerződés](h8-signed-byte-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Yamaha-adat nem került be.

## 2026-10-29 — M0 előjeles szó-osztás és szélső hányados jelölt

Két változatlan eredeti DIVXS.W kezelő, őrzött meglévő01d0 folytatás;32 bites int
fordítási feltétel. Az eredeti unsigned32 segédértékek elkerülik a közvetlen signed
INT_MIN/-1 osztást. A matematikai +2147483648 hányadost az eredeti int32-re szűkíti;
N=1,csomagolt eredmény0,Z=0. Ez forráskompatibilitás,nem hardveres hibajavítás vagy mérés.
524288 osztó-söprés,21504 alias/CCR,20000 véletlen pár,1024 szélső/CCR-eset,321
szétvágás és128 tiltott ER-szomszéd PASS. Független64 bites számítás és külön
szűkítési flagmodell.200000 eredeti referenciaszelet nulla eltéréssel; fordított
N kontroll seed7/slice7 felismerve. Jelölt:64 CTest/87 Python,502 upstream-kezelő
(498 változatlan,4 korábbi adaptáció),241 kiválasztott/23 hiányzó/135 kizárt sor.
Linux UBSan kiterjesztve; CI függőben. [Hatókör és szélső eredmény](h8-signed-word-divide.md).
M0 részleges; teljes gép/firmware/MIDI/hang/plugin elfogadás nyitott. Yamaha-adat nincs.

## 2026-10-29 — M0 32 bites eltolásos bájt/szó-memóriamozgatás jelölt

Eredeti7800 prefix és négy MOV-csoport,tíz változatlan full/partial kezelő.
Meglévő négyszavas IR,őrzött160000 folytatás,full32 cím és külön24 bites fizikai
leképezés.1048576 eltolásfél-söprés,55296 cím/regiszter/CCR-határeset,241 szétvágás,
65536 prefixfolytatás és memóriahibás fail-stop tesztek PASS. Nem teljes2^32 keresztszorzat.
A prefixkontroll nyolc hibás belső-állapot újradekódolást fogott; az elsőszó-maszk
szűkítése rendezte.200000 eredeti referenciaszelet nulla eltéréssel; felső eltolásszó
elhagyása kontroll seed6/slice5 felismerve. Jelölt:65 CTest/87 Python,512 upstream-kezelő
(508 változatlan,4 korábbi adaptáció),245 kiválasztott/19 hiányzó/135 kizárt sor.
Hosszú szó32-eltolással továbbra is nyitott. Linux UBSan kiterjesztve; CI függőben.
[Szerződés és hatókör](h8-displacement32-memory.md). M0 részleges; firmware/MIDI/hang/plugin
elfogadás nincs. Yamaha-adat nem került be.
