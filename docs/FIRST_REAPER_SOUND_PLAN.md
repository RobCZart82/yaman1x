# YamAN1x: fejlesztési terv az első REAPER-ben megszólaló hangig

Rögzítve: 2026-10-27. Kiindulópont: a #14 PR-ig beolvadt kutatási eredmények.
Ez végrehajtási terv; az alábbi, még nyitott mérföldkövek nem kész funkciók.

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
