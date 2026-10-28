# YamAN1x repository-audit — 2026-10-28

Átvizsgált főági revízió: `863d21abcde448d746b3b44c39a1eb61be455a15` (#58).
Motoralap: `f8b6aa20834f209931e3e9bb9463cc47b5b7a540` (#57).
Ez ellenőrzési pillanatkép, nem teljes CPU-, firmware-, hardver- vagy hangelfogadás.

## Eredmény

A vizsgált tesztekben és összehasonlításokban nem találtam reprodukálható
motorregressziót. A főági dokumentáció aktuális összefoglalója helyesen 39
CTest-csoportot és 84 Python-esetet jelez. Egy későbbi STATUS-bekezdés viszont
még „current CMake suite” megjelöléssel 27 csoportot írt; ezt 39-re javítottam,
a korábbi 22/56 és 60-as eredményeket kifejezetten történeti bizonyítékként hagyva.
A friss audit hivatkozásait README/STATUS/fejlesztési napló alatt egységesítettem.
Működő motorkódot nem kellett módosítani.

## Hatókör

- Build és három workflow: platformtesztek, VOP3 kezdeti dekódolási MSan,
  külön teljes MAME-kutatási build; ROM-mentes és helyi kutatási határok.
- Saját CPU futtató, költségkeret, busz, kézi dekóder/segédek; GPIO/IRQ/SCI,
  ciklus-/bemeneti idővonal; DSP bridge, mintavételi óra és upload-szerződés.
- Python bemenet-/napló-/bankellenőrzők és generátor-/referenciaeszközök;
  teszt-fixture-ek, független flag/bit orákulumok és hiba-/izolációs próbák.
- A #44 óta beolvadt fejlesztések (#45–#58), napló, státusz, elsőhang-terv,
  rögzített forráseredet, licencek, privát adatok kizárása és helyi hivatkozások.
- A 201 változatlan upstream fájl tartalmát hash-audit ellenőrizte, nem új,
  sorról sorra végzett AsmJit/VOP3-kódreview. Az egyetlen áttekintett vendor-
  javítás továbbra is a visszafordítható első-dekódolási inicializálás.

## Friss helyi bizonyíték — macOS arm64, Release

| Ellenőrzés | Eredmény és határ |
|---|---|
| Tiszta CMake-konfiguráció/build/CTest | 39/39 PASS; új buildkönyvtár, Yamaha-fájl nélkül |
| Python unittest discovery | 84/84 PASS; itt nincs Windows symlink-jogosultsági kizárás |
| Két rögzített generált kiválasztás | Eredeti generátorból kétszer előállítva; 366 full/partial metódus egyezik a kiválasztással |
| Eredeti H8H dekóder/host/execute_run összevetés | 200000 szelet, 668426 utasításolvasás, 46258 bájtolvasás, 25800 bájtírás, 67331 szóolvasás, 41382 szóírás; 0 eltérés |
| Külön dekóderkiválasztási próba | 917504 vizsgált állapot/fázis; eltérő vagy túl tág elfogadás nem jelentkezett a vizsgált csoportokban |
| ASan/UBSan | Verem/vezérlés, aritmetika, carry/logika, shift/rotate: 4/4 célzott natív csoport PASS |
| Forrás-/feltöltési audit | 425 fájl; 201 upstream azonosság és 1 áttekintett vendor-javítás, 0 probléma |
| Értesítés-/licenclefedettség | 229 fájl, 5 licencszöveg; 0 lefedettségi probléma. Nem bináris terjesztési/jogi jóváhagyás |
| Napló és dokumentumok | 52 bejegyzett merge commit elérhető; 0 hiányzó helyi Markdown-fájlhivatkozás |

A fájlszámok az átvizsgált alapra vonatkoznak; maga ez a jelentés és naplófrissítése
növeli a jelölt fájlszámát. Motor-tesztcsoportot vagy Python-esetet nem ad hozzá.

A külön dekóderpróba az összes 16 bites alsó állapotot ellenőrizte full és partial
fázisban a 0, 2, 0x11, 0x19, 0x1a, 0x1b, 0x1c felső állapotcsoportokban.
A módszer a kezelők végrehajtása helyett a kiválasztott metódus nevét hasonlította
az eredeti generált diszpécserhez, a jelenleg kiválasztott metódushalmaz határán.
Ez egyszeri helyi auditpróba, nem új CI-teszt vagy teljes H8 opcode-elfogadás.
A teljes állapot/busz eredetimetódus-próbát a repository eszköze reprodukálja:

```sh
cmake -S . -B build-audit -DCMAKE_BUILD_TYPE=Release
cmake --build build-audit --parallel 4
ctest --test-dir build-audit --output-on-failure
python3 -m unittest discover -s tests -p 'test_*.py'
python3 tools/repository_audit.py
python3 tools/license_coverage.py
python3 tools/check_h8_first_instructions.py --archive /absolute/path/to/mame-master.zip --work work/audit-h8-reference
```

Az opcionális referenciafuttatás új work könyvtárat és a meglévő hash-elt külső
forrásarchívumot igényli. Nem módosítja a letöltött archívumot vagy ROM-okat.

## GitHub-bizonyíték

A #58 dokumentációs headje `a331d76f4d6a9dd4ab5e6d4b61de0b5145055f1d`:
[három platform PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40690053399),
[MSan PASS](https://github.com/RobCZart82/yaman1x/actions/runs/40690053451).
A dokumentációs útvonalszűrő miatt ezen nem kellett külön MAME-jobnak indulnia.
A #57 motorhead öt lezárt jobját a [STATUS](STATUS.md) és a
[verem/vezérlés komponensjegyzőkönyve](h8-stack-control-flow.md) őrzi.
Ez az audit nem indított új teljes MAME-buildet és nem végzett friss firmware-futást.

## Következő fejlesztési és ellenőrzési feladatok

1. A megvalósított H8H-részhalmaz bővítése előtt maradjon meg a szigorú profil-
   és dekóderhatár. IRQ/TRAPA-belépés, IRQ-szűrők és közös CPU/periféria-ütemezés
   még hiányzik; szintetikus RTE-visszaállítás nem igazol megszakításbelépést.
2. Érdemes a két kiválasztott generált fájl valódi újragenerálását és az
   eredeti dekóderrel való összevetést külön CI-kapuvá tenni. Jelenleg ezek
   opcionális helyi referenciaeszközök; a normál CI fixture-/hash-próbái és a
   teljes MAME-build nem helyettesítik a saját kiválasztás ilyen ellenőrzését.
3. A CPU-családok ASan/UBSan futtatását érdemes automatizálni. A meglévő MSan-job
   a VOP3 kezdeti dekódolási hibáját vizsgálja; nem a teljes CPU sanitizerfutása.
4. M1 firmware/PKS/MIDI-elfogadás, tényleges AN1x órajel/EG/táblacímzés és
   offline hang továbbra is nyitott. A VST3/REAPER szakasz ezek után következik.

A 2–3. pont teszt-infrastruktúra fejlesztési javaslat, nem most reprodukált
motorhiba. A vizsgálat nem indokolja a jelenlegi dokumentációs PR több
mérföldkövön át történő visszatartását. Sikeres saját ellenőrzések és review
után önállóan beolvasztható.
