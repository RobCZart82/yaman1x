# Feltöltésvezérlés: bizonyított feltétel és kísérleti sikeres útvonal

2026. október 26. – YamANIx

**A firmware pontos státuszfeltételét kiolvastuk, és külön kísérleti profilban elértük az első DSP teljes programfeltöltését.** A betöltött program, együtthatók, tagok és slot-offsetek egyeznek a helyi VOP3 csomag AN1x `voice.bin` képével. Az alapértelmezett passzív mód változatlan. A státuszprofil nem hardveren mért YSS236-modell; audio és teljes gépazonosság nincs igazolva.

## Letöltési mappa

Újonnan átnézett anyagok: `mame0289-arm64.zip`, `HD44780Renderer.cpp`, `HD44780LcdDisplayJUCE.h`. Az előző leltárhoz képest az `an1x.zip` és `mulcd.zip` is új bejegyzés, de ezeket már az előző ellenőrzésben külön megvizsgáltuk.

- A MAME ARM64-csomag összes ZIP-tagjának CRC-olvasása sikeres. Ez kész programokat, köztük unidasm eszközt tartalmaz; a roms könyvtárában csak dir.txt van. Új PKS-ROM-ot nem találtunk benne. A csomag programjait nem futtattuk; a saját fordított MAME maradt a kutatási futtató.
- A két új C++ fájl HD44780-karakterrajzolási példa. Az egyik JUCE-komponenst ír le; nem tartalmaznak LC7985 kézfogást vagy az AN1x osztott adatbuszának bekötését. Nem emeltük be őket és nem kezdtünk GUI-fejlesztést. A bennük állított hardverpontosság nincs igazolva.
- A teljes aktuális leltár 27 fájlja változatlan maradt a vizsgálat és a fejlesztés alatt. Részletes fájlazonosítók a projekt melletti `YamANIx-letoltes-frissites-2026-10-26.json` fájlban vannak.

## A firmware-ből bizonyított logikai feltétel

A polling-rutinban a CPU-regiszterek közvetlen megfigyelése:

| Fázis | Maszk | Várt érték |
|---|---:|---:|
| Feltöltés megkezdése, 0x01 regiszterbe írt 1 után | 0x8000 | 0x8000 |
| Feltöltés elengedése, ugyanoda írt 0 után | 0x8000 | 0x0000 |

A `0x056B5C` környéki firmware-rutin az első várakozás időkorlátjának hibáján kihagyja a következő inicializáló blokkot. A sikeres ág beállítja a chip konfigurációját, feltölt egy indítóprogramot, majd a hangprogramot. A korábbi passzív futás ezért csak későbbi paraméterfrissítéseket mutatott, teljes alapfeltöltést nem.

## Külön kísérleti profil

Az új `UploadControlContract` egy korábban kiadott vezérlőparancsból állít elő logikai státuszt:

- 1: a státusz 15. bitje beáll;
- 0: a státusz 15. bitje törlődik;
- egyéb parancs vagy részleges írás: nem támogatott, a kísérleti futás hibával leáll.

A válasz nem a CPU várt értékéből készül; a CPU-regisztereket csak a megfigyelő naplózza. **A válasz késleltetése és a többi státuszbit nullája viszont feltételezés.** Ezeket valódi chipen még ellenőrizni kell. Ezért a profil neve `firmware-contract`, alapból kikapcsolt, és a naplókban `hardware_validated=0` jelölésű. A fő-ROM és a CPU-kód nem módosult.

## Öt másodperces összehasonlító mérés

| Mérőszám | Passzív mód | Kísérleti profil |
|---|---:|---:|
| Rögzített DSP-esemény | 6441 | 13827 |
| Első DSP különböző érintett programszóhelyei | 60 | 2560 |
| Első DSP programszó-írásai, ismétlésekkel | 130 | 5915 |
| Második DSP különböző érintett programszóhelyei | 60 | 60 |
| Elveszett esemény | 0 | 0 |

A friss passzív felvétel minden eseménye egyezik a korábbi ötmásodperces felvétellel, beleértve az időt, adatot és programcímet. Az új modell alapból nem változtatta meg az addigi eredményt.

A kísérleti felvételben az első DSP teljes feltöltése látszik. A firmware `0x23CBC3` című feltételflagje mindvégig nulla; a disassembly szerint emiatt a második DSP teljes inicializáló hívását kihagyja. Ennek a flagnek a hardveres eredete még külön vizsgálandó. Nem írtuk át a flaget a második DSP mesterséges bekapcsolásához.

## A programkép összevetése

Az ellenőrzés az első DSP alapfeltöltése és a slotok beírása utáni, de a későbbi paramétermódosítások előtti pontot választja ki. Mindegyik egyezik:

- 512 × 5 = **2560 programszó**;
- **512 együttható**;
- **512 tag**;
- **128 slot-offsetpár**.

A korábban kétértelműnek tűnő slotmezők is tisztázódtak ennél a képnél: egy négyes lépéscsoportban legfeljebb egy különböző nemnulla offset van. A firmware által a slotba írt érték ezzel egyezik mind a 128 csoportban. Az új adapter konfliktus esetén hibát ad, nem választ önkényesen.

A vizsgált `voice.bin` SHA-256 értéke:

`041ca9414c563158e5f84928c159e1e23b4c46fa43a807bd9713b6bf7f8fffc7`

Az első chip 7170 bájtos feltöltési állapotképének SHA-256 értéke:

`20b036c5c234412a1501d0ad643bca1e9d1727dc86e9fdf32037143817facd0c`

Ez a digest megegyezik a firmware-naplóból rekonstruált és a VOP3 C++ könyvtárra visszajátszott statikus feltöltéssel is. A két adatforrás közös firmware-eredetű lehet; ez nem független fizikai ROM-kiolvasás vagy hangmérés.

## Elkészült kód és tesztek

- `src/upload_control_contract.h`: alapból kikapcsolt, logikai státuszmodell.
- `tools/run_h8_probe.py --dsp-profile`: megkülönböztetett passzív és kísérleti futtatás, mentett profilazonosítóval.
- `tools/dsp_trace.py`: profiljelölés és kevert/érvénytelen profilok elutasítása.
- `tools/an1x_upload.py`: 8192 bájtos AN1x-kép dekódolása, ellenőrzött slotképzés, statikus regiszterírás-lista és összevetés a rögzített feltöltéssel.
- `research/mame-upload-control-experiment.patch`: a megfigyelő és a választható kísérleti profil a MAME-munkapéldányhoz.

Mind az öt saját CTest-bejegyzés sikeres; az új képadapter hozzáadása után a Python-csoport mind a 15 esete sikeres. A natív VOP3-visszajátszó a valódi felvétel végállapotát és a képadapter által előállított programot is betöltötte; a vizsgált állapotok egyeznek. A teljes patchlánc az eredeti MAME ZIP-ből újra előállította a munkapéldány hat módosított/új fájlját.

## Határok és következő feladat

A hangmag élő, időben összekötött H8–VOP3 működése, a táblarom megfelelő skálájú bekötése, a második DSP érzékelése és teljes inicializálása még nincs kész. A statikus képadapter íráslistája időzítés nélküli referencia, nem valós idejű hardverfeltöltés. Audio nem készült, a kísérleti státuszmodell nincs fizikai hardveren validálva.

Következő lépés a második DSP feltételflagjének eredete és az élő VOP3 host-interfész előkészítése; a most rögzített, teljes első program ad hozzá ellenőrzött betöltési referenciát. Mérési összefoglalók: `docs/upload-control-milestone/`. A nyers felvételek és firmware-eredetű állapotképek a `work/` alatt maradtak.
