# YamAN1x: fejlesztési terv az első REAPER-ben megszólaló hangig

Rögzítve: 2026-10-27. Kiindulópont: a #14 PR-ig beolvadt kutatási eredmények.
Ez végrehajtási terv; az alábbi, még nyitott mérföldkövek nem kész funkciók.

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
ROM, bank és privát trace helyett csak publikálható azonosító és összefoglaló
kerülhet a tervbe. A részletes mérési jegyzőkönyvekre hivatkozás mutasson.

## Előrehaladási napló

| Dátum | Változás és állapot | Ellenőrzés / bizonyíték | Következő lépés |
|---|---|---|---|
| 2026-10-27 | [#16](https://github.com/RobCZart82/yaman1x/pull/16), beolvadt: Python `-O` mellett is aktív ABI-ellenőrzések, AsmJit tesztforrások kizárása, harmadik fél patch-darabszám javítása | `ac56d8c7144ef8cb478f3a714578203c440b841e`: Linux x64, macOS arm64, Windows x64 CI 11/11 tesztcsoport PASS; [futás](https://github.com/RobCZart82/yaman1x/actions/runs/40454166792). Helyi Windows: build PASS, CTest 10/11; egy meglévő symlinkteszt WinError 1314 miatt akadályozott. [Auditkövetés](AUDIT_FOLLOWUP_2026-10-27.md). | A VOP3 inicializálás/`memcmp` kockázat célzott vizsgálata; EG target-hold változtatás csak hardveres bizonyíték után. |
| 2026-10-27 | [#17](https://github.com/RobCZart82/yaman1x/pull/17), beolvadt: [ADR-0001](ADR-0001-engine-boundary.md), függőségazonosítók és [buildalap](BUILD_BASELINE.md). **M0 részben teljesült.** | `bd2262fd7c06c91f7e40ea2aaa3ea52dceecedc1`: mindhárom platform CI PASS; [futás](https://github.com/RobCZart82/yaman1x/actions/runs/40454539768). Helyi CMake-konfiguráció és forrásaudit PASS, 201 változatlan upstream fájl ellenőrizve. | H8-port függőség- és licenclezárása, példányizolációs/időléptetési próbák, teljes referencia-toolchain rögzítése; saját terjesztési licenc döntése. M1 a külön MAME-kutatási úton folytatható. |
| 2026-10-27 | [#19](https://github.com/RobCZart82/yaman1x/pull/19): **M1 folyamatban:** időbélyeges SCI-hozzáférés-megfigyelő és naplóellenőrző elkészült; firmware-es elfogadás még nem futott. [Részletek](m1-sci-access-milestone.md). | Natív megfigyelő és célzott Python-tesztek Windows x64-en PASS. A teljes Python-csomag meglévő symlinktesztjét WinError 1314 akadályozza. Tizenegy patch alkalmazása a rögzített forrásra PASS; teljes új MAME-build és friss firmware-próba NOT RUN. | Külön macOS kutatási build, SCI-beállítások és rövid engedélyezési ablakok rögzítése, majd a PC-k követése privát disassemblyvel. PKS-válasz továbbra sem feltételezhető. |
| 2026-10-27 | [#20](https://github.com/RobCZart82/yaman1x/pull/20), beolvadt: **M0/M1 folyamatban.** Upstream forrásazonosítók visszanyerése, ismételhető patch-előellenőrző és teljes macOS MAME-build CI. [Bizonyíték és határok](source-identity-recovery.md#verified-result). | `31664c74f2726091eb9f403c1621ef949479ddbb`: [Linux/macOS/Windows alapcsomag](https://github.com/RobCZart82/yaman1x/actions/runs/40457998062) 12/12 csoport PASS platformonként; [teljes MAME-build és arm64/regisztrációs próba](https://github.com/RobCZart82/yaman1x/actions/runs/40457998036) PASS Xcode 16.2-vel. 11 patch, 4 egyező projektfejléc; firmware/SCI/MIDI-elfogadás NOT RUN. A kezdeti fordító- és Windows-sorvéghiba dokumentálva. | Friss, helyi firmware-felvétel és SCI-vezérlésiút-elemzés. A teljes kutatási build előfeltétele teljesült; H8-port és példányizoláció továbbra is nyitott. |
| 2026-10-27 | [#22](https://github.com/RobCZart82/yaman1x/pull/22): **M0 alapstabilitás, bizonyított konstruktorhiba javítása.** VOP3 `ops` inicializálása az első dekódolás előtt; [bizonyíték és visszafordítható provenance](vop3-initial-decode-fix.md). | `44d5e8c8d2708e6736ce88e8cbe48be67a7b314c`: az eredeti fejléc inicializálatlan `memcmp` olvasását a [MemorySanitizer reprodukálta](https://github.com/RobCZart82/yaman1x/actions/runs/40461084899), az inicializált jelölt tiszta. Natív konstrukció/dekódolás és provenance-regressziók hozzáadva; végső platform- és sanitizer-eredmények a PR checkjeinél. | H8-port és friss helyi firmware/SCI/MIDI-elfogadás továbbra is nyitott. Az EG target-hold változatlan, hardveres bizonyíték szükséges. |
| 2026-10-27 | [#23](https://github.com/RobCZart82/yaman1x/pull/23): **M1 Mac-folytatás előkészítve.** Generált két másodperces MIDI-hang és két friss, egyetlen státuszprofilban eltérő firmware-megfigyelés futtatója; [Mac M1 átadás](MAC_M1_HANDOFF.md). | Szintetikus MIDI-időzítés, párosítási és hibakezelési próbák helyben PASS; forrásaudit PASS. A végső platform- és sanitizer-eredmények a PR checkjeinél. Valódi firmware/SCI/MIDI-futás NOT RUN. | A Macen eszköz- és forrásellenőrzés, build, majd a helyi párosított próba. A befejezett suite nem jelent automatikusan igazolt MIDI-feldolgozást vagy bootot. |
| 2026-10-27 | **M0 H8-port előkészítés:** 30 fájlos, hash-elt include/implementáció leltár és generált/keretrendszer-határok. [Részletek](h8-port-source-boundary.md). | A rögzített archívum read-only feldolgozása PASS; ciklus-, párosítás-, hiányzó forrás-, hash- és útvonal-regressziók PASS. Ez nem teljes fordítói függőségzárás vagy licenclezárás; beágyazott H8-build NOT RUN. | Generátor/lista/buildszabály rögzítése, framework-szolgáltatások feltárása és minimális hostadapter; a Macen külön friss firmware-próba. |
| 2026-10-27 | **M0 H8-generátor előkészítés:** három buildbemenet és négy generált fájl hash-rögzítése; [ismételt generálási próba](h8-port-source-boundary.md#repeated-generation-experiment). | Windows Python 3.13.3: négy kimenet két független generálása PASS; bemenetazonosság- és kimenettartalom-regressziók PASS; forrásaudit PASS. Ág: `feat/h8-source-inventory`; PR/platform-CI még NOT RUN. | Keretrendszer-szolgáltatások és generált kód függőségeinek feltárása, minimális hostadapter és fordítás. CPU/firmware-végrehajtás nem történt. |

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

## Jelenlegi helyzet

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

2026-10-27: [ADR-0001](ADR-0001-engine-boundary.md) rögzíti a motorhatárt,
a függőségazonosítókat és a VST3 SDK-választást. M0 részben teljesült;
a H8-port, annak függőség- és licencauditja, valamint a teljes referencia-
toolchain rögzítése még nyitott.

- Rögzíteni a fordító, célarchitektúra és külső függőségek pontos verzióját.
- Eldönteni, hogyan kerül a H8/perifériamodell a beágyazható motorba. A jelenlegi
  MAME parancssoros kutatási program és dinamikus bridge még nem pluginmotor.
- Megvizsgálni a CPU-mag, DSP-mag és VST3-hosztréteg integrálását és licenceit;
  a saját kód licencéről is dönteni a bináris terjesztés előtt. A kiválasztott
  VST3 SDK/keretrendszer verzióját és feltételeit ekkor kell ellenőrizni.
- Megőrizni a ROM-mentes alapbuildet, a forrásauditot és a meglévő kutatási utat.

**Kész, ha:** van rövid architektúradöntés, függőséglista és megismételhető
buildutasítás. A kiválasztott motor CPU-, periféria-, DSP- és MIDI-állapota
példányonként elkülöníthető, időléptetése külső audiofolyamhoz illeszthető.

### M1 — Indulási út, PKS és valódi MIDI-vétel

Mac M1 folytatás: [helyi előkészítés és párosított firmware-próba](MAC_M1_HANDOFF.md).
Az új futtató szervezése szintetikusan tesztelt; tényleges firmware-es
eredmény csak a Macen végrehajtott friss futás után rögzíthető.

Következő előfeltétel: [rögzített forrású teljes kutatási build](source-identity-recovery.md).
Külön ROM-mentes CI igazolta a teljes MAME-target buildjét; ennek sikere sem
helyettesíti a friss firmware-es SCI/MIDI-próbát.

2026-10-27: **folyamatban**, első műszerezési lépés elkészült:
[időbélyeges SCI-hozzáférések](m1-sci-access-milestone.md). A ROM-mentes
tesztek a megfigyelőt és a naplóellenőrzést vizsgálják; a teljes új MAME-build
azóta a #20 CI-jében sikeres. A friss firmware-es MIDI-próba még nem futott;
M1 nem teljesült.

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
