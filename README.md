# Descan ![alt text](Descan/icons/skener.png "Scanner")

Descan je studentski projekat rađen na kursu Razvoj Softvera u okviru [Matematickog fakulteta](http://www.matf.bg.ac.rs/)  u Beogradu. Glavni cilj ovog softvera je da korisnik na veoma lak i brz način prebaci svoje slike u pdf fajl. Osim ove osnovne funkcionalnosti korisniku je omogućeno da slikama promeni dimenziju, kao i da doda razne efekte. Nakon toga korisnik može svoj pdf fajl da dodatno kompresuje(kako bi smanjio njegovu veličinu) kao i da pošalje na mejl ili da stavi na svoj Google Drive. Osim rada sa slikama, ovaj softver omogućava rad sa pdf fajlovima, i to opcije spajanja više pdf fajlova u jedan, kao i razdvajanje jednog pdf fajla na više delova. 

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
