# NeoPixSegment-clock
# **v2.08**<br>
**Cz** Drobné úpravy a opravy chyb.<br>
**EN** Minor improvements and bug fix.<br>
# **v2.07**<br>
**CZ**:<br>
Některé RTC moduly se zaseknou při pokusu o vyčtení teploty, tak jsem smazala tuhle část kódu. Použij v2.06 pokud tuto funkci chceš. Jiná změna není.
<br>
**EN**:<br>
Some RTC modules stop working when try to read temperature so I delete temp reading. Use 2.06 if you want this function. No other changes was made.
<br><br>
# **v2.06**<br>
**CZ**:<br>
Krátký stisk navigačního tlačítka zobrazí teplotu vyčtenou z modulu hodin. Zaokrouhleno na celé stupně Celsia.
<br>
**EN**:<br>
Short press nav button for temperature in °C rounded to integers. Temperature is read from RTC module.
<br><br>
# **v2.05**<br>
**CZ**:<br>
Arduino kód pro hodiny postavené na displeji NeoPixSegment.<br>
[NeoPixSegment - e-shop](https://www.vokolo.cz/neopixsegment/). [Knihovna k displeji - GitHub](https://github.com/Azuzula/NeoPixSegment)<br>
[3D tištěné díly ke stažení na Printables]([https://www.thingiverse.com/thing:4780832](https://www.printables.com/cs/model/280269-neopix-segment-hodiny))<br>
Funkce:<br>
Hodiny ukazují aktuální čas 😲<br>
Změna barvy navigačním tlačítkem, zmáčknout do stran. Barva se uloží až za 10s.<br>
Změna času navigačním tlačítkem. Zmáčkni střed alespoň na 3s. Potom změna čísel tlačítkem do stran, potvrdit středem tlačítka.<br>

Nastavení přes USB serial konzoli. Nastav 9600baud pro komunikaci, zakončení řádku NL (New line).<br>
Nastavení je v češtině. Kdyby měl někdo zájem udělat překlad do jiných jazyků, je vítán. Pošli mi hotový překlad, nebo vytvoř fork. Děkuji.<br>
Nastavení času<br>
Nastavení data<br>
Vlastní barva pro den i noc<br>
Pir senzor zap/vyp<br>
Časování doby svitu (když je PIR zapnutý)<br>
Minimální jas<br>
Modifikátor pro intenzitu jasu podle okolního světla<br>
Noční mód<br>
Dvojtečka bliká/svítí pro den i noc<br>
Reset do výchozího nastavení<br>

Zapojení pinů (napájecí napětí modulů):<br>
Arduino > NeoPix Segment displej: D3 > DIN, (5V)<br>
Arduino > TEMT6000: A0 > S, (5V)<br>
Arduino > DS3231: A4 > SDA, A5 > SCL, (5V)<br>
Arduino > PIR: D2 > OUT, (3,3V)<br>
Arduino > navigační 3Way tlačítko:  D5 > 1, D6 > T, D7 > 2, GND > C<br>


**EN**:<br>
Arduino code for clocks based on NeoPixSegment display.<br>
[NeoPixSegment - e-shop](https://www.vokolo.cz/neopixsegment/). [Library for display - GitHub](https://github.com/Azuzula/NeoPixSegment)<br>
[3D parts download from Printables]([https://www.thingiverse.com/thing:4780832](https://www.printables.com/cs/model/280269-neopix-segment-hodiny))
Functions: <br>
Clocks show actual time 😲<br>
Color change with 3way button. Push to the sides for color change. Color is saved after 10s.<br>
Time change with 3way button. Push mid button for atleast 3 sec, then use side and mid button for time change.<br>

Functions over USB serial monitor. Set 9600 baud for comunication and NL (new line) for end of line.<br>
Settings are in Czech language. If you want to translate it, you can. Just send me your translatation or make fork. Thank you<br>
Time set<br>
Date set<br>
Your own color for day and night<br>
Pir sensor on/off<br>
Light timing (with Pir on)<br>
Minimum brightness<br>
Brightness exponent<br>
Night mode on/off<br>
Coloon blink day or night on/off<br>
Factory reset<br>
