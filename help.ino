#define myStrLen 5
String myStr[myStrLen] = "";
byte x = 0;

void serialEvent() {
  //Serial << "Serial event" << endl;
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    //Serial << "Serial.read = " << inChar << endl;
    if ((inChar == ':') || (inChar == ' ')) {
      x++;
    }
    else if (inChar != '\n') {
      myStr[x] += inChar;
    }
    else if (inChar == '\n') {
      stringComplete = true;
    }
  }
  if (stringComplete) {
    //    Serial << F("Příkaz = ") << myStr[0] << endl;
    //    for (byte y = 1; y < myStrLen; y++) {
    //      Serial << F("hodnota ") << y << F(" = ") << myStr[y] << endl;
    //    }

    zakladniNastaveni();
    stringComplete = false;
    for (byte y = 0; y < myStrLen; y++) {
      myStr[y] = "";
    }
    x = 0;
  }
}

void zakladniNastaveni () {
  if ((myStr[0] == "help") || (myStr[0] == "Help")) {
    Serial << F("Hodiny lze ovládat navigačním tlačítkem vzadu u konektoru napájení. Vlevo a vpravo se vybírá jedna z předvolených barev + jedna vlastní konfigurovatelná.") << endl;
    Serial << F("Stiskem tlačítka delším, než 2s se dostaneš do nastavení času, dalším krátkým stiskem se přechází po jednotlivých číslicích.") << endl;
    Serial << F("Tlačítkem vlevo/vpravo měníš čísla. Potvrzením poslední číslice se čas uloží. Vteřiny se resetují.") << endl;
    Serial << endl;
    Serial << F("Detailní nastavení různých hodnot a funkcí") << endl;
    Serial << F("Příkaz:možné hodnoty [výchozí hodnota] // nápověda") << endl;
    Serial << F("Například pro vypnutí PIR senzoru napiš Pir:0") << endl;
    Serial << F("Jako oddělovač příkazu a hodnot jde použít i mezera") << endl;
    Serial << endl;
    Serial << F("Pir:0, 1 [1] // Vypne(0)/zapne(1) PIR senzor") << endl;
    Serial << F("Svit:1 až 255 [8] // Doba po kterou jsou hodiny rozsvícené od konce detekce pohybu. Tedy 2,5s + Svit") << endl;
    Serial << F("JasOfset:1 až 25 [2.5] // Nastaví jak moc má displej svítit v závislosti na okolním osvětlení.") << endl;
    Serial << F("JasMin:1 až 255 [2] // Minimální jas ve tmě") << endl;
    Serial << F("Barva:0 až ") << colorArray << F(" // Barevné schéma - jde přepínat i ovládacím tlačítkem vzadu na hodinách.") << endl;
    Serial << F("R:0 až 255 [100] // Vlastní nastavení barvy: červená složka") << endl;
    Serial << F("G:0 až 255 [100] // Vlastní nastavení barvy: zelená složka") << endl;
    Serial << F("B:0 až 255 [100] // Vlastní nastavení barvy: modrá složka") << endl;
    Serial << F("Noc:0, 1 [0] // V noci svítí jiná barva než přes den. Přepíná se podle intenzity světla.") << endl;
    Serial << F("NR:0 až 255 [150] // Vlastní nastavení barvy v noci: červená složka") << endl;
    Serial << F("NG:0 až 255 [0] // Vlastní nastavení barvy v noci: zelená složka") << endl;
    Serial << F("NB:0 až 255 [0] // Vlastní nastavení barvy v noci: modrá složka") << endl;
    Serial << F("DT:0, 1 [1] // Dvojtečka svítí(0), nebo bliká(1)") << endl;
    Serial << F("NDT:0, 1 [1] // Dvojtečka v noci svítí (0), nebo bliká (1)") << endl;
    Serial << F("DtBlik:1 až 255 [5] // Blikání dvojtečky ve stovkách ms. 1 = 100ms, 10 = 1000ms = 1s") << endl;
    Serial << endl;
    Serial << F("Nastavení data a času. Resetem do výchozího nastavení se neresetuje čas.") << endl;
    Serial << F("Datum:23:9:1987 // Formát je DD:MM:RRRR") << endl;
    Serial << F("Cas:12:34:56 // Formát času hh:mm:ss") << endl;
    Serial << endl;
    Serial << F("? // Otazník vypíše všechna aktuální nastavení") << endl;
    Serial << F("Reset:255 // Vyresetuje hodnoty do výchozího nastavení uvedené v hranatých závorkách.") << endl;
    //Serial<<F("Save // Uloží všechna nastavení")<<endl;
  }
  if (myStr[0] == "Pir") {
    EEPROM.update(PIR,  myStr[1].toInt());
    pirActive =  myStr[1].toInt();
    Serial << F("Senzor PIR je ") <<  ((bool)myStr[1].toInt() ? F("zapnutý") : F("vypnutý")) << endl;
  }
  else if (myStr[0] == "Svit") {
    EEPROM.update(SVIT,  myStr[1].toInt());
    svit =  myStr[1].toInt();
    Serial << F("Hodiny svítí po dobu ") <<  myStr[1].toInt() << F("*100 ms") << endl;
  }
  else if (myStr[0] == "JasOfset") {
    if ( myStr[1].toDouble() > 25) {
      EEPROM.update(JAS_OFSET, 250);
      mod = 25.0;
      Serial << F("Hodnota JasOfset nastavena na 25") << endl;
    }
    else {
      EEPROM.update(JAS_OFSET,  myStr[1].toDouble() * 10);
      mod = (float)EEPROM.read(JAS_OFSET) / 10;
      Serial << F("Hodnota JasOfset nastavena na ") << (float) myStr[1].toDouble() << endl;
      //Serial << F("Hodnota v EEPROM paměti je: ") << EEPROM.read(JAS_OFSET) << endl;
      //Serial << F("Hodnota po vyčtení z paměti je ") << (float)EEPROM.read(JAS_OFSET) / 10 << endl;
    }
  }
  else if (myStr[0] == "JasMin") {
    EEPROM.update(JAS_MIN,  myStr[1].toInt());
    minimum =  myStr[1].toInt();
    Serial << F("Hodnota JasMin nastavena na ") <<  myStr[1].toInt() << endl;
  }
  else if (myStr[0] == "R") {
    EEPROM.update(RED,  myStr[1].toInt());
    myColor[12][0] =  myStr[1].toInt();
    colorIndex = 13; setColor ();
    Serial << F("Barva Red nastavena na ") <<  myStr[1].toInt() << endl;
  }
  else if (myStr[0] == "G") {
    EEPROM.update(GREEN,  myStr[1].toInt());
    myColor[12][1] =  myStr[1].toInt();
    colorIndex = 13; setColor ();
    Serial << F("Barva Green nastavena na ") <<  myStr[1].toInt() << endl;
  }
  else if (myStr[0] == "B") {
    EEPROM.update(BLUE,  myStr[1].toInt());
    myColor[12][2] =  myStr[1].toInt();
    colorIndex = 13; setColor ();
    Serial << F("Barva Blue nastavena na ") <<  myStr[1].toInt() << endl;
  }
  else if (myStr[0] == "Noc") {
    EEPROM.update(NOC,  myStr[1].toInt());
    if (myStr[2] != "" && myStr[3] != "" && myStr[4] != "") {

      EEPROM.update(NGREEN,  myStr[3].toInt());
      EEPROM.update(NBLUE,  myStr[4].toInt());

      myColor[13][1] =  myStr[3].toInt();
      myColor[13][2] =  myStr[4].toInt();
    }
    Serial << F("Přepnutí barvy na noc je ") << ((bool)EEPROM.read(NOC) ? F("zapnuto") : F("vypnuto")) << endl;
  }
  else if (myStr[0] == "NR") {
    EEPROM.update(NRED,  myStr[1].toInt());
    myColor[13][0] =  myStr[1].toInt();
    Serial << F("Barva Red nastavena na ") << EEPROM.read(NRED) << endl;
  }
  else if (myStr[0] == "NR") {
    EEPROM.update(NGREEN,  myStr[1].toInt());
    myColor[13][1] =  myStr[1].toInt();
    Serial << F("Barva Green nastavena na ") <<   EEPROM.read(NGREEN) << endl;
  }
  else if (myStr[0] == "NR") {
    EEPROM.update(NRED,  myStr[1].toInt());
    myColor[13][2] =  myStr[1].toInt();
    Serial << F("Barva Blue nastavena na ") <<  EEPROM.read(NBLUE) << endl;
  }
  else if (myStr[0] == "DT") {
    EEPROM.update(COLON, (bool)myStr[1].toInt());
    colon = (bool)EEPROM.read(COLON);
    //cColon = (bool)EEPROM.read(COLON);
    Serial << "Dvojtečka ve dne " << ((bool)EEPROM.read(COLON) ? F("bliká") : F("svítí")) << endl;
  }
  else if (myStr[0] == "NDT") {
    EEPROM.update(NCOLON, (bool)myStr[1].toInt());
    colon = (bool)EEPROM.read(NCOLON);
    //cColon = (bool)EEPROM.read(NCOLON);
    Serial << "Dvojtečka v noci " << ((bool)EEPROM.read(NCOLON) ? F("bliká") : F("svítí")) << endl;
  }
  else if (myStr[0] == "DtBlik") {
    EEPROM.update(COLON_BLINK, myStr[1].toInt());
    dtBlik = EEPROM.read(COLON_BLINK);
    Serial << F("Dvojtečka bliká rychlostí ") << ((int)dtBlik * 100) << F("ms") << endl;
  }
  else if (myStr[0] == "Datum") {
    dt = clock.getDateTime();
    clock.setDateTime(myStr[3].toInt(), myStr[2].toInt(), myStr[1].toInt(), dt.hour, dt.minute, dt.second);
    dt = clock.getDateTime();
    Serial << F("Datum je nastavený na ") << (int)dt.day << F(".") << (int)dt.month << F(".") << (int)dt.year << endl;
  }
  else if (myStr[0] == "Cas") {
    dt = clock.getDateTime();
    clock.setDateTime(dt.year, dt.month, dt.day,  myStr[1].toInt(),  myStr[2].toInt(),  myStr[3].toInt());
    Serial << F("Čas je nastavený. Vidíš ho přímo na displeji ;)") << endl;
  }
  else if (myStr[0] == "Demo") {
    //demo = !demo;
    if (!demo) {
      demo = 1;
      test = 0;
      Serial << F("Demo běží") << endl;
    }
    else {
      Serial << F("Demo zastaveno") << endl;
      colorIndex = EEPROM.read(COLOR_INDEX);
      demo = 0;
      test = 0;
    }
  }
  else if (myStr[0] == "Test") {
    if (!test) {
      demo = 1;
      test = 1;
      Serial << F("Test běží") << endl;
    }
    else {
      demo = 0;
      test = 0;
      Serial << F("Test zastaveno") << endl;
      colorIndex = EEPROM.read(COLOR_INDEX);
    }
  }
  
  else if (myStr[0] == "Reset" &&  myStr[1].toInt() == 255) {
    RESET();
    myStr[0] = "?";
  }
  else if (myStr[0] == "?") {
    Serial << endl << F("Výpis uložených hodnot") << endl;
    Serial << F("Pir je ") << ((bool)EEPROM.read(PIR) ? F("aktivní") : F("neaktivní")) << endl;
    Serial << F("Hodiny svítí ") << EEPROM.read(SVIT) << F("+2,5 sekund po detekci pohybu") << endl;
    Serial << F("JasOfset = ") << (float)EEPROM.read(JAS_OFSET) / 10 << endl;
    Serial << F("JasMin = ") << EEPROM.read(JAS_MIN) << endl;
    Serial << F("Aktuální barva je R:") << myColor[colorIndex - 1][0] << F(" G:") << myColor[colorIndex - 1][1] << F(" B:") << myColor[colorIndex - 1][2] << endl;
    Serial << F("Vlastní barva je R:") << EEPROM.read(RED) << F(" G:") << EEPROM.read(GREEN) << F(" B:") << EEPROM.read(BLUE) << endl;
    Serial << F("Noční barva je R:") << EEPROM.read(NRED) << F(" G:") << EEPROM.read(NGREEN) << F(" B:") << EEPROM.read(NBLUE) << endl;
    Serial << F("Přepínání na noční barvu je ") << (EEPROM.read(NOC) ? F("aktivní") : F("neaktivní")) << endl;
    Serial << F("Dvojtečka ve dne ") << ((bool) EEPROM.read(COLON) ? F("bliká") : F("svítí")) << endl;
    Serial << F("Dvojtečka v noci ") << ((bool) EEPROM.read(NCOLON) ? F("bliká") : F("svítí")) << endl;
    Serial << F("Rychlost blikání dvojtečky je ") <<  EEPROM.read(COLON_BLINK) * 100 << "ms" << endl;
    dt = clock.getDateTime();
    Serial << F("Aktuální nastavené datum je ") << dt.day << "." << dt.month << "." << dt.year << endl;
  }
  else {
    Serial << F("Příkaz nerozpoznán: ") << myStr[0] << endl;
  }
}

void RESET() {
  Serial << F("Reset do výchozího nastavení proveden") << endl;
  EEPROM.update(PIR, 1);
  EEPROM.update(SVIT, 10);
  EEPROM.update(JAS_OFSET, 25);
  EEPROM.update(JAS_MIN, 1);
  EEPROM.update(COLOR_INDEX,0);
  EEPROM.update(RED, 200);
  EEPROM.update(GREEN, 200);
  EEPROM.update(BLUE, 200);
  EEPROM.update(NRED, 150);
  EEPROM.update(NGREEN, 0);
  EEPROM.update(NBLUE, 0);
  EEPROM.update(COLON, 1);
  EEPROM.update(NCOLON, 1);
  EEPROM.update(COLON_BLINK, 5);

  pirActive = 1;
  svit = 8;
  mod = 2.5;
  minimum = 1;
  colorIndex = EEPROM.read(COLOR_INDEX);
  myColor[colorIndex - 1][0] = 200;
  myColor[colorIndex - 1][1] = 200;
  myColor[colorIndex - 1][2] = 200;
  setColor();
  colon = true;
  dtBlik = 5;
}
