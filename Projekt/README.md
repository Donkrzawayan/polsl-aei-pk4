Tutaj umieszczać projekt

# TinyBusiness

W ramach projektu chciałbym napisać program dla magazynu z bazą towaru jaki jest na magazynie i modyfikowanie go poprzez wystawienie paragonu/faktury lub wprowadzenie danych z faktury (podanie ścieżki do dokumentu XML).

Bazą danych jest klasa DataManager przechowująca wektor z obiektami Item oraz dane wlasciciela w formie obiektu Party. Paragon reprezentuje klasa Receipt, fakture, dziedzicząca po niej, Invoice.
Faktury i paragony byłyby wystawiane w formacie XML, do którego chciałbym skorzystać z biblioteki TinyXML.
Użytkownik będzie mógł zmieniać i przeglądać informacje o sobie (jako właścicielu), które będą na generowanych dokumentach.

Co do szczegółów to na towar (Item) składałaby się: jego nazwa, ilość, cena zakupu, cena sprzedaży oraz stawka VAT. Towar na stanie przed zapisaniem do pliku będzie sortowany po nazwie.
Klasa faktura (Invoice) dziedziczyłaby po paragon (Receipt) i posiadała dodatkowo dane klienta (obiekt Party). W czasie wystawiania faktury i paragonu jest możliwość anulowania, które nie zmieni bazy danych w stosunku do sprzed wystawienia.
Baza danych będzie zapisywana do pliku binarnego. W niej także będą zapisane informacje o właścicielu.
Klasy BaseData i Receipt/Invoice są skonstruowane tak, by mogły działać od siebie niezależnie i od użytkownika bedzie zależeć w jaki sposob obie maja wspolpracowac.
