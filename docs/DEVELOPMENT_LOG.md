# YamAN1x fejlesztési napló

Frissítve: 2026-10-27. Időzóna: Europe/Budapest.
Ellenőrzött alap: `76787a6ee5450fa29a7403b658fc861ef908a297` (#40).

## A nyilvántartás feladata

Ez a repository-szintű változásjegyzék a fejlesztéseket, javításokat és
kapcsolódó bizonyítékokat köti a PR-ekhez. A részletes mérföldkő-elfogadás és a
PR #16–#36 végső CI-revízióinak táblája továbbra is a
[REAPER-tervben](FIRST_REAPER_SOUND_PLAN.md) marad. Az aktuális készültséget a
[STATUS](STATUS.md), az audit megállapításait a
[2026-10-27-es jelentés](REPOSITORY_AUDIT_2026-10-27.md) tartalmazza.

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

## Beolvadt változások — a legutóbbi elöl

A következő 35 bejegyzés GitHub PR-metaadatból és az aktuális `main` Git-előzményéből
ellenőrzött visszamenőleges könyvelés (#6–#40). A beolvadás bizonyított; a
felsorolás önmagában nem állít minden PR-re újrafuttatott hardveres vagy helyi
validációt. A kezdeti importot a [történeti összefoglaló](HISTORY.md) őrzi.

| Beolvasztás (Budapest) | PR | Fejlesztés / javítás | Merge commit |
|---|---|---|---|
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

Ág: `feat/sci-cycle-timeline`, alap: beolvadt #40. Saját új komponens, új upstream átvétel nélkül. [Részletes szerződés és határok](sci-cycle-timeline.md). Időbélyegzett bemenet, két csatorna, eseményenkénti keret és determinisztikus sorrend; 256 teljes duplex minta három futtatási felosztása egyezik. CPU-végrehajtás és M0 lezárása nyitott.

Helyi ellenőrzés: macOS arm64 Release, 25/25 CTest-csoport és 60 Python-eset PASS; az új natív teszt ASan/UBSan alatt is PASS. Végső platform-CI a kapcsolódó PR-headhez kötve ellenőrizendő.
