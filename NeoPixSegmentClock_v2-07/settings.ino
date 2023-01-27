void Settings() {
  byte setNum = 0;
  ParseTime();
  Pix.SetBlinkInterval(300);
  Pix.SegmentBlink(1, 0);

  while (setNum < 5) {
    if (!digitalRead(settPin)) {
      delay(debounce);
      Pix.SegmentBlink(0, ALL_SEGMENTS);
      setNum++;
      if (setNum == 2) {
        setNum = 3;
      }
      Pix.SegmentBlink(1, setNum);
    }

    if (!digitalRead(plusPin)) {
      //Serial.println("+ btn");
      delay(debounce);
      myNum[setNum]++;

      if (setNum == 0) { // desítky hodin 0-2
        if (myNum[setNum] > 2) {
          myNum[setNum] = 0;
        }
      }
      else if (setNum == 1) { // jednotky hodin
        if (myNum[0] < 2) { // hodiny  < 20
          if (myNum[setNum] > 9) {
            myNum[setNum] = 0;
          }
        }
        else if (myNum[0] == 2) { // hodiny > 20
          if (myNum[setNum] > 3) {
            myNum[setNum] = 0;
          }
        }
      }
      else if (setNum == 3) { // desítky minut 0-6
        if (myNum[setNum] > 5) {
          myNum[setNum] = 0;
        }
      }
      else if (setNum == 4) { // jednotky minut 0-9
        if (myNum[setNum] > 9) {
          myNum[setNum] = 0;
        }
      }
      //Serial << "+číslo = " << myNum[setNum] << endl;
    }

    if (!digitalRead(minusPin)) {
      //Serial.println("- btn");
      delay(debounce);
      myNum[setNum]--;
      if (setNum == 0) { // desítky hodin 0-2
        if (myNum[setNum] < 0) {
          myNum[setNum] = 2;
        }
      }
      else if (setNum == 1) { // jednotky hodin
        if ((myNum[0] < 2)) { // hodiny  < 20
          if (myNum[setNum] < 0) {
            myNum[setNum] = 9;
          }
        }
        else if (myNum[0] == 2) { // hodiny > 20
          if (myNum[setNum] < 0) {
            myNum[setNum] = 3;
          }
        }
      }
      else if (setNum == 3) { // desítky minut 0-6
        if (myNum[setNum] < 0) {
          myNum[setNum] = 5;
        }
      }
      else if (setNum == 4) { // jednotky minut 0-9
        if (myNum[setNum] < 0) {
          myNum[setNum] = 9;
        }
      }
      //Serial << "-číslo = " << myNum[setNum] << endl;
    }
    ParseTime();
    Pix.Show();
  }
  byte setHour = (myNum[0] * 10) + myNum[1];
  byte setMinute = (myNum[3] * 10) + myNum[4];

  // Manual (YYYY, MM, DD, HH, II, SS
  //Serial << "Čas uložen: " << setHour << ":" << setMinute << endl;
  clock.setDateTime(dt.year, dt.month, dt.day, setHour, setMinute, 0);
  Pix.SegmentBlink(0, ALL_SEGMENTS);
  Pix.Show();
}
