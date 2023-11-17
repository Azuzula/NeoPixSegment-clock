void showTemp(){
  Pix.Clear(ALL_SEGMENTS);
  clock.forceConversion();
  char tmpS[5];
  sprintf(tmpS,"%i:%s",(int)clock.readTemperature(),"*C");
  Serial << F("Teplota: ") << tmpS << endl;
  Pix.WriteText(tmpS, LEFT_ALIGMENT);
  Pix.WriteDotColon(0, 2);
  Pix.Show();
  delay(5000);
}
