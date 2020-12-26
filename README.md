# Descan ![alt text](Descan/icons/skener.png "Scanner")

Descan je studentski projekat rađen na kursu Razvoj Softvera u okviru [Matematičkog fakulteta u Beogradu](http://www.matf.bg.ac.rs/). Glavni cilj ovog softvera je da korisnik na veoma lak i brz način konvertuje svoje slike u pdf dokument. Osim ove osnovne funkcionalnosti, korisniku je omogućeno da slikama promeni dimenziju i da primeni razne efekte. Nakon toga korisnik može dodatno svoj pdf dokument da kompresuje (smanji njegovu veličinu), da pošalje na mejl ili da postavi na Google Drive. Osim rada sa slikama, omogućen je i rad sa pdf dokumentima i to opcije spajanja više dokumenata u jedan, kao i razdvajanja jednog dokumenta u više. 

## Korišćene biblioteke
- Qt 5.15
- PDFNetC64
- libcurl4
- Google drive API

## Instalacija :computer:
- Klikom na [link](https://www.pdftron.com/documentation/linux/get-started/cpp/?fbclid=IwAR3byDmn_aiA3t-oSBWjdNNQOWxObSR9L4_DnB5zn0iD-azKSPebJpNAuW4) otvara se prozor odakle se preuzima biblioteka PDFNetC64. Raspakovan fajl treba ubaciti u klonirani direktorujum projekta.
- Biblioteka libcurl4 se instalira na sledeći način:
  1. `sudo apt update`
  2. `sudo apt install curl`
  3. `sudo apt install libcurl4-openssl-dev`

## Developers

- [Jelena Milivojević, 4/2016](https://gitlab.com/ratspeaker)
- [Anđela Milićević, 53/2016](https://gitlab.com/andjaam)
- [Dušica Golubović, 119/2016](https://gitlab.com/golubovicd)
- [Marija Marković, 76/2016](https://gitlab.com/marija.markovic)
