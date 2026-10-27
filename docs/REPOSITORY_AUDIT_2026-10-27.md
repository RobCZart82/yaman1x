# Repository-vizsgálat — 2026-10-27

Vizsgált `main`: `276399870400bd81272c21889257984651c816ed`, beolvadt #37.
Az auditot követő jelölt javítások külön PR-ben kerülnek ellenőrzésre.
Az induló leltár 354 követett fájlt tartalmazott.

## Eredmény és az igazolás határa

A jelenlegi kódbázis működő, szintetikus tesztekkel ellenőrzött kutatási alap.
A tiszta helyi macOS arm64 build minden 22 CTest-csoportja és 56 Python-esete
sikeres volt. A tényleges termék továbbra sem kész H8-motor vagy hangszer:
M0 részben teljesült, M1 előkészített, M2–M6 nyitott. AN1x-hang, VST3/AU és
REAPER-elfogadás nincs igazolva.

A repository egészének leltárát, saját kódját, tesztfelületeit, build-/CI- és
kutatási határait vizsgáltuk. A külső 201 fájlon tartalomazonosságot és az egy
regisztrált VOP3-javítás visszafordíthatóságát ellenőriztük; ez nem a teljes
AsmJit/MAME forráscsalád új biztonsági vagy hardverhelyességi auditja.

## Átvizsgált területek

| Terület | Ellenőrzés és megállapítás |
|---|---|
| CMake és alap-CI | 22 tesztcsoport; három platformmátrix; az ABI-próbák normál és optimalizált Python alatt is futnak. Az alap-CI nem firmware-/audio-elfogadás. |
| Saját DSP-kód | Bridge ABI, argumentum-/maszkhatárok, egész mintaóra, kétchip-állapot, szintetikus soros próba és feltöltés-visszajátszás áttekintve, tesztelve. Táblarom-bekötés és audioút még hiányzik. |
| H8-segédek | GPIO, IRQ-választó és láb-/regiszteradapter, SCI ciklusóra/SSR és közös TX/RX tulajdonos áttekintve. A natív byte/format, hibajelzés, sorrend és izolációs tesztek PASS; teljes CPU-integráció nincs. |
| Python bemenet/trace | ROM/SYX/table/upload ellenőrzők, SCI/DSP stream és kísérletfuttatók áttekintve. A bridge audit formátumhibája reprodukálva és javítva, lásd F-01. |
| MAME-kutatás | A rögzített archívumból 11 patch előkészítése sikeres; 8 előállított fájl és injektált saját helper egyezik. A H8-generátor négy kimenete két független futásban egyezik a lockkal. |
| Forrás és licenc | 201 vendorizált fájl, egy visszafordítható helyi módosítás, 222 lefedett fájl és öt licencszöveg ellenőrzése PASS. Ez forrás-/notice-ellenőrzés, nem bináristerjesztési jóváhagyás. |
| Dokumentáció | Aktuális és történeti állítások összevetve a CMake-mel, patch-lánccal és PR-ekkel; elavult állítások javítva. A helyi Markdown-fájlhivatkozások feloldhatók. |
| Fejlesztési előzmény | A #6–#37 közötti 32 merge metaadata és commitja ellenőrizve. A részletes terv már tartalmazott dátumozott naplót és CI-lezárásokat; ezekhez külön központi index és vezetési szabály készült. |

## Konkrét hibák és javításuk

### F-01 — P2: a jelenlegi register-only trace téves elutasítása — javítva

Az előállított MAME-driver `BRIDGE state=live-register-only abi=2
hardware_validated=0` fejlécet ír, a kikapcsolt órajelet pedig külön
`DSP_SAMPLE_CLOCK profile=disabled rate_hz=0` sorban rögzíti. Az auditor
kizárólag a régi, `dsp_samples=0` mezőt tartalmazó formátumot fogadta el.
Így egy helyesen befejezett, mai register-only futás is elbukott a fejlécnél.

A szintetikus teljes capture-teszt a javítás előtt reprodukálta a hibát.
A javítás elfogadja az új formátumot a kikapcsolt órajel igazolásával, megőrzi
a régi ABI 1/2 felvételeket, és elutasítja a hiányzó/ellentmondó órajelet vagy
a register-only manifestben állított nemnulla mintaszámot. Négy regressziós
eset ellenőrzi az aktuális, régi, timed és hibás felvételeket normál és `-O`
módban. A DSP-modell és a MAME-patchek viselkedése ettől nem változik.

### F-02 — P2: több jelenidejű státuszállítás elavult — javítva

- README: tévesen azt állította, hogy a H8-futtató nem hív mintaléptetést.
  Most az alapértelmezett register-only és az opt-in timed profil külön szerepel.
- STATUS/HISTORY: a kilenc tesztcsoportos kezdeti snapshot aktuális állapotként
  jelent meg; a jelenlegi 22-es szám és a későbbi H8-komponensek bekerültek.
- STATUS: változatlan VOP3-snapshotot állított, miközben #22 óta egy dokumentált,
  visszafordítható konstruktorjavítás van benne. A leírás most ezt rögzíti.
- ROADMAP/research README: a már elkészült keresztplatformos CI és teljes
  macOS MAME-build még jövőbeli/hiányzó eredményként is szerepelt. Javítva;
  a friss firmware-/MIDI-elfogadás továbbra is nyitott.

A korabeli mérési jegyzőkönyveket és számokat nem írtuk át új eredménynek.

