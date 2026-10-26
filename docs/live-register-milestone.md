# Két DSP feltöltése és élő H8–VOP3 regiszterkapcsolat

2026. október 26. – YamANIx

**Az eredeti 1.04 firmware mindkét VOP3-példány teljes hangprogramját feltöltötte a futó kutatási folyamatban.** A feltöltési állapot egyezik a független Python-rekonstrukcióval és a külön C++ visszajátszóval. A feltöltésvezérlés továbbra is kísérleti státuszprofilt használ. Ez regiszterkapcsolat: nincs DSP-mintaütemezés, hangkimenet vagy igazolt teljes AN1x-boot.

## A második DSP érzékelése

A firmware `0x06C558` címen a H8 Port 9 regiszterét (`0xFFFFD2`) olvassa, majd a 2. bitet (`0x0004`) vizsgálja. A `0x23CBC3` bájtba magas bemenetnél nullát, alacsonynál egyet ír. Ez a feltétel vezérli a második DSP inicializálását.

A helyi **AN1x-ServiceManual.pdf 51. PDF-oldalán** a kapcsolási rajz útvonala:

`H8/3002 IC1, 16. láb P92/RXD0 → DM CN11, 1. érintkező → DMS CN1, 1. érintkező → DG`.

A főpanelen felhúzó ellenállás tartozik a bemenethez; a csatlakoztatott DMS panel földre köti. A korábbi kutatási modell a teljes Port 9 bemenetre `0xFF` értéket adott, tehát ennél az érzékelésnél a hiányzó panel állapotát mutatta. Ez magyarázza, miért maradt ki a második chip teljes feltöltése.

Új, explicit futtatási beállítás:

- `--dms-board absent`: a régi `0xFF` bemenet; alapértelmezés a korábbi mérések megismételhetőségéhez;
- `--dms-board installed`: `0xFB`, csak P92 alacsony; a többi bemenet magas marad, a kimeneteket a H8 perifériamodell egyesíti.

A firmware saját utasítása állítja elő a jelenlétflaget. A RAM-flaget és az eredeti ROM-ot nem írjuk át. A rajz és a firmware egyezése erős bizonyíték a bemeneti logikára, de nem helyettesít fizikai műszeres mérést, és nem igazolja a teljes modell többi feltételezését.

## Élő regiszterkapcsolat

A saját `yamanix_vop3_bridge` megosztott könyvtár két változatlan VOP3 objektumot tartalmaz. A külön MAME-kutatási program macOS-en, csak explicit `--bridge` kapcsolóval tölti be. A verziózott C interfész ellenőrzi az argumentumokat; részleges szóhozzáférést és hibás chipcímet elutasít. Az ABI 1 egyetlen szálú, regiszterekre korlátozott felület.

A H8 írásai azonnal, a tényleges végrehajtási sorrendben jutnak a VOP3 `write()` függvényébe. A 0x01 státuszolvasását a külön kísérleti `firmware-contract` profil kezeli, a többi DSP-olvasás a VOP3 `read()` függvényéhez kerül. A napló megőrzi a H8 gépidejét, de **a VOP3 `run()` függvényét még nem hívjuk**, ezért ez nem mintaszinten időzített emuláció.

A library hiánya, eltérő ABI vagy elutasított hozzáférés leállítja a próbát. A futtató friss munkakönyvtárat igényel, kizárja a korábbi NVRAM-ot, a környezetből örökölt bridge-beállítást pedig törli. Kilépéskor a tényleges élő VOP3-példányok feltöltési mezőit és a hozzáférésszámlálókat menti. Az új auditor hiányzó lezárást, sérült állapotképet és a trace-től eltérő számlálót elutasít.

## Öt másodperces mérés

Kísérleti státuszprofil, csatlakoztatott DMS-panel bemeneti modellje, élő regiszterkapcsolat:

