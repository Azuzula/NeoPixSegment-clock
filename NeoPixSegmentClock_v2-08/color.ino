void setColor () {
  byte R = (myColor[colorIndex - 1][0]);
  byte G = (myColor[colorIndex - 1][1]);
  byte B = (myColor[colorIndex - 1][2]);
  Pix.SetColor(R, G, B, ALL_SEGMENTS);
  //Pix.Show();

  //Serial << "Barva = " << R << ", " << G << ", " << B << " " << endl;
}
