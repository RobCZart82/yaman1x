# YamAN1x fejlesztési napló

Frissítve: 2026-10-29. Időzóna: Europe/Budapest.
Ellenőrzött motoralap: `2dab39c1598533fc0a84da9017b3f2975854ff96` (#102).

## Aktuális lezárás — #102, 2026-10-29

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
állapotot őrzik; a jelenlegi készültségre ez a lezárás és az aktuális mérföldkő-tábla
irányadó. A dokumentáció mostani javítása nem új motorfejlesztés vagy elfogadás.

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

## A nyilvántartás feladata

Ez a repository-szintű változásjegyzék a fejlesztéseket, javításokat és
kapcsolódó bizonyítékokat köti a PR-ekhez. A részletes mérföldkő-elfogadás és a
PR #16–#36 végső CI-revízióinak táblája továbbra is a
[REAPER-tervben](FIRST_REAPER_SOUND_PLAN.md) marad. Az aktuális készültséget a
[STATUS](STATUS.md), az audit megállapításait a
[2026-10-28-as jelentés](REPOSITORY_AUDIT_2026-10-28.md) tartalmazza.

A korábbi napló tehát már létezett a tervben; ez a dokumentum teljesebb,
külön megtalálható könyvelést ad, és nem nyilvánít késznek nyitott mérföldkövet.
A [strukturált nyilvántartás](development-ledger.json) tartalmazza a teljes
merge- és PR-head SHA-kat, valamint az eredeti UTC-időbélyegeket.

## AUD-2026-10-27 — repository-audit és javítások

Állapot: [#38 PR](https://github.com/RobCZart82/yaman1x/pull/38) beolvadt 2026-10-27 15:36:15 UTC-kor; merge: `47950097b564f7ca48a1a2fd27a125066c3fc7a9`.

A helyben tesztelt kódfa a `de892c3ea278611d1a60e68ccbad8991e174b768`
GitHub-revízió fájlfájával azonos. A követő változás csak e napló metaadatait
rögzíti; a legutóbbi PR-head saját CI-je az irányadó.

- Átvizsgált terület: build/CI, saját C++/Python kód és tesztek, kutatási patch-lánc,
  forrás-/licencleltár, állapotdokumentumok és PR-előzmények.
- Javítás: az élő bridge auditor elfogadja a jelenlegi register-only ABI 2 fejlécet,
  megtartja a régi ABI 1/2 formátumot, és elutasítja az ellentmondó órajeladatot.
- Regresszió: az új teszt a javítás előtt hibázott; utána négy célzott teszt
  normál és `python -O` módban is PASS. Yamaha-adat nem kellett hozzá.
- Dokumentáció: elavult DSP-időléptetési, tesztszám- és változatlan upstream
  forrásra vonatkozó állítások javítva; aktuális és történeti eredmények jelölve.
- Ellenőrzés: tiszta macOS arm64 Release build; alapállapot 22/22 CTest és
  56 Python-eset PASS; a jelölt négy új Python-esetet ad ugyanazon csoporthoz.
  A részletes, végső eredmény az auditjelentésben és a PR saját checkjeiben van.
- Nyitott: SCI IRQ/MMIO és H8-ütemező/CPU-bekötés; firmware-es PKS/MIDI,
  AN1x órajel/EG/táblacímzés, audio és REAPER-elfogadás.

## SCI-MMIO-2026-10-27 — kétcsatornás SCI busz/IRQ-adapter

Állapot: [#39 PR](https://github.com/RobCZart82/yaman1x/pull/39) beolvadt 2026-10-27 18:06:08 UTC-kor; merge: `6c1dd8a0c83eac1d593abd4e70c040f88908bb48`.
A hatregiszteres bájt/szó MMIO és a csatornánkénti ERI/RXI/TXI/TEI útvonal elkészült, DMA-elsőbbséggel. Szintetikus 512 RX bájt/csatorna eset, TX, hibák, reset és izoláció ellenőrizve. Az eredeti PR-head öt platform/build ellenőrzése sikeres volt; a #38 főági dokumentációjával feloldott ütközés után új CI szükséges. A kombinált fa helyben 23/23 CTest-csoporttal és 60 Python-esettel PASS. M0 részben teljesült; CPU-ütemezés és firmware/audio továbbra is nyitott.

## #45 dokumentációs lezárás

Beolvadt 2026-10-27 21:58:12 UTC: `ed966cef79eb33eb309165e9e9d6edca920410ea`; head `eee27cef236fb54cb0d012f1df1716d2d03fa63c`. [Három platform PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40589542410), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40589542442), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40589542551). A strukturált leltár #45-öt is tartalmazza; az alábbi régebbi táblázat #44-ig történeti index.

## Beolvadt változások — a legutóbbi elöl

A következő 39 bejegyzés GitHub PR-metaadatból és az aktuális `main` Git-előzményéből
ellenőrzött visszamenőleges könyvelés (#6–#44). A beolvadás bizonyított; a
felsorolás önmagában nem állít minden PR-re újrafuttatott hardveres vagy helyi
validációt. A kezdeti importot a [történeti összefoglaló](HISTORY.md) őrzi.

| Beolvasztás (Budapest) | PR | Fejlesztés / javítás | Merge commit |
|---|---|---|---|
| 2026-10-27 22:43:08 | [#44](https://github.com/RobCZart82/yaman1x/pull/44) | H8 ciklustartozás és perifériahatáridők | `b644b6e1c471f8632ea086d75bd37d20b5ebbeb4` |
| 2026-10-27 21:49:51 | [#43](https://github.com/RobCZart82/yaman1x/pull/43) | H8H buszsegédek és eredetimetódus-összevetés | `f8b1f4fbd4a422eab33d633e2384f1c82c11623d` |
| 2026-10-27 21:33:08 | [#42](https://github.com/RobCZart82/yaman1x/pull/42) | H8 CPU gazdaszolgáltatás-határ és reprodukálható leltár | `e3f71a89f6d967034a3c05926a7a86c595ab9a49` |
| 2026-10-27 20:47:56 | [#41](https://github.com/RobCZart82/yaman1x/pull/41) | Kétcsatornás SCI-idővonal | `259338dbffed8b5b10b606c5f8f907bd791b1b74` |
| 2026-10-27 19:56:09 | [#40](https://github.com/RobCZart82/yaman1x/pull/40) | Korlátozott ciklus-eseményfuttató | `76787a6ee5450fa29a7403b658fc861ef908a297` |
| 2026-10-27 19:06:08 | [#39](https://github.com/RobCZart82/yaman1x/pull/39) | Két SCI-csatorna MMIO/IRQ-adaptere | `6c1dd8a0c83eac1d593abd4e70c040f88908bb48` |
| 2026-10-27 16:36:15 | [#38](https://github.com/RobCZart82/yaman1x/pull/38) | Repository-audit, bridge-kompatibilitási javítás és fejlesztési napló | `47950097b564f7ca48a1a2fd27a125066c3fc7a9` |
| 2026-10-27 13:32 | [#37](https://github.com/RobCZart82/yaman1x/pull/37) | Reconcile development plan with verified PR closures and historical snapshots | `276399870400` |
| 2026-10-27 13:10 | [#36](https://github.com/RobCZart82/yaman1x/pull/36) | Unify asynchronous H8 SCI transmit and receive under one channel | `33dc7e97c174` |
| 2026-10-27 12:22 | [#35](https://github.com/RobCZart82/yaman1x/pull/35) | Compose H8 asynchronous SCI receiver with sampled input and errors | `e7257ebe2bbc` |
| 2026-10-27 11:59 | [#34](https://github.com/RobCZart82/yaman1x/pull/34) | Compose H8 asynchronous SCI transmitter with status and cycle clock | `389594251b9b` |
| 2026-10-27 11:43 | [#33](https://github.com/RobCZart82/yaman1x/pull/33) | Update LICENSE | `6e5da15024e3` |
| 2026-10-27 11:27 | [#32](https://github.com/RobCZart82/yaman1x/pull/32) | Track reviewed third-party license coverage and enforce source notices | `cded711258a3` |
| 2026-10-27 11:00 | [#31](https://github.com/RobCZart82/yaman1x/pull/31) | Require replaceable PKS backends and a versioned firmware upgrade path | `e1bb9206b024` |
| 2026-10-27 10:56 | [#30](https://github.com/RobCZart82/yaman1x/pull/30) | Record PKS replacement ideas as unverified research options | `bc946bd39d2b` |
| 2026-10-27 10:47 | [#29](https://github.com/RobCZart82/yaman1x/pull/29) | Extract H8 SCI status latch with exhaustive flag transition checks | `3890122ffddf` |
| 2026-10-27 10:11 | [#28](https://github.com/RobCZart82/yaman1x/pull/28) | Extract internal H8 SCI cycle clock and refresh milestone plan | `019070d95f82` |
| 2026-10-27 09:46 | [#27](https://github.com/RobCZart82/yaman1x/pull/27) | Adapt H8H IRQ registers and pins preserving upstream publication | `3ec574095dfa` |
| 2026-10-27 09:09 | [#26](https://github.com/RobCZart82/yaman1x/pull/26) | Extract standalone H8H internal interrupt arbitration | `3a6cf4832025` |
| 2026-10-27 08:48 | [#25](https://github.com/RobCZart82/yaman1x/pull/25) | Record completed H8 peripheral CI evidence | `26dce3d3ceff` |
| 2026-10-27 08:44 | [#24](https://github.com/RobCZart82/yaman1x/pull/24) | Prepare H8 source boundary and extract standalone digital port | `14635883d5e3` |
| 2026-10-27 08:15 | [#23](https://github.com/RobCZart82/yaman1x/pull/23) | Prepare paired local M1 serial experiment for Mac continuation | `54757fbcb340` |
| 2026-10-27 07:35 | [#22](https://github.com/RobCZart82/yaman1x/pull/22) | Initialize VOP3 decoded operations before the first comparison | `94482f2e05a9` |
| 2026-10-27 07:08 | [#21](https://github.com/RobCZart82/yaman1x/pull/21) | Record verified MAME build evidence and remaining M1 acceptance | `c42c6f244631` |
| 2026-10-27 07:02 | [#20](https://github.com/RobCZart82/yaman1x/pull/20) | Pin MAME research source and compile the full patched target in CI | `4bca07273afa` |
| 2026-10-27 06:32 | [#19](https://github.com/RobCZart82/yaman1x/pull/19) | Add timestamped SCI access observation for M1 startup research | `291e04554c59` |
| 2026-10-27 06:23 | [#18](https://github.com/RobCZart82/yaman1x/pull/18) | Track development plan progress with dated validation evidence | `064ddbe56dd7` |
| 2026-10-27 06:19 | [#17](https://github.com/RobCZart82/yaman1x/pull/17) | Record M0 engine architecture and dependency baseline | `865e47930497` |
| 2026-10-27 06:17 | [#16](https://github.com/RobCZart82/yaman1x/pull/16) | Keep ROM-free ABI checks active under Python -O | `d70a309ca08a` |
| 2026-10-27 05:18 | [#15](https://github.com/RobCZart82/yaman1x/pull/15) | Plan development through the first audible voice in REAPER | `d24237292908` |
| 2026-10-27 00:03 | [#14](https://github.com/RobCZart82/yaman1x/pull/14) | Observe both H8 SCI devices and keyboard-port traffic | `1f9163c2866e` |
| 2026-10-26 23:53 | [#13](https://github.com/RobCZart82/yaman1x/pull/13) | Audit MIDI input at H8 SCI0 | `216af8b06070` |
| 2026-10-26 23:38 | [#12](https://github.com/RobCZart82/yaman1x/pull/12) | Route MAME MIDI input to H8 SCI0 | `db9028297e05` |
| 2026-10-26 23:29 | [#11](https://github.com/RobCZart82/yaman1x/pull/11) | Add opt-in timed H8 VOP3 experiment | `b735bd3e8cc6` |
| 2026-10-26 23:02 | [#10](https://github.com/RobCZart82/yaman1x/pull/10) | Document AN1x table-ROM findings and audit tool | `f19674cebede` |
| 2026-10-26 22:34 | [#9](https://github.com/RobCZart82/yaman1x/pull/9) | Add ROM-free cross-platform CI | `cdfce265590e` |
| 2026-10-26 22:26 | [#8](https://github.com/RobCZart82/yaman1x/pull/8) | Align branch ruleset docs with zero required approvals | `f79779625139` |
| 2026-10-26 21:29 | [#7](https://github.com/RobCZart82/yaman1x/pull/7) | Record public research release and active main-branch protection | `9978fb71b0ea` |
| 2026-10-26 20:54 | [#6](https://github.com/RobCZart82/yaman1x/pull/6) | Prepare main-branch protection ruleset; activation requires eligible plan | `89e54b953089` |

## Következő bejegyzések vezetése

Minden új fejlesztés és javítás saját dátumozott bejegyzést kap ugyanabban a
PR-ben. Kötelező adatok: probléma/cél, változás, érintett mérföldkő, PR vagy
commit, ellenőrzések és platform, maradó korlát, következő feladat. A PASS,
FAIL, NOT RUN és környezeti akadály külön szerepeljen. A végső merge SHA a
beolvasztás utáni egyeztetéskor kerül be; ismeretlen adat maradjon függőben.

A részletes vezetési szabály: [CONTRIBUTING](../CONTRIBUTING.md). A PR-sablon
emlékeztet a frissítésre; ez nem automatikus naplózási szolgáltatás. A történeti
adatokat megőrizzük, a téves következtetést új, indokolt korrekcióval javítjuk.

## SCHED-2026-10-27 — korlátozott ciklus-eseményfuttató

PR: [#40](https://github.com/RobCZart82/yaman1x/pull/40), ág: `feat/bounded-cycle-dispatcher`; [szerződés](cycle-dispatcher.md). Új, saját komponens, nem upstream átvétel. Költségkeret, folytatás, időmonotonitás, hibahatár és izoláció tesztelve; 1000 szintetikus eseménysor felosztásfüggetlen. Külön PR a #39-től; annak státuszát nem zárja le. A teljes CPU/periféria-ütemezés és M0 elfogadása nyitott.

Helyi záróellenőrzés: macOS arm64 Release, 23/23 CTest-csoport és 60 Python-eset PASS; natív ASan/UBSan PASS. Végső platform-CI a PR-head checkjeiben.

2026-10-27, #39 utáni szinkronizálás: a build- és naplóütközések feloldva, mindkét komponens megtartva. Az egyesített kódfa helyben 24/24 CTest-csoporttal és 60 Python-esettel PASS. Az előző 23-as eredmény a különálló ág történeti bizonyítéka. Új GitHub CI a szinkronizált #40-headen szükséges.

## SCI-TIMELINE-2026-10-27 — közös SCI-eseménytulajdonos

PR: [#41](https://github.com/RobCZart82/yaman1x/pull/41), ág: `feat/sci-cycle-timeline`, alap: beolvadt #40. Saját új komponens, új upstream átvétel nélkül. [Részletes szerződés és határok](sci-cycle-timeline.md). Időbélyegzett bemenet, két csatorna, eseményenkénti keret és determinisztikus sorrend; 256 teljes duplex minta három futtatási felosztása egyezik. CPU-végrehajtás és M0 lezárása nyitott.

Helyi ellenőrzés: macOS arm64 Release, 25/25 CTest-csoport és 60 Python-eset PASS; az új natív teszt ASan/UBSan alatt is PASS. Végső platform-CI a kapcsolódó PR-headhez kötve ellenőrizendő.

## H8-HOST-2026-10-27 — CPU gazdaszolgáltatás-határ

PR: [#42](https://github.com/RobCZart82/yaman1x/pull/42), ág: `feat/h8-host-services-boundary`, alap: beolvadt #41. Új ROM-mentes elemző és [áttekintett integrációs szerződés](h8-host-services-boundary.md). 34 forrás-/generált fájl, hash-ek és tokenhelyek, két független bájtazonos regenerálás. Nyolc új Python-regresszió; CPU-fordítás/végrehajtás és függőségzár továbbra is nyitott.

Local validation / helyi ellenőrzés: macOS arm64 Release, 25/25 CTest groups and 68 Python cases PASS; source/notice audits PASS. Final PR-head platform CI pending.

## H8H-BUS-2026-10-27 — CPU buszsegédek

PR: [#43](https://github.com/RobCZart82/yaman1x/pull/43), ág: `feat/h8h-bus-access`, alap: beolvadt #42. [Szerződés és bizonyíték](h8h-bus-access.md). BSD-forráseredet rögzítve, licenclefedettség frissítve. Öt önálló buszsegéd; 200000 műveletes eredetimetódus-összevetés 0 eltéréssel, 65536 szóérték és hibahatárok. CPU-futtató, firmware és hang továbbra is nyitott.

Local validation / helyi ellenőrzés: macOS arm64 Release, 26/26 CTest groups and 72 Python cases PASS; new native ASan/UBSan PASS. Final PR-head platform CI remains separate evidence.

## H8-BUDGET-2026-10-27 — végrehajtási keret számtana

PR: [#44](https://github.com/RobCZart82/yaman1x/pull/44), ág: `feat/h8-execution-budget`, alap: beolvadt #43. [Szerződés és bizonyíték](h8-execution-budget.md). Ciklustartozás, határidő és stabil CPU-counter a buszsegédhez; 1000 felosztási történet és 400000 eredetimetódus-összevetés 0 eltéréssel. Licenc- és eredetnyilvántartás frissítve. Teljes CPU-utasításvégrehajtás, firmware és hang nincs.

Local validation / helyi ellenőrzés: macOS arm64 Release, 27/27 CTest groups and 74 Python cases PASS; new native ASan/UBSan and source/notice audits PASS. Final PR-head CI pending.

## H8-RUNNER-2026-10-28 — végrehajtási sorrend

PR: [#46](https://github.com/RobCZart82/yaman1x/pull/46). Ág: `feat/h8-execution-order`, alap: #45. Korabeli jelöltállapot: H8 full/partial/periféria-sorrend a meglévő cikluskeret fölött, nem teljes CPU. 200000 eseményes referencia-szelet: 338598 update, 761211 full, 276336 partial callback, nulla eltérés. Natív 1000 felosztás és hibahatárok; két új Python regresszió. A Windows helyi teljes Python-csoport symlink-jogosultsági korlátját külön kezeljük; végső PR-head CI szükséges. Következő: tényleges generált kezelők és CPU-regiszterállapot. M0 részben teljesült.

## H8-FIRST-2026-10-28 — első generált utasítások

Előző lépcső lezárása: [#46](https://github.com/RobCZart82/yaman1x/pull/46): merge `00e16ce805d542e4a654923ddde2d462ebac78bb`, végső head `478294b0570b0140b0ba2c5f78b65fcc99fecaa4`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467656), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467504), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40637467588).

PR: [#47](https://github.com/RobCZart82/yaman1x/pull/47). Ág: `feat/h8-first-generated-handlers`, alap: beolvadt #46. Korabeli jelöltállapot: nyolc eredeti generált full/partial kezelő (reset, NOP, MOV.B immediate, BRA rel8) fordul és fut saját CPU-állapottal a busz-/cikluskereten. Az összes többi opcode explicit hiba. 4096 regiszter/érték, 65536 jelzőbit-eset, 256 elágazási eltolás és 1000 futásfelosztás tesztelve. Eredeti H8H-dekóderrel/kezelőkkel/futtatóval 200000 szakasz és 899400 olvasás, nulla eltérés; szándékos regiszterhibát a kontroll észleli. Hat új Python regresszió. Részletek: [komponensszerződés](h8-first-instructions.md). Teljes CPU, IRQ/SCI integráció és firmware/audio nincs; M0 részben teljesült. A jelölt végső PR-head CI-je külön ellenőrizendő.

## H8-MEMORY-BRANCH-2026-10-28 — adatmemória és feltételes elágazások

Előző lépcső lezárása: [#47](https://github.com/RobCZart82/yaman1x/pull/47): merge `343f4c32c81229e89ec092b0d8e324f2a0305f8a`, végső head `6f8107ec35e511c6aa9d347295e67a882a7826d3`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640131062), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130940), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40640130959).

PR: [#48](https://github.com/RobCZart82/yaman1x/pull/48). Ág: `feat/h8-memory-conditional-branches`, alap: beolvadt #47. Korabeli jelöltállapot: 44 eredeti generált full/partial kezelő; MOV.L immediate32, indirekt bájtolvasás/-írás és mind a 16 rel8 feltétel. 65536 elágazási és 65536 dekódereset, 32768–32768 load/store eset, 1000 felosztás; két új Python regresszió. Eredeti H8H-kóddal 200000 szakasz, 844255 utasítás-, 27571 adatolvasás, 27574 adatírás: nulla eltérés; szándékos adatírási hibát a kontroll észleli. [Szerződés és korlátok](h8-memory-branches.md). M0 részben teljesült; IRQ/SCI CPU-bekötés, teljes CPU és firmware/audio továbbra is nyitott. A végső PR-head CI-je külön ellenőrizendő.

## H8-WORD-LONG-2026-10-28 — szó- és hosszúszó-átvitel

Előző lépcső lezárása: [#48](https://github.com/RobCZart82/yaman1x/pull/48): merge `800f8b4eabfe484b54e9918a652096c0acf50986`, végső head `a2da3e66797ec5d9f617e3e85b1e6505ccf26beb`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345253), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345183), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40644345179). Az automatikus review a beolvasztáskor még futott; nem lezárt eredményként szerepel. A beolvasztás a kézi forrásellenőrzés, eredeti kódos kontrollok és mind az öt sikeres CI alapján történt.

PR: [#49](https://github.com/RobCZart82/yaman1x/pull/49). Ág: `feat/h8-word-long-memory`, alap: beolvadt #48. Korabeli jelöltállapot: 56 eredeti generált full/partial kezelő; MOV.W immediate, indirekt word/long load/store és eredeti 0100 prefixállapot. Mind a 65536 szóérték, regiszter-/címátfedések, 65536 prefixkiterjesztés és 1000 felosztás ellenőrizve. Eredeti H8H-kóddal 200000 szakasz: 745100 utasításolvasás, 19304 bájtolvasás, 19310 bájtírás, 57833 szóolvasás, 57853 szóírás; nulla eltérés. Szándékos szó-bájtsorrendhibát a kontroll észleli. [Szerződés és korlátok](h8-word-long-memory.md). M0 részben teljesült; aritmetika, IRQ/SCI és teljes CPU/firmware/audio továbbra is nyitott. Végső PR-head CI külön ellenőrizendő.

## H8-ARITHMETIC-2026-10-28 — ADD/SUB/CMP és jelzőbitek

Előző lépcső lezárása: [#49](https://github.com/RobCZart82/yaman1x/pull/49): merge `4567d7bfe852100f5483a8104bcba81b50ae26bb`, végső head `f69503487de2fbbf3c575d81f54ca5a91e965f90`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971856), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971826), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40647971835). Az automatikus review a `5aff015d6157f166d9f26d0591e7b13d067cd171` forrásrevízión hibát nem jelzett; a követő commit csak PR-metaadatot rögzített a dokumentációban.

PR: [#50](https://github.com/RobCZart82/yaman1x/pull/50). Ág: `feat/h8-arithmetic-flags`, alap: beolvadt #49. Korabeli jelöltállapot: 78 eredeti generált full/partial kezelő; ADD/CMP byte immediate, ADD/SUB/CMP byte register és word/long immediate formák. 327680 byte-bemenetpár, 12288 regiszterátfedés, 100800 szélesebb eset és 1000 felosztás tesztelve. Matematikai overflow/borrow orákulum; CMP nem ír regisztert. Eredeti H8H-kóddal 200000 szakasz: 791089 utasításolvasás, 13555 byte-olvasás, 13557 byte-írás, 40593 word-olvasás és 40606 word-írás; nulla eltérés. Szándékos half-carry-hibát a kontroll észleli. [Szerződés és korlátok](h8-arithmetic-flags.md). M0 részben teljesült; további aritmetika/logika/shift, stack/IRQ és CPU/SCI-integráció nyitott. Végső PR-head CI külön ellenőrizendő.

## H8-CARRY-LOGIC-2026-10-28 — carry, regiszterek és logika

Előző lépcső lezárása: [#50](https://github.com/RobCZart82/yaman1x/pull/50): merge `0f8d3ae96eb32dcabb3a039f40e5c0fd0315a1b9`, végső head `4bbea3449f5d73f15031332b537919b493a5defc`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119562), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119555), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40652119545). Automatikus review a `ddc8d51221838e64c9e849101756c1aeca260dce` forrásrevízión: kész, nincs találat; a követő commit csak PR-metaadatot rögzített.

Ág: `feat/h8-carry-register-logic`, alap: beolvadt #50. Korabeli jelöltállapot: 124 eredeti full/partial kezelő; wide register ADD/SUB/CMP, byte ADDX/SUBX, byte register/immediate és wide immediate logika, LDC CCR. 1048576 carry/Z eset, 30720 wide register eset, 393216 byte logika, 12288 logikai átfedés, 36864 wide logika, 20000 négybyte-os lánc, 1000 felosztás. Eredeti kóddal 200000 szakasz, nulla eltérés; hibás Z-assertet a kontroll észleli. [Szerződés és korlátok](h8-carry-register-logic.md). M0 részben teljesült; LDC csak CCR-állapot, nem IRQ-elfogadás. Shift/rotate, bit, stack/IRQ és CPU/SCI-integráció nyitott. Végső PR-head CI külön ellenőrizendő.

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

## AUD-2026-10-28 — teljes repository-áttekintés

Ág: `audit/repository-2026-10-28`; átvizsgált dokumentációs főág: #58, `863d21abcde448d746b3b44c39a1eb61be455a15`. [Jelentés](REPOSITORY_AUDIT_2026-10-28.md). Helyi macOS: 39/39 CTest, 84/84 Python, 200000 eredetimetódus-szelet, célzott négy CPU ASan/UBSan-csoport PASS; forrás/licenc/napló/link ellenőrizve. Reprodukálható motorhiba nem jelentkezett. STATUS elavult „current 27” bekezdése javítva. A jelentés két CI-fejlesztési javaslatot és a nyitott CPU/firmware/audio határokat rögzíti; motorkód és mérföldkő-készültség változatlan.

## H8-COVERAGE-CCR-2026-10-28

Ág: `feat/h8-coverage-ccr`; alap: beolvadt #59. [Komponensjegyzőkönyv](h8-coverage-and-ccr.md). Reprodukálható 399 soros H8H-leltár és öt új CCR-család; 376 eredeti full/partial metódus, 196608 immediát CCR-eset, regiszterlane- és partíciópróbák. Eredeti kód 200000 szelet, 0 eltérés; ASan/UBSan PASS. A régi dekóderteszt 800 újonnan támogatott kódolással frissítve. M0 részben teljesült. A #59 saját négy ellenőrzése zöld volt, nyitott review-thread nélkül beolvasztva a felhasználó sorozatos merge-engedélye alapján.

## H8-REGISTER-MOV-2026-10-28

Ág: `feat/h8-register-transfers`; [jegyzőkönyv](h8-register-transfers.md). Bájt/szó/hosszúszó regiszterközi MOV, hat új eredeti full/partial metódus (382 összesen), 576 további elsőszavas kódolás. 147456 lane/alias/flag eset, 1000 felosztási történet; 41/41 helyi CTest, 87 Python, ASan/UBSan és 200000 eredetimetódus-szelet PASS. Szülő: #60; saját PR csak annak beolvasztása után, a főágra újraalapozva.

### Sorozatos lezárás — #60 beolvadt

#60 merge: `18f7b7fa1b708c67cf7ed0baa2109d5f73c750db`, 2026-10-28 15:04:13 UTC. Végső head `e8dee6975054c4143abe36b3b8731b7cea510cf4`; három platform, MSan és MAME mind PASS. A korábbi referenciafixture elérhetőségi hibája beolvadás előtt javítva. #61 a friss mainre újraalapozott, önálló CI-re kész regiszter-MOV jelölt; #62–#64 szülőfüggőség miatt draft.

## H8-SMALL-ARITHMETIC-2026-10-28

Ág: `feat/h8-small-arithmetic`; [jegyzőkönyv](h8-small-arithmetic.md). 16 új csoport, 32 generált metódus (414 összesen). INC/DEC N/Z/V frissítés H/C megőrzéssel, ADDS/SUBS teljes CCR-megőrzéssel. Teljes szó/bájt tartomány és hosszúszó-határok, 1000 felosztás; 200000 eredetimetódus-szelet. Szülője a regiszter-MOV ág; PR csak annak beolvadása és újraalapozás után.

### #61 lezárása, #62 előkészítése

#61 mind az öt ellenőrzése zöld, nyitott review-thread és változtatáskérés nélkül beolvasztva: `9cedcdf92d0330103fbeac24932c5c13e3924b3a`, ellenőrzött head `c36473985c685147867f9b56ecd250b9377d037a`. A korábbi piros macOS-futások futtatóhiány miatt szakadtak meg; az újrafuttatások sikeresek. #62 önálló változtatása a friss mainre újraalapozva; saját végső CI-je még külön kapu. #63–#66 továbbra is függőségi draft.

### #62 CI-indítás javítása

A főágra célzás és ready állapot önmagában nem indított workflow-t: az újraalapozott commit feltöltésekor a PR még a korábbi feature ágat célozta, a workflow-k pedig csak main célágra indulnak. Az élő Actions API az `1d0cd3d2df4a6dfcb319822ec5a034c1aece8bc0` headhez nulla futást mutatott. Ez a naplófrissítés a már mainre célzott PR szinkronizálásával indítja a szükséges ellenőrzéseket. A következő lépcsőknél a main célág beállítása után történjen a végső feltöltés. Motorkód változatlan; nincs CI- vagy merge-szabály megkerülés.

### #62 Windows-fordítási javítás

A `9222223a7c4c4c57c3d214b34e7af87eac4410f9` head Windows CI-je MSVC C1061 hibát jelzett: a 127 ágból álló utasításválasztó else-if lánc túllépte a fordító beágyazási korlátját. A legkülső lánc sorrendtartó, korai visszatéréses if ágakká alakult; minden kiválasztott kezelő után pontosan egy kilépés történik, így a kezelő által módosított állapotra nem történik újabb kiválasztás. A generált utasítástörzsek és kódolások változatlanok. A macOS normál feladata külön futtatóhiány miatt szakadt meg; a MAME macOS referencia-, Linux- és MSan-feladat sikeres volt. Az új head saját teljes CI-je továbbra is kötelező.

A lapos választó helyi ellenőrzése: 42/42 CTest, 87 Python-eset, 200000 eredeti dekóder/execute_run referenciaszelet nulla eltéréssel; forrás-/licenc-/feltöltési audit PASS. A Windows-fordító eredménye az új CI futásától függ, helyi macOS-eredményből nincs Windows PASS állítás.

## H8-UNARY-2026-10-28

Ág: `feat/h8-unary-extension`; [jegyzőkönyv](h8-unary-extension.md). NOT/NEG/EXTU/EXTS tíz csoportban, 20 eredeti metódus, 434 összesen. 43/43 helyi CTest, 87 Python, független flag/adat orákulum, ASan/UBSan PASS. Elérhető referenciaciklus, minden új családra bejárási őrfeltétellel; 200000 szelet 0 eltérés. Szülője az INC/DEC ág; a szülők merge/CI feltételei miatt egyelőre egymásra épülő jelölt, nem main-elfogadás.

### #62 lezárása, #63 előkészítése

#62 végső head `3e1652252342b0b58d31f9f475af2a6bf14cd909`: mind az öt CI PASS, nyitott review-thread és változtatáskérés nincs. Merge: `f0b37f3e10e02248b8c9d0f52b3a555ae944c8d5`, 2026-10-28 17:44:11 UTC. A GitHub új stack #67 miatt a szinkron merge API nem támogatott; a webes felület kizárólag #62-re indított aszinkron squash merge-je sikeres, API-val visszaellenőrizve. #63 önálló unáris változtatása a friss mainre újraalapozva, a Windows-javítás lapos választójának megőrzésével. Saját végső CI külön kapu; #64–#66 draft függőség marad.

#63 újraalapozás utáni helyi ellenőrzés: 43/43 CTest, 87 Python, 200000 eredetimetódus-szelet nulla eltéréssel, forrás-/licenc-/feltöltési audit PASS. A főágra célzás már megtörtént; a végső feltöltés indítja a PR saját CI-jét.

## H8-TRAPA-2026-10-28

Ág: `feat/h8-trapa-entry`; [jegyzőkönyv](h8-trapa-entry.md). Négy szinkron trapkódolás, 436 eredeti metódus. Rögzített reset-SYSCR, vektor8–11, stack-sorrend és observerhibák. 44/44 helyi CTest, 87 Python, ASan/UBSan és 200000 eredetimetódus-szelet PASS. A stackbe írt m_NPC viselkedése forrásazonosság, nem hardverbizonyíték. Külső IRQ/NMI külön következő lépés; szülője az unáris ág.

### #63 lezárása és a #64–#66 Windows-javítása

#63 a GitHub szerint beolvadt: `dfa7af9bc21c8fe2b91367ad88b91bd7a56ba498`, 2026-10-28 18:12:45 UTC, végső head `ff7fa98a801ad33e0976206606673c7db0b17eca`, öt zöld CI. #64 újraalapozva a friss mainre; a régi Windows C1061 hibát okozó választólánc helyett a már ellenőrzött sorrendtartó korai kilépések megőrizve. #65 és #66 ugyanebben a függőségi sorrendben kapja meg ezt a javítást. A generált metódustörzsek változatlanok; minden új head saját CI-je szükséges.

## H8-EXTERNAL-IRQ-2026-10-28

Ág: `feat/h8-external-irq`; [jegyzőkönyv](h8-external-irq.md). Két eredeti state_irq metódus (438 összesen), opcionális kontrollerkötés, rögzített reset-SYSCR CCR.I szűrés, normál/noirq prefetch különválasztása. 512 maszkolás/NMI/RTE eset, 322 felosztás, külső edge pin, stackhibák és példányizoláció. 45/45 CTest, 87 Python, ASan/UBSan és 200000 eredetimetódus-szelet PASS. Szülő #64; a merge kizárólag a szülők és saját végső CI miatt vár. #61 Linux/Windows/MSan zöld, két macOS-feladat még sorban áll; nem újrafuttatott hibák. Teljes gép- és audioelfogadás továbbra sincs.

#65 Windows-javítás: a #64 javított ágára újraalapozva, a régi mély else-if választó lecserélve sorrendtartó, azonnal kilépő ágakra. Új opcode vagy metódustörzs-változás nincs. A #64 függőség és a #65 saját végső CI-je továbbra is beolvasztási kapu.

### #64 lezárása, #65 aktuális jelölt

#64 beolvadt `ad1df979f17d7806d24f997f84d63fb087803675`, öt sikeres ellenőrzéssel, nyitott review-thread nélkül. A GitHub a #65 és #66 stackágakat automatikusan újraalapozta; #65 `d4fbd149d7141e05b47aee8262ce9973c9b5f2c8` teljes fájltartalma azonos a helyben validált `c091847` változattal. #65 már mainre céloz és review-ra kész; Windows/Linux/MSan zöld, két macOS-feladat vár. E naplófrissítés után az új head CI-je a merge-kapu. #66 még szülőfüggőség miatt draft.

## H8-SCI-CPU-RECEIVE-2026-10-28

Ág: `feat/h8-sci-cpu-receive`; [jegyzőkönyv](h8-sci-cpu-receive.md). Meglévő CPU/IRQ/SCI összekötése szintetikus teszthostban: 512 bájt/csatorna eset, 1602 felosztás; vezetéki él → RXI → ISR → RAM → RDRF-törlés → RTE. Új motorkód nincs. A host kizárólag előre konfigurált RX-et és időfüggetlen SSR-nyugtázást támogat; általános MMIO-írás időbélyeg és új határidő publikálása külön következő feladat. Szülő #65, merge/CI függőség miatt jelölt.

Helyi lezárás: 46/46 CTest és ASan/UBSan PASS; változatlan eszközök 87 Python-esete újra ellenőrizve. #60 beolvasztva `18f7b7fa1b708c67cf7ed0baa2109d5f73c750db` (öt zöld ellenőrzés), #61 saját macOS-feladatai még várakoznak. #65 az IRQ/NMI-jelölt PR-je.

#66 a javított #65 ágára újraalapozva; örökli a Windows-fordítási javítást. A három javított head közös teljes helyi tesztje és a legutolsó eredetimetódus-referenciája következik; a szülői merge-kapuk változatlanok.

A #64–#66 javított lánc helyi lezárása: 46/46 CTest, 87 Python-eset, 200000 eredeti dekóder/execute_run referenciaszelet nulla eltéréssel; forrás-/licenc-/feltöltési audit és diffellenőrzés PASS. #64 külön TRAPA-próbája 1024 eset/81 vágáspont mellett is sikeres. Windows PASS csak az új headek GitHub-fordítása után állítható; a régi piros headek történeti eredmények.

### #65 lezárása, #66 utolsó jelölt

#65 beolvadt `f59661ed79860460ba2989f6d2177554e0b56a05`, 2026-10-28 19:05:03 UTC, head `18f1fa382c8768e2424644464d7556170c9a8501`, öt sikeres CI, nyitott review-thread és változtatáskérés nélkül. #66 önálló fogadási tesztje a friss mainre újraalapozva; csak napló/ledger konfliktus volt. A végső head saját ellenőrzése még szükséges.

## H8-BUS-CYCLE-TIME-2026-10-28

Ág: `feat/h8-bus-cycle-time`, alap: #66 beolvadt `817e9ce91db22c770291fe2acf666f86f29e1896`, head `66687f2313c5824e629cff570aeffd365da71d73`, négy sikeres CI, nyitott review-thread nélkül. A korábbi #66 jelöltleírás történeti. [Új szerződés](h8-bus-cycle-time.md): opcionális bus_cycle időbélyeg minden kétciklusos buszművelet után, a memóriaművelet előtt, túlcsordulás- és hibakezeléssel. Öt művelettípus és 201 felosztás független ellenőrzése sikeres. Ez a CPU-vezérelt soros adás előfeltétele; az MMIO által létrehozott új határidő beillesztése külön következő lépés.

Helyi lezárás: 47/47 CTest, 87 Python, 200000 eredetimetódus-szelet nulla eltéréssel; új célzott teszt és ASan/UBSan PASS, forrás-/licenc-/feltöltési audit PASS. Hiányzó időalap és negatív abszolút idő külön visszautasítva. Saját végső CI/review még külön merge-kapu.

## H8-SCI-CPU-TX-2026-10-28

Ág: `feat/h8-sci-cpu-transmit`, szülő #68. [Szerződés](h8-sci-cpu-transmit.md): minden sikeres buszművelet után opcionális tiszta next_bus_event lekérdezés frissíti a futtató és az aktuális generált kezelő eseményhatárát. CPU által konfigurált SCI-adás két csatornán, összesen 512 bájteset és 1602 felosztás; start/adat/stop/végső idle, 32 ciklusos bitköz. Eredeti metódustörzsek változatlanok; teljes teszt/ref/sanitizer lezárás folyamatban, szülőmerge és saját végső CI külön kapu.

Helyi lezárás: 48/48 CTest, 87 Python, ASan/UBSan, 200000 eredetimetódus-szelet nulla eltéréssel; forrás-/licenc-/feltöltési audit PASS. #68 mind az öt CI-je zöld, nyitott review-thread/változtatáskérés nélkül beolvasztva: `0fa40aaba818a0efd7265a1861650479795cbba1`, head `7867ec70308e4e06ad794758e4f7c05008534f38`. A TX-jelölt saját végső CI-je külön kapu.

## H8-SCI-CPU-DUPLEX-2026-10-28

Ág: `feat/h8-sci-cpu-duplex`; [szerződés](h8-sci-cpu-duplex.md). Motorváltoztatás nélkül egyidejű TX/RX, mind a négy csatornapárosítás, 1024 bájteset és 3604 felosztás. Független wire/adat/IRQ/RDRF/RTE ellenőrzés; a frame-ek átfedik egymást, de aktív TX közbeni RXI-preempciót nem állítunk. Szülő #69 öt sikeres CI és üres review-thread/változtatáskérés lista után beolvadt: `298b8465aee4e74cfe7ec8e1459402b21f2ce9aa`, head `08a18b492a2c7df3abc5c33edac1f37a994a080f`. Teljes helyi teszt/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 49/49 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Negatív kontroll: RXI engedélyezés nélkül a teszt hibát jelez, tehát a RAM/IRQ elfogadás nem pusztán a TX befejezését ellenőrzi. Eredeti CPU-/UART-motorkód változatlan; a #69 eredetimetódus-referenciája továbbra is a motoralap bizonyítéka.

## H8-SCI-CPU-FER-2026-10-28

Ág: `feat/h8-sci-cpu-framing`; [jegyzőkönyv](h8-sci-cpu-framing.md). Motorváltoztatás nélküli ERI/FER-integráció: 1024 bájt/csatorna eset, 3604 felosztás, 512 következő jó frame helyreállítási eset. CPU SSR-kiolvasás/RAM/nyugtázás/RTE, majd RXI és helyes adat. A hiányzó idle-high stimulus mind512 recovery esetben hibát jelzett, a tesztbemenet javítva. #70 négy zöld CI, üres review-thread/változtatáskérés lista után beolvadt `860815eb62afb2d9b55e1e2cc8318a85967d4d04`, head `a063232d4f877fc2d3eea43303b73273a8e872eb`. Saját végső CI külön kapu; overrun/paritás még nyitott.

Helyi lezárás: 50/50 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Negatív kontroll: FER-törlés kihagyása 2048 elfogadási hibát eredményez, tehát a teszt a nyugtázást és recovery-t is ellenőrzi. A közös fixture kiemelése után a korábbi duplex teszt változatlan alapbeállításokkal továbbra is sikeres. Saját végső CI/review még szükséges.

## H8-SCI-CPU-PARITY-2026-10-28

Ág: `feat/h8-sci-cpu-parity`; [jegyzőkönyv](h8-sci-cpu-parity.md). SMR0x20/0x30, jó/rossz paritás mindkét csatornán, 2048 bájteset és 8808 felosztás. CPU RXI/ERI, RAM/SSR, PER-törlés és RTE; független bit-count bemenet és TX-paritás/időzítés. A rögzített forrás paritáspolaritását követjük, hardver-validált even/odd állítás nélkül. #71 négy zöld CI, nyitott review-thread/változtatáskérés nélkül beolvadt `8e9c431f1a0629cc20db136c50c56eba81854202`, head `e49e6c0053c8c5179e4475fc51ee3628478b9177`. Teljes helyi teszt/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 51/51 CTest, 87 Python, célzott ASan/UBSan, forrás-/licenc-/feltöltési audit PASS. PER-törlés nélküli negatív kontroll hibát jelez. A közös fixture alapértelmezett no-parity és FER-próbái továbbra is sikeresek. Motorkód változatlan; saját végső CI/review még szükséges.

## H8-SCI-CPU-OVERRUN-2026-10-28

Ág: `feat/h8-sci-cpu-overrun`, szülő #72. [Jegyzőkönyv](h8-sci-cpu-overrun.md). Két kiolvasatlan frame maszkolt CPU mellett, ORER+RDRF és első adat megőrzése; késleltetett LDC0 után CPU SSR/RDR mentés, törlés és RTE. A korábban latch-elt RXI az ERI után külön RTE-kezelőt kap; ez rögzített modellviselkedés, nem hardverbizonyíték. 512 bájteset, 4802 felosztás célzott PASS. #72 Linux/Windows/MSan zöld, macOS futtatóhiány miatt megszakadt, újraindítva. Az overrun PR a szülő CI/merge miatt visszatartandó, nem több fejlesztési lépcső miatt.

Helyi lezárás: 52/52 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Második frame nélküli negatív kontroll 2048 elfogadási hibát jelez. Motorkód változatlan. #72 macOS újrafutása folyamatban; az overrun jelölt szülőmerge és saját végső CI miatt draft.

#72 lezárása: macOS újrafutás sikeres, négy zöld CI, üres review-thread/változtatáskérés lista; merge `f29f0373be4dc6ca866744c2a0d832d82eb7dfa6`, head `dceda74cfa1402f050649b14b91de531aa8d9fdf`. #73 tisztán újraalapozva a mainre és review-ra kész; a korábbi draft/szülővárakozás bejegyzés történeti. A végső head saját CI-je még merge-kapu.

## H8-ABSOLUTE-MEMORY-2026-10-28

Ág: `feat/h8-absolute-memory`; [jegyzőkönyv](h8-absolute-memory.md). Hat abs8/abs16 MOV-csoport, 12 eredeti full/partial metódus (450 összesen), leltár217 kiválasztott/47 hiányzó/135 kizárt. Összes abs16 cím, abs8 regisztersáv/érték/CCR, 201 felosztás és hat buszhibahely; elérhető eredeti referenciaciklus mindhat formára fetch-őrrel. #73 négy zöld CI és üres review-thread/változtatáskérés lista után beolvadt `8446d10b65c698b5fbc749279f696d69c100e5fc`, head `c37346094ed6b35f068d8969a2401df40f010636`. Teljes helyi teszt/ref/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 53/53 CTest, 87 Python, célzott ASan/UBSan, forrás-/licenc-/feltöltési audit PASS. Eredeti dekóder/execute_run: 200000 szelet nulla eltéréssel, mindhat abszolút forma fetch-őre teljesült. A végső adatbuszhiba-tesztekkel az új cél külön újrafordítva és újrafuttatva. A 65536 elsőszavas dekóder-orákulum az újonnan támogatott abszolút kódolásokkal frissítve; szomszédos tiltott formák továbbra is elutasítva. Saját végső CI/review szükséges.

## H8-ABSOLUTE32-MEMORY-2026-10-28

Ág: `feat/h8-absolute32-memory`; [jegyzőkönyv](h8-absolute32-memory.md). Négy bájt/szó abs32 MOV-csoport, nyolc eredeti metódus (458 összesen), leltár221 kiválasztott/43 hiányzó/135 kizárt. 15 címhatár minden regisztersávval, formánként65536 korrelált32-bites címminta (nem teljes32-bites tartomány), 201 felosztás és négy buszhibahely. Külön elérhető eredeti referencia mindnégy fetch-őrrel. #74 öt zöld CI, nyitott review-thread/változtatáskérés nélkül beolvadt `758ad981253f8d3b5c72915f2fe5b60df560de97`, head `642977224868632203a46b1d220b926e506b0b5c`. Teljes helyi teszt/ref/sanitizer lezárás folyamatban; saját végső CI külön kapu.

Helyi lezárás: 54/54 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Eredeti dekóder/execute_run: 200000 szelet nulla eltéréssel; mindnégy abs32 fetch-őr és minden korábbi család elérhetőségi őre teljesült. Teljes32-bites tartomány helyett határpontokat és65536 mintát ellenőriztünk formánként. Saját végső CI/review szükséges.

## DOC-CLOSE-ABSOLUTE-MEMORY-2026-10-28

#75 lezárása az élő GitHub merge/head/időpont és öt sikeres ellenőrzés alapján rögzítve. A napló eleje, STATUS, REAPER-terv, README és strukturált ledger egységesítve; a korábbi jelöltleírások történetiek. A motoralap #75, nincs új motor-/firmware-/audioelfogadás. A dokumentációs PR saját CI-je külön követendő.

## H8-LONG-ABSOLUTE-MEMORY-2026-10-29

Ág: `feat/h8-long-absolute-memory`; [jegyzőkönyv](h8-long-absolute-memory.md). Négy abs16/abs32 MOV.L csoport, nyolc eredeti metódus (466 összesen),225 kiválasztott/39 hiányzó/135 kizárt sor. Négy szavas IR-puffer és snapshot/reference,0100 prefix ER-őrök; eredeti két szavas sorrend/átfordulás. Célzott összes abs16 cím, ER/CCR határpontok,201 felosztás és első/második írási hibák PASS. #76 dokumentációs PR még CI alatt; a mostani motorjelölt ettől külön kezelendő. Teljes teszt/ref/sanitizer lezárás folyamatban.

Helyi lezárás: 55/55 CTest, 87 Python, célzott ASan/UBSan és forrás-/licenc-/feltöltési audit PASS. Eredeti dekóder/execute_run:200000 szelet nulla eltéréssel, mindnégy long-abs fetch-őr és minden korábbi őr teljesült. A régi65536 prefixkiterjesztés-orákulum32 új támogatott kódolással frissítve; érvénytelen ER8..15 szomszédok továbbra is tiltottak. #76 macOS várakozik; az új motor-PR csak a szülőmerge és saját végső CI miatt draft.

#76 lezárva: négy sikeres CI, üres review-thread/változtatáskérés lista; merge `0c67fce0e32450709523e4ac01ae477a64633916`, head `ae753a6bc1418be4885645a38ae3fc757e7e293c`. #77 tisztán újraalapozva a mainre; korábbi szülővárakozása megszűnt. Saját végső CI/review még merge-kapu.

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