| Mérőszám | Első DSP | Második DSP |
|---|---:|---:|
| Élő VOP3-példánynak átadott írás | 12 208 | 12 200 |
| Különböző érintett programszóhely | 2560 / 2560 | 2560 / 2560 |
| Átadott olvasás | 449 | 449 |
| Ebből a VOP3 által nem támogatott olvasás | 449 | 449 |
| Lefuttatott DSP-minta | 0 | 0 |

Összesen **25 310 DSP-esemény**, veszteség nélkül. A 902 olvasásból négy a kísérleti feltöltési státuszhoz tartozik; a többi a 0x22-es regiszter olvasása. Mindkét DSP alapfeltöltése külön-külön egyezik a helyi VOP3 `voice.bin` képével: **2560 programszó, 512 együttható, 512 tag, 128 slot-offsetpár**. A második chip feltöltésének ellenőrzési pontja a 21 676. esemény után van.

A két chip összesen 14 340 bájtos, futásvégi feltöltési állapotának SHA-256 értéke:

`62f2939a81db9b28b200033f59944e5a851eb20c891843d958f53389b739e598`

Három módon azonos: élő natív objektumokból mentve, a teljes trace-ből független Python-kóddal rekonstruálva, és a trace-t külön natív VOP3-példányokra visszajátszva. Ez a SELECT/program/együttható/tag/slot mezőket ellenőrzi; más belső állapotot vagy audioazonosságot nem.

A bridge nélküli, de csatlakoztatott DMS és azonos státuszprofil mellett felvett mind a 25 310 esemény egyezik az élő futással, időbélyeggel és adattal együtt. Ez megmutatja, hogy ezen a futási szakaszon az élő regiszterkezelés nem változtatja meg a megfigyelt CPU-útvonalat. A passzív/hiányzó panel alapbeállítás mind a korábbi **6441 eseményt** változatlanul reprodukálja.

## Nyitott regiszterkérdés és határok

Az AN1x ebben a felvételben a **0x22 regisztert** olvassa. A helyi VOP3 forrásban az EG-státusz a **0x21** címen van implementálva; minden más olvasás nulla. Ezért a 898 átadott olvasás mind **nem támogatott, nullás helyettesítő válasz**, amit a bridge külön számlál. Nem vezetünk be bizonyítatlan 0x22→0x21 címátirányítást. Előbb az AN1x firmware olvasási rutinját és a regiszter jelentését kell tisztázni.

A VOP3 egyes konfigurációs és host-RAM írásokat sem valósít meg; az írásszámláló az átadást jelenti, nem minden regiszter teljes modellezését. A feltöltésvezérlő 0x01 időzítése és további státuszbitjei feltételezések maradtak. A táblarom skálázása/bekötése, a chipenkénti mintafuttatás, az összekötött soros audioút, a MEG-effektek és a hiányzó PKS-ROM kezelése továbbra is nyitott. Nincs VST3/AU csomag vagy GUI.

## Ellenőrzések és folytatás

- Mind a **8 CTest-bejegyzés** sikeres: hat saját tesztcsoport és két külső VOP3-teszt.
- A C ABI tesztje két chip külön állapotát, SELECT-léptetést, körbefordulást, hibás argumentumokat és a valós olvasás-/mintaszámlálókat ellenőrzi.
- Valódi felvétel másolatán a sérült élő állapot és a hiányzó chipzárósor elutasítását is ellenőriztük.
- Hat egymás utáni kutatási patch az eredeti MAME ZIP-ből bájtra újra előállítja a hét módosított/új MAME-forrásfájlt.
- A letöltési leltár mind a **27 eredeti fájlja** változatlan SHA-256 alapján.

Reprodukció: `research/README.md`. Mentett, ROM-adatot nem tartalmazó eredmények: `docs/live-register-milestone/`. A firmware, DSP-állapotképek, nyers trace-ek és fordított programok a helyi `work/` alatt maradtak. Következő motorfeladat a 0x22 státuszolvasás értelmezése, majd az igazolt táblarom-formátumra épített DSP-mintaütemezés.
