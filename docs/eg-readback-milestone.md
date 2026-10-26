# EG-szint visszaolvasása és a DSP-időalap előkészítése

2026. október 26. – YamANIx

**Visszakövettük a 0x22-es regiszter AN1x-firmware-beli használatát, elkészült egy külön kapcsolható szint-visszaolvasási kísérlet és a két VOP3 időalapja.** Szintetikus próba chipenként 2400 tényleges `Vop3::run()` hívással ellenőrizte a blokkfelosztástól független működést. A firmware-es futás továbbra is regiszterpróba: az új mintafuttatás még nincs bekötve hozzá, AN1x-hangkimenet nincs.

## Mit olvas a firmware a 0x22-es regiszterből?

A korábbi felvétel 898 ilyen olvasását két rutinba azonosítottuk az eredeti 1.04 ROM disassemblyjével és a teljes busznaplóval.

| Útvonal | Megfigyelt működés |
|---|---|
| `0x0545E8` rutin, olvasás `0x054674`, naplózott következő PC `0x054676` | A SELECT-be 0..15 csatornaszámot ír, mindegyikhez külön 16 bites értéket olvas, és a `0x2382EE + 2 × csatorna` címen tárolja |
| Ugyanott `0x05468E–0x0546B2` | Előjeles összehasonlítás: 16384 fölött, illetve −256 alatt külön eljárást hív; a 14. csatornán az alsó korlátot kihagyja |
| `0x0515EE` rutin | A 14. csatornát választja; a két chip olvasását `0x051668` és `0x0516C6` naplózott PC mellett 0xC000 értékhez hasonlítja |
| `0x0514A6–0x0514C4` | A RAM-ból vett szintet 64-gyel szorozza, és a kiolvasott szóhoz hasonlítja |

Ezek **kiválasztott csatornához tartozó számszerű, előjeles szintérték** használatát mutatják. Nem indokolják a VOP3 mag 0x21-es, `0x10 | csatornaszám` alakú EG-befejezési státuszának egyszerű átirányítását a 0x22 címre.

Az öt másodperces felvételben chipenként 28 teljes, 16 csatornás pásztázás van, összesen 448 olvasással. Ezen felül egy 14. csatornás összehasonlítás történik. A nullás válasz a pásztázás tartományellenőrzésén átmegy, de a külön 0xC000 összehasonlításon **nem**: ezért a korábbi nullás válasz nem tekinthető teljesen megfelelő perifériaviselkedésnek.

Az új `tools/eg_readback_audit.py` a SELECT állapotát a trace-ből követi, ellenőrzi a csatornasorrendet és külön mutatja a kétféle feltétel eredményét. Hiányos pásztázás és részleges szóolvasás hiba. A firmware-címek az ismert 1.04 ROM-ra vonatkoznak.

## Külön kísérleti modell

A bridge ABI 2 új `an1x-level-experiment` profilja a kiválasztott VOP3 EG-csatorna modellbeli `level` értékét szorozza 64-gyel. A nem egész eredményt nulla felé csonkolja, majd előjeles 16 bitre telíti. A szorzó firmware-beli bizonyítékon alapul; **a pontos lekerekítés, telítés és a modell szintjének fizikai hardverhez való megfelelése még nincs igazolva**. Az index 0..15 lehet; más indexet elutasítunk, nem maszkoljuk önkényesen egy létező csatornára.

A kapcsoló `--eg-readback an1x-level-experiment`, csak explicit bridge és kísérleti feltöltésprofil mellett használható. Az alapértelmezett `zero` megőrzi az előző nullás viselkedést. A napló és a manifeszt rögzíti a profilt. A 0x21 státusz továbbra is külön a külső VOP3 `read()` függvényét használja. Az auditor a régi ABI 1 felvételeit is kezeli, de nem fogad el hozzájuk új modellre utaló jelölést.

A kísérleti profil 898 olvasását az audit külön **kísérleti szintolvasásként** jelöli. A `unsupported_reads=0` ebben a profilban a szoftveres kezelés meglétét jelenti, nem hardveres helyességet. A firmware-es próbában a mintafuttatás hiánya miatt a modell szintjei továbbra is nullák; a külön 0xC000 feltétel nem teljesült. Nem állítottuk mesterségesen a várt értékre.

## Időalap és tényleges szintetikus mintafuttatás

