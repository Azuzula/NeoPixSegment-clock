byte counter = 0;
byte data[sampl];

byte Jas_read() {
  int x = 0;
  int y = 0;
  y = analogRead(temt6000Pin);
  data[counter] = y;
  counter++;
  if (counter >= sampl) {
    counter = 0;
  }
  for (byte a = 0; a < sampl; a++) {
    x = x + data[a];
  }
  //Serial << "Analog:" << y << " Smooth:" << x/sampl << endl;

  //  if (counter >= sampl) {
  //    counter = 0;
  x = (x * (mod/10)) / sampl;
  
  if (x <= minimum) {
    x = minimum;
  }
  else if (x >= 255) {
    x = 255;
  }
  
  return x;
  
  //  }
  //  else {
  //    return Jas;
  //  }
}
