

/* Required Connections
  --------------------
   pin 2:  LED Strip #1    OctoWS2811 drives 8 LED Strips.
   pin 14: LED strip #2    All 8 are the same length.
   pin 7:  LED strip #3
   pin 8:  LED strip #4    A 100 ohm resistor should used
   pin 6:  LED strip #5    between each Teensy pin and the
   pin 20: LED strip #6    wire to the LED strip, to minimize
   pin 21: LED strip #7    high frequency ringining & noise.
   pin 5:  LED strip #8
   pin 15 & 16 - Connect together, but do not use
   pin 4 - Do not use
   pin 3 - Do not use as PWM.  Normal use is ok.
   pin 1 - Output indicating CPU usage, monitor with an oscilloscope,
           logic analyzer or even an LED (brighter = CPU busier)

     ....   ....   ....   ....   ....   ....   ....   ....   ....   ....
 y72|    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  . |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  . |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  . |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  . |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  . |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  . |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  5 |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  4 |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  3 |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  2 |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |
  1 |    | |    | |    | |    | |    | |    | |    | |    | |    | |    |

  x 1    2 3    4 5    6 7    8 9   10 11  12 13  14 15  16 17  18 19   20
*/



#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>


//===============
#define NUM_LEDS_PER_STRIP 300
#define NUM_STRIPS 5
#define LONG_PRESS 1000
#define HOUR 3600*1000
#define DAY 3600*1000*24
#define RUNTIME 30*1000
const int buttonPin = 17;
const int ledPin = 13;

//added for fastLED
//const CRGBArray<1500> ledArray;
CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
CRGB leds_backup[NUM_STRIPS * NUM_LEDS_PER_STRIP];
CHSV IR_leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];
const int numOfBytes = NUM_LEDS_PER_STRIP * NUM_STRIPS * 3;
const int numLeds = NUM_STRIPS * NUM_LEDS_PER_STRIP;

char inputBuffer[numOfBytes];

int  Sail[21][80];
int  SailTop[11][7] = {73, 74, 75, 76, 77, 78};
elapsedMillis millis_elapsed;
elapsedMillis frame_millis_elapsed;
unsigned long millis_to_run;
unsigned long last_frame_millis;

// button stuff
int buttonPressedTime;

boolean cont;
boolean panelActive;

void setup() {
  Serial.begin(115200);
  delay(20);
  Serial.println("Initializing");


  pinMode(buttonPin, INPUT_PULLUP);
  LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
  LEDS.setBrightness(100);
  delay(400);
  Serial1.begin(19200);

  // initialize random generator
  randomSeed(millis());

  LEDS.show();

  makeArray();

  setupGIFs();

  // initializePanel();
  Serial.println("Initialization complete");

}

// call this before every sequence
void reset_for_next_pattern() {
  fill_solid( &(leds[0]), 1500, CRGB::Black );
  LEDS.setBrightness(255);

  millis_elapsed = 0;
  millis_to_run = RUNTIME;
  cont = true;
}

void loop() {

  //  if(Serial.available()>0){
  //  Serial.readBytes(inputBuffer, numOfBytes);

  // reset_for_next_pattern();
  // loopPanel();

  //PLACE THE NAME OF YOUR ROUTINE HERE.  It will run for 30 seconds.  If you need to adjust this, change millis_to_run above to the number of milliseconds you want it to run.
  //  Panel();
  // SequenceB();

  reset_for_next_pattern();
  playGIF();


  reset_for_next_pattern();
  CoolGradient();

  reset_for_next_pattern();
  WolfAlienSpaceship();

  reset_for_next_pattern();
  WolfAtariPong(true);

  reset_for_next_pattern();
  playGIF();

  reset_for_next_pattern();
  WolfBoxes();

  reset_for_next_pattern();
  WolfColorTunnel();

  reset_for_next_pattern();
  WolfColorWheel(true);  //true=black sector, false=all color

  reset_for_next_pattern();
  playGIF();

  reset_for_next_pattern();
  WolfComets(true);    //true=fade-to-black and reset; false=persistent color trails (very different looks!)

  reset_for_next_pattern();
  WolfEyeball();

  reset_for_next_pattern();
  WolfFractals();     //UNFINISHED: Use only if you are desperate for more patterns!

  reset_for_next_pattern();
  WolfFuzzScreen();

  reset_for_next_pattern();
  playGIF();

  reset_for_next_pattern();
  WolfMoire(true);

  reset_for_next_pattern();
  WolfPinwheel_1();

  reset_for_next_pattern();
  WolfPinwheel_2();

  reset_for_next_pattern();
  WolfSeaStorm();

  reset_for_next_pattern();
  WolfStarfield(false);   //true=color stars, false=white only

  reset_for_next_pattern();
  playGIF();

  reset_for_next_pattern();
  WolfSpiralStarfield(true, true); //true=color stars, false=white only (pattern looks best using(true,true))

  reset_for_next_pattern();
  Sounder();
}//end of loop


