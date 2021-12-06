## Dijagram slučajeva upotrebe
![](dijagram_slucajeva_upotrebe.png) 
___

### Slučaj upotrebe Učitavanje slika
_Kratak opis:_ Korisnik bira da li će jednu ili više slika sa svog računara konvertovati u PDF dokument.

_Akteri:_ Korisnik

_Preduslovi:_ Aplikacija je pokrenuta.

_Postuslovi:_ Slike su učitane – prikazuju se u aplikaciji i spremne su za editovanje.

_Osnovni tok:_ 

1. Korisnik bira dugme "Import image" ili dugme "Choose multiple images" iz glavnog menija.
2. Aplikacija prikazuje dijalog za odabir fajlova.
<br>&emsp; 2.1. Ako je korisnik odabrao da učita samo jednu sliku:
<br>&emsp;&emsp;&emsp; 2.1.1. Aplikacija mu dozvoljava da odabere tačno jedan fajl sa nekom od sledećih ekstenzija: .jpg, .jpeg, .png, .bmp.
<br>&emsp;&emsp;&emsp; 2.1.2. Prelazi se na korak 3.
<br>&emsp; 2.2. Ako je korisnik odabrao da učita više slika:
<br>&emsp;&emsp;&emsp; 2.2.1. Aplikacija mu dozvoljava da odabere više fajlova sa nekom od sledećih ekstenzija: .jpg, .jpeg, .png, .bmp.
<br>&emsp;&emsp;&emsp; 2.2.2. Prelazi se na korak 3.
3. Aplikacija proverava da li su odabrani fajlovi ispravni.
<br>&emsp; 3.1. Ako su odabrani fajlovi ispravni:
<br>&emsp;&emsp;&emsp; 3.1.1. Aplikacija obaveštava korisnika da su slike uspešno učitane.
<br>&emsp;&emsp;&emsp; 3.1.2. Aplikacija omogućava korisniku da odabere dugme "Next".
<br>&emsp;&emsp;&emsp; 3.1.3. Korisnik bira dugme "Next" i prelazi u prozor za editovanje slika.
<br>&emsp;&emsp;&emsp; 3.1.4. Prelazi se na slučaj upotrebe Editovanje slika.
<br>&emsp; 3.2. Ako odabrani fajlovi nisu ispravni:
<br>&emsp;&emsp;&emsp; 3.2.1. Aplikacija obaveštava korisnika da je nemoguće učitati slike.
<br>&emsp;&emsp;&emsp; 3.2.2. Prelazi se na korak 1.

_Alternativni tokovi:_
* A1: **Neočekivani izlaz iz aplikacije.** Ako u bilo kom koraku korisnik isključi aplikaciju, sve što je eventualno bilo učitano će se poništiti i aplikacija će završiti sa radom. Slučaj upotrebe se završava.

* A2: **Korisnik se predomislio.** Ako u drugom ili trećem koraku korisnik shvati da ne želi da učitava slike i konvertuje ih u PDF dokument, već želi da uradi split dokumenta ili merge dokumenata, može odabrati dugme "Split PDF" odnosno dugme "Merge PDF" iz glavnog menija. Sve što je eventualno bilo učitano se poništava i prelazi se na slučaj upotrebe Split dokumenta, odnosno slučaj upotrebe Merge dokumenata.

_Podtokovi:_ /

_Specijalni zahtevi:_ /

_Dodatne informacije:_ /

___
### Slučaj upotrebe 
_Kratak opis_: 

_Akteri_: 

_Preduslovi_:

_Postuslovi_:

_Osnovni tok_:

_Alternativni tokovi_:

_Podtokovi_:

_Specijalni zahtevi_:

_Dodatne informacije_:
___
