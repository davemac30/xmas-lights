#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

//#define DEBUG

#define LED_PIN 6
#define LED_COUNT 100

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_RGB + NEO_KHZ800);

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

class Programme
{
protected:
    const char *name;

public:
    Programme(char *name) {
        this->name = name;
    };
    virtual void display(uint32_t) = 0;

    char *get_name() {
      return name;
    }
};

class Off: public Programme
{
public:
    Off(char *name) : Programme(name) {}

    void display(uint32_t count) {
        strip.clear();
    }
};

class Runner: public Programme
{
public:
    Runner(char *name) : Programme(name) {}

    void display(uint32_t count) {
      strip.setPixelColor(count % LED_COUNT, 0, 0, 0);
      strip.setPixelColor((count + 1) % LED_COUNT, 32, 32, 32);
    }
};

class RandomTwinkle: public Programme
{
public:
    RandomTwinkle(char *name) : Programme(name) {}

    void display(uint32_t count) {
        if (count % 10 == 0) {
            for (int i = 0; i < LED_COUNT; i++) {
                strip.setPixelColor(i, random(32), random(32), random(32));
            }
        }
    }
};

class HalloweenTwinkle : public Programme
{
public:
    HalloweenTwinkle(char *name) : Programme(name) {}

    void display(uint32_t count) {
        if (count % 10 == 0) {
            for (int i = 0; i < LED_COUNT; i++) {
                strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(65536 / 12, 255, random(32) + 64)));
            }
        }
    }
};

class XmasTwinkle : public Programme
{
public:
    XmasTwinkle(char *name) : Programme(name) {}

    void display(uint32_t count) {
        if (count % 10 == 0) {
            for (int i = 0; i < LED_COUNT; i++) {
                strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(65536 * 2 / 3, 255, random(32) + 64)));
            }
        }
    }
};

RandomTwinkle random_twinkle("random twinkle");
Off off("all lights off");
Runner runner("running light");
HalloweenTwinkle halloween_twinkle("halloween twinkle");
XmasTwinkle xmas_twinkle("xmas twinkle");

//Programme *prog_list[] = {&random_twinkle, &off, &runner, &off, &halloween_twinkle};
Programme *prog_list[] = {&xmas_twinkle};

int num_progs = sizeof(prog_list) / sizeof(*prog_list);

void setup() {
#ifdef DEBUG
    Serial.begin(9600);
#endif

    strip.begin();

    lcd.begin(16, 2);
}

char buf[32];

uint32_t count = 0;
int prog = 0;

void loop() {

  if (count % 1000 == 0) {
    prog = (prog + 1) % num_progs;
    lcd.clear();
    sprintf(buf, "%s", prog_list[prog]->get_name());
    lcd.print(buf);
#ifdef DEBUG
    Serial.println(buf);
#endif
  }

  prog_list[prog]->display(count);

  strip.show();
  count++;
  delay(10);
}
