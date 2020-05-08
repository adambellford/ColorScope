/*
  DIY фазово-контрастный осветитель (подсветка) для микроскопа
  
  Скетч позволяет управлять цветом светодиодной ленты с трех потенциометров.
  Код работает на Arduino UNO с лентой WS2812, но легко меняется, чтобы
  работать с другими комбинациями железа.
  Используется библиотека NeoPixel от Adafruit.
  
  Автор: Global Hell 2015-2-12
 */
#include <Adafruit_NeoPixel.h>


// Аналоговые входы для переменных резисторов (потенциометров)
const uint8_t PM_RED = A0;
const uint8_t PM_GREEN = A1;
const uint8_t PM_BLUE = A2;
// Параметры для RGB-ленты
const uint8_t LED_PIN = 3;     // выходной пин, управляющий лентой (должен поддерживать ШИМ)
const uint8_t LED_QUAN = 16;  // сколько светодиодов в ленте?
const uint8_t BRIGHTNESS = 255; // яркость, 255 - максимальная
// Интевал опроса потенциометров
const uint16_t INTERVAL = 5;  // миллисекунд


// Переменная хранения цвета
uint32_t current_color;
// Переменная таймера опроса
uint32_t last_time;

// Обьект NeoPixel для управления лентой
Adafruit_NeoPixel led_strip = Adafruit_NeoPixel(LED_QUAN, LED_PIN, NEO_GRB + NEO_KHZ800);  // WS2812

void setup() {
    // Инициализируем пины
    pinMode(PM_RED, INPUT);
    pinMode(PM_GREEN, INPUT);
    pinMode(PM_BLUE, INPUT);

    // Установка пина управления лентой в качестве выхода
    led_strip.begin();
    // Яркость
    led_strip.setBrightness(BRIGHTNESS);
    // "Пустой" цвет, для начала
    current_color = led_strip.Color(0, 0, 0);

    last_time = millis();
}


void loop() {
  uint32_t current_time = millis();
  if ((current_time - last_time) >= INTERVAL) {  // если прошел интервал
    last_time = current_time;
    // Считываем величины с потенциометров
    uint8_t* rgb = pt2RGB();
    // Собираем из них цвет
    current_color = led_strip.Color(rgb[0], rgb[1], rgb[2]);
  }

  led_strip.fill(current_color);  // заполняем ленту цветом
  led_strip.show();               // отображаем
}


uint8_t* pt2RGB() {
  // Считываем и преобразуем данные с потенциометров
  // Заносим их в массив, потом его возвращаем
  static uint8_t vals[3];
  vals[0] = (uint8_t) map(analogRead(PM_RED), 0, 1023, 0, 255);
  vals[1] = (uint8_t) map(analogRead(PM_GREEN), 0, 1023, 0, 255);
  vals[2] = (uint8_t) map(analogRead(PM_BLUE), 0, 1023, 0, 255);

  return vals;
}
