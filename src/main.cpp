#include <Arduino.h>
#include <SPI.h>

typedef uint8_t SPI_DEVICE_INSTANCE_t;
enum
{
    SPIDEV_NONE = 0b00000000,     // Bit mask for no radio
    SPIDEV_1    = 0b00000001,     // Bit mask second device
    SPIDEV_2    = 0b00000010,     // Bit mask first device
    SPIDEV_3    = 0b00000100,     // Bit mask first device
    SPIDEV_ALL  = 0b00000111      // bit mask for all devices
};

#define GPIO_PIN_SPI_SCK  27
#define GPIO_PIN_SPI_MISO 33
#define GPIO_PIN_SPI_MOSI 32
#define GPIO_PIN_SPI_NSS  25

#ifndef RELEASE
//#  error "Build in release mode!"
#endif

//#define SOFTWARE_CS

void setup() {
  pinMode(22, OUTPUT);
  digitalWrite(22, HIGH);

  pinMode(GPIO_PIN_SPI_NSS, OUTPUT);
  digitalWrite(GPIO_PIN_SPI_NSS, HIGH);

  SPI.begin(GPIO_PIN_SPI_SCK, GPIO_PIN_SPI_MISO, GPIO_PIN_SPI_MOSI, GPIO_PIN_SPI_NSS);
  SPI.setFrequency(10000000);
#if !defined(SOFTWARE_CS)
  SPI.setHwCs(true);
  spiEnableSSPins(SPI.bus(), SPIDEV_ALL);
#endif
}

void spiNssSet(uint8_t mask, uint8_t value)
{
#if !defined(SOFTWARE_CS)
    spiDisableSSPins(SPI.bus(), ~mask);
    spiEnableSSPins(SPI.bus(), mask);
#else
  if (mask == SPIDEV_1) {
      digitalWrite(GPIO_PIN_SPI_NSS, value);
  }
#endif
}

void sr_write(uint8_t value)
{
    spiNssSet(SPIDEV_1, HIGH);
    SPI.transfer(&value, (uint8_t)sizeof(value));
    spiNssSet(SPIDEV_1, LOW);
}

void loop() {

    uint8_t sr_value = 0x00;

    while (1) {
      digitalWrite(22, HIGH);
      sr_write(sr_value);
      sr_value += 1;
      digitalWrite(22, LOW);
      //delay(250);
    }
}
