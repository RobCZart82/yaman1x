# Következő mérföldkő: működő LCD-kézfogás, elért DSP-címek

Újabb állapot: a `dsp-capture-milestone.md` már teljes regiszternaplót és offline feltöltési auditot dokumentál; a következő konkrét hiány a DSP 0x01 feltöltésvezérlő regisztere.

2026. október 26. – YamANIx

**Az eredeti 1.04-es firmware túljutott a korábbi LCD-várakozó cikluson.** A külön kutatási futtatóban a MAME meglévő LC7985 regiszter- és időzítésmodelljét kötöttük a H8 portjaihoz. Az öt másodperc szimulált idejű hidegindítás szabályosan befejeződött. Ez még nem igazolt teljes AN1x-boot vagy hangzó emulátor.

## Mit bizonyítottunk a bekötés előtt?

A korábbi naplóban a firmware a `0x03F66A–0x03F67E` ciklus környékén maradt. A MAME H8 disassemblere szerint a rutin:

- a portok irányát olvasásra állítja;
- PB2-n RS=0-t, P90-en R/W=1-et állít;
- P95-ön E-jelet vált;
- a B portot olvassa;
- a negatív/busy bit alapján visszaugrik a ciklus elejére.

Ez megerősíti, hogy a megfigyelt ciklus a kijelző busy-flagjére vár. A teljesebb szervizkönyvek CPU- és LCD-oldalán ellenőrzött bekötés:

| LCD-vonal | H8-vonal |
|---|---|
| DB0 | PA0 |
| DB1 | PB1 |
| DB2–DB5 | PA2–PA5 |
| DB6–DB7 | PB6–PB7 |
| RS | PB2 |
| R/W | P90 |
| E | P95 |

Az adatbusz tehát nem egyszerűen egyetlen port. Az írási adat `(PA & 0x3D) | (PB & 0xC2)`. A saját, kis C++ bekötési modell ezt és a jeléleket kezeli; a vezérlőregiszterek, DDRAM/CGRAM és a busy-állapot továbbra is a MAME LC7985 eszközében vannak.

Olvasást az E felfutó élén mintavételezünk; írást a lefutó élén adunk át. Az ismételt, azonos E-szint nem hoz létre új tranzakciót. Az LCD-adatvonalakon kívüli bemeneti bitek korábbi felhúzott szintje megmarad; a portirány és a kimeneti bitek visszaolvasását a H8 modell kezeli.

## Az ellenőrzött öt másodperces futás

| Megfigyelés | Eredmény |
|---|---:|
| Futtató kilépési kódja | 0 |
| PC-minták | 105 |
| Első olvasási címmegfigyelések | 110 |
| Első írási címmegfigyelések | 183 |
| Busy státuszolvasások | 7193 |
| Kész státuszolvasások | 561 |
| LCD parancsbusz-írások | 288 |
| LCD adatbusz-írások | 274 |

A parancs-/adatszámlálók a buszon átadott tranzakciókat számolják, nem minden egyes parancs hardveres elfogadásának független bizonyítékai. A busy- és készválaszok viszont a vezérlőmodell változó státuszából származnak; nem állandó nullát adtunk a firmware-nek.

A naplóban felismerhető inicializáló parancsok jelennek meg (`38`, `0C`, `01`, `06`, `80`), majd adatírások. A megfigyelt adatfolyamban a `YAMAHA` és `1997` bájtjai is szerepelnek. Ezek buszadatok, nem megjelenített képernyőkép.

A firmware a `0x440000` és `0x480000` körüli, 16 bites DSP-regisztereléréseket is végrehajtja; e két korábbi báziscímjelölt így dinamikus megerősítést kapott. A `0x400000` körüli effektinterfész-írások is láthatók. Ettől a DSP még nem működik: ezek jelenleg megfigyelt, a szintézismaghoz még nem kapcsolt elérések. A teljes címtükrözés, az összes regiszter jelentése és a host-válaszok még ellenőrzendők.

## Tesztelés

- A saját LCD-buszteszt mind a 256 bájt leírását és visszaolvasását ellenőrzi az osztott portokon, továbbá RS-választást, E-élirányt, ismételt jelszintet és a tétlen busz felhúzott szintjét.
- A saját soros DSP-próba továbbra is sikeres.
- A Python ROM-/bank-/trace-eszközök 9 tesztesete sikeres. Az új trace-teszt ellenőrzi az LCD-számlálók és a DSP-címek kiolvasását, és nem állít ezekből teljes bootot.
- A `ctest -R yamanix_ --output-on-failure` három saját tesztbejegyzése sikeres.
- A fordított MAME-kutatási programmal friss munkamappában 5 másodperces integrációs próbát futtattunk.
- A kutatási patch visszafelé alkalmazhatóságának ellenőrzése sikeres. A korábban leltározott 22 letöltött fájl SHA-256 értéke változatlan.

## Reprodukálható eredmények

Források és fordítás: `research/README.md`. Saját bekötés: `src/lcd_bus.h`. Mérési parancs, napló és buildazonosítók: `docs/lcd-milestone/run.json`, `trace.log`, `console.log`, `build-fingerprints.json`. Az előző mérföldkő naplói külön megmaradtak, így összehasonlítható az LCD nélküli és az új állapot.

Az LCD karakter-ROM-ját ebben a kijelzés nélküli kutatási buildben üres grafikus régió helyettesíti; a karakterrajzokat nem használjuk. A firmware és a H8 CPU-logika változatlan. A LC7985 busz- és időzítésmodellje nincs mesterséges „mindig kész” válaszra cserélve. A valódi hardverhez mért LC7985-időzítési pontosságot ebben a körben nem igazoltuk. Soft reset és save-state kezelés még nincs validálva.

## Következő motorfeladat

A DSP-k teljes regiszterfolyamának rögzítése és visszajátszása, majd a két VOP3 példány valós host-interfészen történő bekötése. A jelenlegi „címenként első elérés” trace nem alkalmas teljes mikroprogram rekonstruálására, mert az ismételt feltöltési írásokat nem őrzi meg. Az ismeretlen eszközök alapértelmezett olvasási értékei nem tekinthetők hiteles DSP-válasznak.

Hangszín megszólaltatása, MIDI-vezérlés, MEG effektek és a VST3/AU csomagolás még hátravan. GUI-fejlesztés és távoli publikálás nem történt.