void makeArray() {
  int y, s;
  s = 0;

  for (int x = 1; x < 20; x += 2) {
    int l =  (150 * s);
    int  l2 = l + 149;
    for (y = 1; y <= 75; y++) {
      Sail[x][y] = l++;
      Sail[x + 1][y] = l2--;
    }
    s++;

  }

  SailTop[1][1] = 72;
  SailTop[1][2] = 73;
  SailTop[1][3] = 74;
  SailTop[1][4] = 75;
  SailTop[1][5] = 76;
  SailTop[1][6] = 77;

  for (int numsail = 2; numsail <= 10; numsail++) {
    for (int pos = 1; pos <= 6; pos++) {
      SailTop[numsail][pos] = SailTop[numsail - 1][pos] + 150;
    }
  }
}

CRGB hsv2rgb(CHSV hsv) {
  CRGB rgb;
  hsv2rgb_rainbow(hsv, rgb);
  return rgb;
}

bool Show() {
  // time's up
  if (millis_elapsed > millis_to_run) {
    cont = false;
    return cont;
  }

  // test if button press, short or long
  if (digitalRead(buttonPin) == LOW) {
    buttonPressedTime = millis();
    delay(20); // debounce
    while (digitalRead(buttonPin) == LOW) {
      delay(10);
    }
    if (millis() - buttonPressedTime >= LONG_PRESS) {
      millis_to_run = HOUR;
    } else {
      millis_to_run = RUNTIME;
      cont = false;
      return cont;
    }
  }


  /* This block deals with the reading and adding of IR board infromation*/
  // test
  if (Serial.read() == 'a') {
    IR_leds[Sail[1][1]] = CHSV(100, 255, 255);
    Serial.println("setting leds[Sail[1][1]] = CHSV(100, 255, 255);");
  }

  // store original LED colors
  memcpy(leds_backup, leds, NUM_STRIPS * NUM_LEDS_PER_STRIP * sizeof(CRGB)  );
    
  // Read from IR Panel
  receive();
  //for (int i = 0; i < NUM_STRIPS * NUM_LEDS_PER_STRIP; ++i) {
    //IR_leds[i].v = max(IR_leds[i].v - (int) frame_millis_elapsed, 0);
    // add layer to LEDs
    //leds[i] += hsv2rgb(IR_leds[i]);
    // print out debug information
    // if (IR_leds[i].v > 0)
    // {
    //   Serial.print("IR_leds[i].v = ");
    //   Serial.println(leds[i].g);
    //   Serial.print("frame_millis_elapsed = ");
    //   Serial.println(frame_millis_elapsed);
    // }
  //}

  // animations draw on top, from control panel
  static elapsedMillis milliseconds_since_last_draw;
  uint32_t msec = milliseconds_since_last_draw;
  milliseconds_since_last_draw = 0;
  float ms = (float)msec * 0.001;
  draw_sparks(ms);
  draw_deadarea(ms);
  
  LEDS.show();
  //restore original colors
  memcpy( leds, leds_backup, NUM_STRIPS * NUM_LEDS_PER_STRIP * sizeof(CRGB) );
  /* End IR block*/

  frame_millis_elapsed = 0;
  cont = true;
  return cont;
}



#define MAX_SPARKS 500

float spark_x[MAX_SPARKS];  // current position
float spark_y[MAX_SPARKS];
float spark_xspeed[MAX_SPARKS]; // current speed, in pixels/second
float spark_yspeed[MAX_SPARKS];
float spark_energy[MAX_SPARKS]; // current energy/brightness, 1.0=max, 0=dead
CRGB spark_color[MAX_SPARKS];

