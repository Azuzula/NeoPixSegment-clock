void SleepNow() {
  byte JasStep = Jas / 10;
  //Serial.print("Jas_arr = "); Serial.println(JAS);
  //Serial.print("step = "); Serial.println(JasStep);
  JasTed = Jas;

  for (byte i = 0; i < 10; i++) {
    JasTed = JasTed - JasStep;
    //Serial.print("JasTed = "); Serial.println(JasTed);
    Pix.SetBrightness(JasTed);
    Pix.Show();
    delay(40);
  }
  Pix.SetBrightness(0);
  //Pix.Clear(ALL_SEGMENTS);
  Pix.Show();
}

void WakeUp() {
  //Serial.println("Vstávám");
  //Sleeping = 0;
  byte JasStep = Jas / 10;
  JasTed = 0;

  for (byte i = 0; i < 10; i++) {
    JasTed = JasTed + JasStep;
    //Serial.print("JasTed = "); Serial.println(JasTed);
    Pix.SetBrightness(JasTed);
    Pix.Show();
    delay(40);
  }
}
