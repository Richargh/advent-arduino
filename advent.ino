/*********************************************************************
Christmas example, based on the Adafruit example code
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define STERN_HEIGHT 16
#define STERN_WIDTH  16
static const unsigned char PROGMEM stern_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

#define SANTA_WIDTH    48
#define SANTA_HEIGHT   48
// https://javl.github.io/image2cpp/ hintergrund schwarz mit weisser Figur
static const unsigned char PROGMEM santa_bmp [] = {
  0x00, 0x00, 0x0f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x80, 0x80, 0x00, 0x03, 0xe0, 0x03, 
  0xf3, 0xf0, 0x00, 0x07, 0x00, 0x00, 0x7f, 0x38, 0x00, 0x0e, 0x00, 0x00, 0x0e, 0x18, 0x00, 0x18, 
  0x00, 0x00, 0x0c, 0x0c, 0x00, 0x30, 0x00, 0x01, 0x8c, 0x0c, 0x00, 0x70, 0x00, 0x01, 0xcc, 0x18, 
  0x00, 0x60, 0x00, 0x00, 0xcf, 0x38, 0x00, 0x60, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0xc0, 0x00, 0x00, 
  0x70, 0xc0, 0x00, 0xc0, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x30, 0x00, 0x00, 0xc0, 
  0x00, 0x00, 0x30, 0x00, 0x01, 0xff, 0xc6, 0x7f, 0xf8, 0x00, 0x03, 0xff, 0xe6, 0x7f, 0xfc, 0x00, 
  0x03, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x03, 0x00, 0x00, 0x00, 
  0x0c, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x00, 0x1f, 0xb0, 
  0x00, 0x00, 0xdf, 0x80, 0x19, 0xa0, 0x00, 0x00, 0xd9, 0x80, 0x31, 0xa0, 0x00, 0x00, 0xd9, 0x80, 
  0x33, 0x31, 0x80, 0x10, 0xd9, 0x80, 0x1b, 0x30, 0x80, 0x10, 0xc9, 0x80, 0x1f, 0x30, 0x3f, 0xc0, 
  0xcf, 0x80, 0x0f, 0x39, 0xf9, 0xf9, 0xcf, 0x00, 0x03, 0x1f, 0xc0, 0x3f, 0x8c, 0x00, 0x03, 0x00, 
  0x00, 0x00, 0x0c, 0x00, 0x03, 0x00, 0x30, 0xc0, 0x0c, 0x00, 0x03, 0x00, 0x1f, 0x80, 0x0c, 0x00, 
  0x06, 0x00, 0x0f, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 
  0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 
  0x00, 0x00, 0x06, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x03, 0x80, 0x00, 0x00, 0x18, 0x00, 
  0x01, 0xe0, 0x00, 0x00, 0x78, 0x00, 0x00, 0xfe, 0x00, 0x07, 0xe0, 0x00, 0x00, 0x1f, 0x00, 0x0f, 
  0x80, 0x00, 0x00, 0x01, 0x80, 0x18, 0x00, 0x00, 0x00, 0x01, 0xc0, 0x38, 0x00, 0x00, 0x00, 0x00, 
  0xe0, 0x70, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x00, 0x00
};

#if (SSD1306_LCDHEIGHT != 48)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

const int button1Pin = 2;
const int button2Pin = 3;
const int button3Pin = 4;
const int button4Pin = 5;

enum Zustand {
    WARTE, MACHE_INTERAKTION, ZEICHNE_STERNE
};

Zustand zustand;
long letzerZustandInMillis;

void setzeZustand(Zustand neuerZustand) {
  zustand = neuerZustand;
  letzerZustandInMillis = millis();
}

long millisSeitLetztemZustand() {
  return millis() - letzerZustandInMillis;
}

void setup()   {
  Serial.begin(9600);

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)

  setzeZustand(WARTE);
}

void loop() {
  int button1State = digitalRead(button1Pin);
  int button2State = digitalRead(button2Pin);
  int button3State = digitalRead(button3Pin);
  int button4State = digitalRead(button4Pin);

  if (button1State == LOW) {
    setzeZustand(MACHE_INTERAKTION);
    onButton1Push();
    setzeZustand(WARTE);
  } else if(button2State == LOW) {
    setzeZustand(MACHE_INTERAKTION);
    onButton2Push();
    setzeZustand(WARTE);
  } else if(button3State == LOW) {
    setzeZustand(MACHE_INTERAKTION);
    onButton3Push();
    setzeZustand(WARTE);
  } else if(button4State == LOW) {
    setzeZustand(WARTE);
  }

  if(zustand == WARTE && millisSeitLetztemZustand() > 3000){
    initiereSterne(stern_bmp, STERN_HEIGHT, STERN_WIDTH);
    setzeZustand(ZEICHNE_STERNE);
  }
  if(zustand == ZEICHNE_STERNE){
    zeichneSterne(stern_bmp, STERN_HEIGHT, STERN_WIDTH);
  }
}

void onButton1Push(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0,0);
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.println("Foto Mit");
  display.display();
  delay(3000);
  zeichneBitmap(santa_bmp, SANTA_HEIGHT, SANTA_WIDTH);
  delay(1000);
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
}

void onButton2Push(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Hello, world!");
  display.setTextColor(BLACK, WHITE); // 'inverted' text
  display.println("Foto Mit");
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("0x"); display.println(0xDEADBEEF, HEX);
  display.display();
}

void onButton3Push(){
  zeichneBitmap(santa_bmp, SANTA_HEIGHT, SANTA_WIDTH);
  delay(1);

  // invert the display
  display.invertDisplay(true);
  delay(1000);
  display.invertDisplay(false);
}

void onButton4Push(){

}

uint8_t icons[NUMFLAKES][3];

void initiereSterne(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(display.width());
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;

    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  return icons;
}

void zeichneSterne(const uint8_t *bitmap, uint8_t w, uint8_t h){
  display.clearDisplay();
  // draw each icon
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, WHITE);
  }
  display.display();
  delay(200);

  // then erase it + move it
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    display.drawBitmap(icons[f][XPOS], icons[f][YPOS], bitmap, w, h, BLACK);
    // move it
    icons[f][YPOS] += icons[f][DELTAY];
    // if its gone, reinit
    if (icons[f][YPOS] > display.height()) {
      icons[f][XPOS] = random(display.width());
      icons[f][YPOS] = 0;
      icons[f][DELTAY] = random(5) + 1;
    }
  }
}

void zeichneBitmap(const uint8_t *bitmap, uint8_t w, uint8_t h) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width() - w) / 2,
    ((display.height() - h) / 2),
    bitmap, w, h, 1);
  display.display();
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();
  delay(1);

  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
}

void testdrawcircle(void) {
  for (int16_t i=0; i<display.height(); i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
    display.display();
    delay(1);
  }
}
