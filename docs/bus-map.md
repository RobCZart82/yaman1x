# AN1x busztérkép – bizonyítékok és nyitott kérdések

Frissítés: az LC7985-mérföldkőben a firmware mindkét DSP-báziscímet (`0x440000`, `0x480000`) dinamikusan is elérte, 16 bites tranzakciókkal. Az alábbi korábbi statikus megállapításokat ez kiegészíti; teljes címtükrözési térképet még nem bizonyít. A bekötött LCD-portok leírása a `lcd-milestone.md` dokumentumban található.

2026-10-26. Az alábbi statikus alaptérképet a későbbi futtatási mérések egészítik ki. LCD-modell és választható VOP3-regiszterkapcsolat már van; a teljes perifériakészlet és a fizikai hardvervalidálás nincs kész.

| Tartomány / elem | Bizonyíték | Státusz |
|---|---|---|
| 0x000000–0x0FFFFF fő-ROM | Letöltött MAME yman1x.cpp és a helyi 1.04 kép | Bizonyított betöltési tartomány |
| 0x200000–0x23FFFF NVRAM | MAME driver, két HM628128 SRAM | MAME szerinti leképezés |
| 0xFFFD10–0xFFFF0F belső RAM 24 bites módban | MAME h83002_device::map | CPU-modell része; nem külső RAM |
| 0x440000 és 0x480000 | Firmware-mérés és rajz IC6 V31CS/V32CS | 16 bites elérések, mindkét DSP teljes programfeltöltése; teljes címtükrözés még nincs igazolva |
| H8 P92/RXD0, IC1 pin 16 | DM CN11 pin 1 → DMS CN1 pin 1 → DG | Aktív alacsony panelérzékelés; firmware Port 9 bit 2 → 0x23CBC3 jelenlétflag |
| DSP 0x22 (base + 0x44) | Firmware 0x0545E8 / 0x0515EE és trace | Kiválasztott csatorna előjeles szintje; külön kísérleti readback, fizikai hardvervalidálás nélkül |
| 0x400001 és közeli címek | Firmware-immediát minták, IC6 MEGCS | Effekt-interfész jelöltek; még nem megvalósított térkép |

## Kapcsolási rajz vizsgálata

A felhasználó AN1x-ServiceManual.pdf fájljának 51. PDF-oldalán teljes DM/DMS kapcsolási rajz található. Külön nagyítva ellenőriztük az IC6 74AC139 dekódert: az 1A/1B bemeneteken A18/A19, az 1Y0/1Y1/1Y2 kimeneteken MEGCS/V31CS/V32CS jelölések szerepelnek. A chipkiválasztás engedélyezését és minden tükrözött címtartományt még külön végig kell követni.

A főprocesszor H8/3002, a főpanelen és a DMS panelen is van VOP3. A fizikai billentyűzetvezérlő továbbra is külön, hiányzó belső ROM-mal szerepel.

## Firmware-indítás

A word-swap utáni resetvektor 0x000009D8. A resetcím környezetében a veremmutató 0x00FFFEFE értékét betöltő MOV.L mintája található. Ez a MAME 24 bites H83002 belső RAM-tartományába esik. A statikus mintakeresés önmagában nem bizonyít végrehajtott vezérlési utat.

A DSP-báziscímek statikus azonosításához 16 bites határokon 7A 00..07 opkód utáni 32 bites értékeket vizsgáltunk. Adattáblákban is lehet ilyen minta, ezért ezek csak címjelöltek. A H8-kutatási futtató dinamikus megfigyelése szükséges a megerősítéshez.

## Korábbi mérési terv

Az első LCD nélküli megfigyelést később LC7985-bekötés, teljes DSP-regiszternapló, külön kísérleti feltöltésvezérlés és élő VOP3-regiszterkapcsolat követte. Az alapbeállítás továbbra is passzív; a kísérleti válaszok csak explicit profilban aktívak. Aktuális eredmények és következő mérés: [élő regiszterkapcsolat](live-register-milestone.md).

Legfrissebb vizsgálat: [EG-szint és mintaszámláló](eg-readback-milestone.md).