A saját `SampleClock` abszolút, egész nanomásodpercekből számol mintaszámot. A minta határára eső hozzáférés előtt esedékes az adott minta; az idő nem léphet vissza. Nem kerekített mintaperiódusok összeadásával dolgozik, ezért nincs blokkfelosztástól függő sodródás. Az egy hívásra jutó korlát túllépését állapotmódosítás előtt elutasítja.

Az ABI 2 `yamanix_vop3_advance()` függvénye mindkét tényleges VOP3-objektumon meghívja a `run()` függvényt a kiszámolt darabszámban, legfeljebb 4096 mintát chipenként és hívásonként. A könyvtár jelenlegi modelljének **48 kHz** értékét használja. Ez **nem az AN1x hardverórajelének igazolása**; a firmware-es futtató nem hívja az új függvényt. A fizikai órajel, a CPU–DSP hozzáférések mintahatárhoz viszonyított sorrendje, az upload alatti leállítás és az audioút még külön bekötést és mérést igényel.

A szintetikus teszt Yamaha programkép nélkül:

- külön chipet és külön csatornát ellenőriz, például −256 → 0xC000 és +256 → 0x4000;
- egy 1 ms-os, még célszint-keresztezés előtti változást független exponenciális képlettel vet össze;
- az 50 ms-ot egyben és több részre bontva futtatja: chipenként **2400 minta**, egyező visszaolvasás;
- ellenőrzi az előjeles ábrázolást, a választott telítési szabályt, az érvénytelen indexet, az idővisszalépést és a mintakeretet;
- az időszámláló külön próbája 44,1/48/192 kHz-en vizsgálja a mintahatárokat, a blokkfelosztást és a nagy időértékeket. Ez általános számlálóteszt, nem három hardveres AN1x-mód igazolása.

## Feltárt eltérés a külső EG-modellben

A próba a VOP3 forráskód leírása és a tényleges működés között eltérést talált. A `Vop3::run()` célszint-keresztezéskor beállítja a `done` bitet és lenullázza a `side` mezőt, de a következő mintákban az aktív `rate` miatt tovább közelít az `aim` értékhez. A forrás leírása ezzel szemben a célszinten való megállást mondja.

A konkrét, szintetikus esetben a kezdőszint −256, cél 128, aim 511, rate 64. 50 ms után a visszaolvasott szó **0x7FBF**, a célszint 64-szerese pedig **0x2000** lenne. A blokkfelosztások egyeznek, tehát ez nem az új időszámláló kerekítési hibája. A cél- és aim-írások pontos fizikai szakaszindítási viselkedése is nyitott kérdés az upstream dokumentációban; a megfigyelés önmagában nem bizonyítja, hogy a valódi chip hogyan tartja a szintet.

Ezt reprodukálható mérési eredményként rögzítettük, a külső VOP3-forrás változatlan maradt. A sikeres tesztek a szállítási, profilkezelési, számlálási és célszint előtti matematikai feltételeket igazolják; **nem állítják, hogy a teljes EG-modell hardverhű**. A mérési JSON `upstream_holds_target=false` mezővel őrzi az eltérést.

## Ellenőrzések és folytatás

- Mind a **10 CTest-bejegyzés** sikeres, köztük az új időszámláló, a tényleges VOP3-mintafuttatás és a firmware-feltételek elemzője.
- Az új `zero` profil és a kísérleti szintprofil mind a korábbi **25 310 regiszteres eseményt** változatlanul reprodukálja ebben a mintafuttatás nélküli firmware-próbában. Az élő feltöltési mezők és az előző natív visszajátszás egyeznek.
- A passzív alapbeállítás korábbi **6441 eseménye** is változatlan.
- A profillal ellentmondó trace-jelölést az auditor elutasítja.
- Hét egymás utáni kutatási patch az eredeti ZIP-ből bájtra előállítja a hét módosított/új MAME-forrásfájlt. A VOP3 eredeti munkapéldányának minden meglévő archívumbeli fájlját és a letöltési leltár 27 eredeti fájlját is változatlannak ellenőriztük.

Következő feladat az EG célszint utáni viselkedésének tisztázása/javítása mérési referenciával, az AN1x órajel és táblarom-skála igazolása, majd a most ellenőrzött időalap összekötése a firmware regisztereléréseivel. AN1x-hang, teljes boot, VST3/AU és GUI még nincs.

Mentett eredmények: `docs/eg-readback-milestone/`. Reprodukció: `research/README.md`. A nyers firmware/disassembly felvételek és az állapotképek a `work/` alatt maradtak.