## Nyitott fejlesztési és bizonyítéki feladatok

| Azonosító | Sorrend | Következő konkrét eredmény |
|---|---|---|
| N-01 | M0 következő lépés | Két SCI-csatorna IRQ- és MMIO-bekötése a H8H-vezérlőhöz; közös eseményütemező, majd tényleges CPU-port és végrehajtási/izolációs próbák. |
| N-02 | M1 párhuzamos kutatás | A már elkészült Mac-kísérletfuttató friss helyi firmware-felvétele. SCI0 bájtfogadás és note-on/off feldolgozás igazolása, PKS-kézfogás bizonyítása. A 21 RXD-átmenet nem sikeres MIDI-elfogadás. |
| N-03 | M2–M3 | AN1x órajel, EG célon tartás, upload stop/run, táblacímzés és DMS-ROM-azonosság, soros száraz hangút. A 48 kHz továbbra is modellfeltevés. |
| N-04 | Reprodukálhatóság | A dokumentált nagy, ideiglenes upstream-összehasonlító fixture-ök nem mind részei a repository-nak. Ezek korábbi eredményeit az audit nem futtatta újra; tartós, licencelt, ROM-mentes reprodukciójuk későbbi erősítés. |
| N-05 | Naplóegyeztetés | A régi nyitott #2/#3 issue-leírások a timed profil előtti állapotot is tükrözik. A jelenlegi státusz a tervben/STATUS-ban van; issue-frissítéskor az eredeti hardveres elfogadási feltételeket meg kell tartani. |
| N-06 | M4–M6 és terjesztés | Offline referenciahang, határidőn belüli render, VST3/REAPER-próba és a ténylegesen terjesztett függőségek felülvizsgálata. A saját kód nyilvános terjesztési licence továbbra sincs kiválasztva. |

Az ismert upstream ICR-maszk- és EG-viselkedést nem módosítottuk pusztán
feltételezés alapján. A szintetikus tesztben megőrzött upstream-egyezés
nem azonos az AN1x hardverének validálásával.

## Friss ellenőrzési jegyzőkönyv

Platform: macOS arm64; AppleClang 21.0.0 (`clang-2100.3.34.2`), CMake 4.4.3,
Python 3.9.6; Release, explicit arm64 cél, új buildkönyvtár.

| Ellenőrzés | Eredmény |
|---|---|
| Vizsgált alapcommit tiszta configure/build/CTest | 22/22 PASS |
| Alapcommit Python unittest | 56/56 PASS |
| F-01 regresszió javítás előtt | Az aktuális register-only formátum tesztje hibázott a várt fejléc-elutasítással |
| F-01 regresszió javítás után, normál és `python -O` | Módonként 4/4 PASS |
| Végső javításjelölt teljes Python unittest | 60/60 PASS |
| Végső javításjelölt CTest | 22/22 PASS; a négy új eset a meglévő Python-csoport része |
| Lockolt archívum patch-preflight / H8 generálás | 11 patch, 8 kimeneti fájl; 3 generátorbemenet, 4 kimenet, 2 futás PASS |
| Forrás-/notice-audit és helyi dokumentumlinkek | PASS |
| Új teljes MAME-fordítás / új firmware-, audio- vagy REAPER-próba ebben az auditban | NOT RUN; a patch-lánc nem változott, korábbi MAME CI-t ellenőriztünk |
| Új helyi Linux MSan / Windows futás ebben az auditban | NOT RUN; a megjelölt CI-adatok külön bizonyítékok |

Az auditált alapcommit saját [ROM-mentes CI-je](https://github.com/RobCZart82/yaman1x/actions/runs/40504511349)
és [MSan-futása](https://github.com/RobCZart82/yaman1x/actions/runs/40504511264)
sikeres. A változatlan kutatási cél legutóbbi main-ági
[MAME-futása](https://github.com/RobCZart82/yaman1x/actions/runs/40501704765)
a #36 merge `33dc7e97c174c7b4992ff838380b2ae9de01a579` revíziójához tartozik;
nem a #37 vagy az auditjelölt újrafordításának állítjuk be.

A végső jelölt [#38 PR](https://github.com/RobCZart82/yaman1x/pull/38) saját
ellenőrzései a PR checkjeinél követhetők. A helyben tesztelt fa megegyezik a
`de892c3ea278611d1a60e68ccbad8991e174b768` közzétett revízió fájlfájával;
a követő commit csak a napló/ellenőrzési hivatkozásokat egészíti ki. A fenti
helyi jelölttesztek a kódjavítást és regresszióit tartalmazó munkafát vizsgálják;
a merge SHA csak beolvasztás után ismert. A nyilvános jegyzőkönyv nem tárol
személyes elérési utat, Yamaha-adatot vagy privát nyers logot.

## A fejlesztési napló további vezetése

[DEVELOPMENT_LOG.md](DEVELOPMENT_LOG.md) és
[development-ledger.json](development-ledger.json): PR, dátum, reviewed head,
merge commit, változásfajta és aktív auditbejegyzés. A részletes elfogadási
bizonyíték a REAPER-tervben marad. [CONTRIBUTING](../CONTRIBUTING.md) és a PR-sablon
rögzíti, mikor kell frissíteni ezeket. A sablon és a kézi nyilvántartás nem
garantál automatikus jövőbeli naplófrissítést; ezt a PR-review során ellenőrizni kell.
