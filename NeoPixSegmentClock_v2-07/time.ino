void ParseTime() {
  Pix.WriteNumber(myNum[0], 0);
  Pix.WriteNumber(myNum[1], 1);
  Pix.WriteNumber(myNum[3], 3);
  Pix.WriteNumber(myNum[4], 4);
  if (myNum[0] == 0 & cSetup < 20) { // pokud je míň hodin než 10, první číslo zhasne
    Pix.Clear(0);
  }
}
