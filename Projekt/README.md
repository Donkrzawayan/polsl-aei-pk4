Tutaj umieszczać projekt

# TinyBusiness

W ramach projektu chciałbym napisać program dla magazynu z bazą towaru jaki jest na magazynie. Dodawanie towaru będzie poprzez wprowadzenie faktury z nim, a sprzedaż poprzez wystawienie faktury bądź paragonu dla klienta.
Faktury i paragony byłyby wystawiane w formacie XML, do którego chciałbym skorzystać z biblioteki TinyXML.
Ponieważ duża ilość tekstu potrzebna do wystawienia/przeglądania faktury byłaby mało czytelna na ekranie konsoli chciałbym się ograniczyć do jednego systemu (Windows) by mieć większą możliwość formatowania tekstu (kolory, wielkość okna konsoli itp.) i móc utrzymać kod bardziej czytelny.
Co do szczegółów to na towar składałaby się: jego nazwa, ilość, cena zakupu, cena sprzedaży oraz stawka VAT. Klasa faktura dziedziczyłaby po paragon i posiadała dodatkowo dane klienta.