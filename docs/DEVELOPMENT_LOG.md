# YamAN1x fejlesztési napló

Frissítve: 2026-10-28. Időzóna: Europe/Budapest.
Ellenőrzött alap: `88e88bd8551f78bc312230d459ea3c92d7b66f01` (#52); a carry/regiszter/logika jelölt alább.

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

Aktív carry/regiszter/logika fejlesztés: [PR #51](https://github.com/RobCZart82/yaman1x/pull/51), `feat/h8-carry-register-logic`. Helyben 32 CTest-csoport és 83 Python-eset PASS; egy symlink-jogosultsági eset kihagyva. A végső head teljes CI-je külön ellenőrzendő; M0 részben teljesült.

## H8-WIDE-LOGIC-2026-10-28 — széles regiszteres logika

Előző lépcső lezárása: [#51](https://github.com/RobCZart82/yaman1x/pull/51), merge `ba2c91d8a156972bcf3c7b9adab98e36f7279b0c`, végső head `931a5a50dfe57c575d45d9d8b6b817889501a5a0`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821739), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821601), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40656821621). A forrásrevízió (`df6ce5574346e32b57a81dffb5c5b5f0e7929bbf`) automatikus review-ja kész, nincs találat; a követő commit dokumentációs PR-metaadat. A korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-wide-register-logic`, alap: beolvadt #51. 138 eredeti generált full/partial kezelő: word/long regiszteres OR/XOR/AND és eredeti 01f0 prefix. 196608 word-eset, 245760 regiszter/CCR-kombináció, 65536 prefixkiterjesztés és 1000 felosztás PASS. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; XOR helyett OR hibát a kontroll észleli. [Szerződés és korlátok](h8-wide-register-logic.md). Helyben 33 natív CTest-csoport PASS; a teljes 34 csoport / 84 Python-eset végső CI-je külön ellenőrizendő. M0 részben teljesült; shift/rotate, bit, stack/IRQ és CPU/SCI-integráció nyitott.

Aktív széles regiszteres logika: [PR #52](https://github.com/RobCZart82/yaman1x/pull/52), `feat/h8-wide-register-logic`. Helyi 33 natív csoport / 83 Python-eset PASS; a teljes végső head CI-je külön ellenőrizendő.

## H8-SHIFT-ROTATE-2026-10-28 — egybites eltolás és forgatás

Előző lépcső lezárása: [#52](https://github.com/RobCZart82/yaman1x/pull/52), merge `88e88bd8551f78bc312230d459ea3c92d7b66f01`, végső head `eac6be6a95e22f1266730c6941efa0c766658913`; [Linux/macOS/Windows PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124816), [MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124527), [MAME PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40660124813). A `c726582b72c43bc87cc917f1ff60c10f762a768a` forrásrevízió review-ja kész, nincs találat; a későbbi dekódersorrend-védelmet célzott és eredeti kódos teszt igazolja. Korábbi pending-CI bejegyzések történeti állapotok.

Ág: `feat/h8-shift-rotate`, alap: beolvadt #52. 186 eredeti generált full/partial kezelő; SHLL/SHAL/SHLR/SHAR/ROTXL/ROTL/ROTXR/ROTR 8/16/32 biten. Független bitenkénti orákulum, signed-range overflow, 1744896 operandus/CCR/regisztersáv-eset, 1000 felosztás. Eredeti H8H-kóddal 200000 szakasz, nulla eltérés; a megerősített explicit carry-program a kihagyott ROTXR carry-t seed 2/slice 8 helyen észleli. [Szerződés és korlátok](h8-shift-rotate.md). M0 részben teljesült; kétbites változatok, bitműveletek, stack/IRQ és CPU/SCI-integráció nyitott. A jelölt teljes CI-je külön ellenőrizendő.