void draw_sparks(float ms)
{
  for (int i=0; i < MAX_SPARKS; i++) {
    if (spark_energy[i] <= 0) {
      // unused sparks have zero energy
      continue;
    }
    // decrease energy
    spark_energy[i] -= ms;
    if (spark_energy[i] <= 0) {
      // this spark has died out
      spark_energy[i] = 0;
      continue;
    }
    // move to new position
    spark_x[i] += spark_xspeed[i] * ms;
    if (spark_x[i] < 0.5 || spark_x[i] > 20.5) {
      spark_energy[i] = 0;
      continue;
    }
    spark_y[i] += spark_yspeed[i] * ms;
    if (spark_y[i] < 0.5 || spark_y[i] > 72.0) {
      spark_energy[i] = 0;
      continue;
    }
    // randomly create more sparks
    if (spark_energy[i] * ms * 1800.0 > random(2, 100)) {
      add_spark(spark_x[i], spark_y[i], spark_energy[i] * 0.9, spark_color[i]);
    }
    // actually draw the spark onto the pixels
    int x = (int)(spark_x[i] + 0.5);
    int y = (int)(spark_y[i] + 0.5);
    if (x >= 1 && x <= 20 && y >= 1 && y <= 72) {
      //float alpha = spark_energy[i];
      float alpha = sqrtf(spark_energy[i]);
      int index = Sail[x][y];
      unsigned int r = leds[index].r * (1.0 - alpha) + spark_color[i].r * alpha;
      unsigned int g = leds[index].g * (1.0 - alpha) + spark_color[i].g * alpha;
      unsigned int b = leds[index].b * (1.0 - alpha) + spark_color[i].b * alpha;
      if (r > 255) r = 255;
      if (g > 255) g = 255;
      if (b > 255) b = 255;
      leds[index] = CRGB(r, g, b);
    }
  }
}

float random_signed_float(float range)
{
  return range * (((float)random(2000000) / 1000000.0) - 1.0);
}

void add_spark(float x, float y, float energy, CRGB color)
{
  if (x < 1.0 || x > 20.0 || y < 1.0 || y > 72.0) return;
  if (energy < 0.01) return;
  if (energy > 1.0) energy = 1.0;
  int i;
  for (i=0; i < MAX_SPARKS; i++) {
    if (spark_energy[i] <= 0) break;
  }
  if (i >= MAX_SPARKS) {
    // can't add another spark - maximum number already animating
    return;
  }
  spark_x[i] = x;
  spark_y[i] = y;
  spark_xspeed[i] = random_signed_float(4.2);
  spark_yspeed[i] = random_signed_float(20.0);
  spark_energy[i] = energy;
  spark_color[i] = color;
}

void add_spark(float x, float y, float energy)
{
  CHSV random_color;
  random_color.h = random(256);
  random_color.s = random(200, 256);
  random_color.v = random(200, 256);
  add_spark(x, y, energy, hsv2rgb(random_color));
}


#define MAX_DEADAREA 50

int deadarea_x[MAX_DEADAREA];
int deadarea_y[MAX_DEADAREA];
float deadarea_fade[MAX_DEADAREA]; // time to fade area, in seconds

void draw_deadarea(float ms)
{
  ms *= 0.2;
  for (int i=0; i < MAX_DEADAREA; i++) {
    if (deadarea_fade[i] <= 0) continue;
    deadarea_fade[i] -= ms;
    if (deadarea_fade[i] <= 0.001) continue;
    int x = deadarea_x[i];
    int y = deadarea_y[i];
    float n = deadarea_fade[i];
    //Serial.printf("draw deadarea, x=%d, y=%d, n=%.2f\n", x, y, n);
    fade_led(x, y, n);
    fade_led(x, y - 1, n * 0.8);
    fade_led(x, y + 1, n * 0.8);
    fade_led(x, y - 2, n * 0.5);
    fade_led(x, y + 2, n * 0.5);
    fade_led(x, y - 3, n * 0.3);
    fade_led(x, y + 3, n * 0.3);
  }
}

void fade_led(int x, int y, float amount)
{
  if (amount <= 0) return;
  if (amount > 1.0) amount = 1.0;  
  if (x >= 1 && x <= 20 && y >= 1 && y <= 75) {
    int index = Sail[x][y];
    unsigned int r = leds[index].r * (1.0 - amount);
    unsigned int g = leds[index].g * (1.0 - amount);
    unsigned int b = leds[index].b * (1.0 - amount);
    leds[index] = CRGB(r, g, b);
  }
}

void add_deadarea(int x, int y, float amount)
{
  if (x < 1.0 || x > 20.0 || y < 1.0 || y > 75.0) return;
  if (amount < 0.001) return;
  if (amount > 1.0) amount = 1.0;
  //Serial.printf("add  deadarea, x=%d, y=%d, n=%.2f\n", x, y, amount);
  int i;
  for (i=0; i < MAX_DEADAREA; i++) {
    if (deadarea_fade[i] <= 0.001) break;
  }
  if (i >= MAX_DEADAREA) return;
  deadarea_x[i] = x;
  deadarea_y[i] = y;
  deadarea_fade[i] = amount;
}


