void IRPlay(int irX, int irY, int irDist, int irSize, int irMass)
{
  //if(irMass>20){
  //int X = min(((irX + 1) / 4.3) + 1, 20);
  //int Y = min(75, ((95 - irY) * .8));
  int X = (1.0 - (float)irX / 100.0) * 21.0 - 0.5;
  int Y = (1.0 - (float)irY / 100.0) * 72.0 + 1.0;
  if (X < 1) X = 1;
  if (X > 20) X = 20;
  if (Y < 1) Y = 1;
  if (Y > 72) Y = 72;
  int Dist = irDist;
  int Mass = irMass;
  // CRGB
  // int red=min(Dist*Mass*1.1,255);
  // int green= min(Dist*1,255);
  // int blue= min(Dist*1.6,255);
  // CHSV
  int hue = min(Dist * Mass * 1.1, 255);
  int sat = min(Dist * 1, 255);
  int val = min(Dist * 1.6, 255);

  IR_leds[Sail[X][Y]] = CHSV(hue, sat, val);

  if (Mass > 20 && Dist > 14 && Dist < 200) {
    float e = ((float)Dist - 14.0) * (1.0 / (200.0 - 14.0));
    e = sqrtf(1.0 - e) + 0.8;
    add_spark(X, Y, e);
  }
  if (Mass > 150 && Dist >= 180) {
    float n = (float)(Dist - 180) / 70.0;  // 0 to 1.0
    n = (1.0 - n) * 5.12;
    add_deadarea(X, Y, n);
  }
  // LEDS.show();
  // addSquare(X,Y,irMass);
  //}
  // fadeToBlackBy(leds,1499, 40);
}

void addSquare(int x, int y, int mass) {

  for (int xx = x - 2; xx < x + 2; ++xx)
  {
    if ((xx < 1) || (xx > 20)) continue;
    for (int yy = y - mass / 3; yy < y + mass / 3; ++yy)
    {
      if ((yy < 1) || (yy > 72)) continue;
      IR_leds[Sail[xx][yy]] = CHSV(100, 255, mass);
    }
  }

}
