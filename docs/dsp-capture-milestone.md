# DSP-regiszternapló és ellenőrzött feltöltés-visszajátszás

Újabb mérföldkő: `upload-control-milestone.md`. A maszk és a várt státuszbit már kiolvasott, és külön kísérleti profilban az első DSP teljes programfeltöltése ellenőrizhető. A passzív eredmény változatlan; a kísérleti profil nem hardvervalidálás.

2026. október 26. – YamANIx

**Elkészült a veszteségellenőrzött DSP-regiszternapló és a VOP3 könyvtárra épülő offline feltöltési audit.** Az eredeti firmware öt másodperces futásának 6441 eseményét rögzítettük; a 2343 írás visszajátszott feltöltési állapota bájtra pontosan egyezik egy külön Python-rekonstrukcióval. A mérés egy konkrét hiányt is feltárt: a DSP feltöltésvezérlő regiszterének válasza még nincs modellezve, ezért teljes programfeltöltést nem igazoltunk.

## Mit rögzítünk?

A H8 által végrehajtott minden olvasást és írást a `0x440000–0x4400FF` és `0x480000–0x4800FF` tartományokban. A napló megőrzi az ismételt hozzáféréseket, a sorrendet, a nanomásodpercben megadott szimulált gépidőt, az adatot, a memóriamaszkot és a programcímet. Ez az adott időablakra és címtartományokra teljes felvétel; más címeken megjelenő esetleges aliasokat nem zár ki.

Sorszámfolytonosság, időrend, fejléc, zárósor és eseményszám ellenőrzött. A 200 000-es korlátból nem veszett esemény. Hiányzó zárósor, számeltérés, eseményvesztés vagy részleges szóhozzáférés esetén az ellenőrző hibával leáll. Az exportált eseményfájlnak is van kötelező zárószámlálója, így egy teljes sor végén csonkolt exportot is elutasít a visszajátszó.

## Mért forgalom

| | DSP 0, 0x440000 | DSP 1, 0x480000 |
|---|---:|---:|
| Olvasás | 4097 | 1 |
| Írás | 1176 | 1167 |
| Programszó-írási tranzakció | 130 | 130 |
| Különböző érintett programszóhely | 60 | 60 |

A 512 lépéses programtérben 2560 programszóhely van chipenként. A felvételben csak a 0x06 és 0x07 programregiszterek írásai látszanak; a 0x08–0x0A regiszterek, a teljes kezdeti mikroprogram és a slotok feltöltése nem igazolt. A lefedettségi szám önmagában nem jelenti, hogy egy működő programnak minden nullás szót is fel kell töltenie; a hiányos betöltésre a vezérlési út megfigyelése is utal.

## A következő akadály pontosabb helye

Az első rögzített tranzakció a DSP 0 `0x01` regiszterébe írt `0x0001`. Ezt ugyanennek a regiszternek **4096 darab, nullát visszaadó olvasása** követi. A passzív felvételben ezek a még be nem kötött MAME-címtartomány alapértékei, nem valódi YSS236-válaszok.

A firmware saját H8 disassemblyjében a `0x0567AA` környékén `0x1000` számláló inicializálása, majd maszkolt státusz-összehasonlítás látszik. A számláló elfogyásához tartozó ágon `0x007F` visszatérési értéket állít elő. A firmware tehát nem egyszerűen díszítésként olvassa a címet; feltételes, időkorlátos várakozás történik.

A VOP3 kutatási dokumentuma a `0x01` regisztert feltöltésvezérlésként írja le, de a bitjeit nyitott kérdésként kezeli. A könyvtár jelenleg nem modellezi ezt a kézfogást. Nem állítottunk be találomra echo-/„kész” választ a teljes feltöltés kikényszerítésére. Az AN1x-ből látott olvasási feltétel pontos jelentését és az elvárt chipviselkedést külön kell tisztázni.

## Mit ellenőriz a visszajátszás?

Az új `yamanix_dsp_replay` program két valódi VOP3 könyvtárpéldány `write()` interfészére adja az írásokat. A külön Python-rekonstrukció a leírt regiszterszabályokból számolja ki a következőket:

- SELECT-latch, beleértve az automatikus növelést és az 511→0 körbefordulást;
- az öt programszó minden lépésen;
- együtthatók, tag bájtok és slot-offsetpárok.

Az ellenőrzött állapotkép 14 340 bájt; mindkét útvonalon SHA-256:

`d2776aff98b440bb4c7d9378ff408503f5b01bbf7b54c744a71bc36c338f2145`

**Ez feltöltési állapotegyezés, nem teljes DSP-állapot vagy hangazonosság.** A visszajátszó nem futtat DSP-mintákat, nem lépteti az LFO-/EG-időzítést, nem modellezi a hosttal közös órázást vagy a külső táblaromot. A 4098 passzív olvasási értéket nem táplálja helyes hardverválaszként a magba. A VOP3 által még figyelmen kívül hagyott konfigurációs regiszterek jelentését az egyező állapotkép sem igazolja.

## Validálás és megőrzés

- A saját négy CTest bejegyzés sikeres: DSP soros próba, LCD-busz, új feltöltési keresztellenőrzés és a Python-eszközök tesztcsoportja.
- A Python-csoport most 12 esetet tartalmaz; a C++ feltöltési próba külön ellenőrzi a két chipet, a select-wrapet, a tagcsonkolást, a slotokat és a sérült bemenetek elutasítását.
- A zárószámlálós export bevezetése után az érintett teszteket újrafuttattuk és a tényleges felvételt ismét összevetettük.
- A végleges MAME-kutatási build friss munkamappában, öt másodperces próbával szabályosan befejeződött; az állapotegyezés ezen a felvételen is fennáll.
- A kutatási patch ellenőrzött; a leltározott 22 eredeti letöltött fájl hashértéke változatlan.

Reprodukció: `research/README.md`. Elemző: `tools/dsp_trace.py`; VOP3-visszajátszó: `src/dsp_replay.cpp`; módosítás: `research/mame-dsp-capture.patch`. A `docs/dsp-capture-milestone/` könyvtár összefoglalókat, teszteredményeket és hash-azonosítókat tartalmaz. A nyers regiszterfelvétel, az export és a firmware-ből származó állapotképek a `work/YamANIx/dsp-milestone-final/` alatt maradnak; nem csomagoltuk őket a saját forrás mellé.

## Következő lépés

A `0x01` feltöltésvezérlő állapotregiszterének működése és a firmware sikeres/fallback útvonalainak összevetése. Ezután következhet az igazolt teljes mikroprogram-feltöltés és a CPU-val időben összekötött két VOP3 mag. Hangkimenet, teljes hardverazonosság és VST3/AU kiadás továbbra sincs igazolva. GUI nem készült, eredeti ROM nem módosult.
