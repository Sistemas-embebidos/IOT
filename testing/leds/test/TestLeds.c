#include "unity.h"
#include "leds.h"

static uint16_t ledsVirtuales;

void setUp(void) {
   Leds_Create(&ledsVirtuales);
}

void tearDown(void) {
}

void test_LedsOffAfterCreate(void) {
   uint16_t ledsVirtuales = 0xFFFF;
   Leds_Create(&ledsVirtuales);
   TEST_ASSERT_EQUAL_HEX16(0, ledsVirtuales);
}

void test_TurnOnLedOne(void) {
   Leds_TurnOn(1);
   TEST_ASSERT_EQUAL_HEX16(1, ledsVirtuales);
}

void test_TurnOffLedOne(void) {
   Leds_TurnOn(1);
   Leds_TurnOff(1);
   TEST_ASSERT_EQUAL_HEX16(0, ledsVirtuales);
}

void test_TurnOnMultipleLeds(void) {
   Leds_TurnOn(8);
   Leds_TurnOn(9);
   TEST_ASSERT_EQUAL_HEX16(0x0180, ledsVirtuales);
}

void test_TurnOffAnyLed(void) {
   Led_TurnAllOn();
   Leds_TurnOff(8);
   TEST_ASSERT_EQUAL_HEX16(0xFF7F, ledsVirtuales);
}

void test_TurnAllOn(void) {
   Led_TurnAllOn();
   TEST_ASSERT_EQUAL_HEX16(0xFFFF, ledsVirtuales);   
}

void test_LedMemoryIsNotReadable(void) {
   ledsVirtuales = 0xFFFF;
   Leds_TurnOn(8);
   TEST_ASSERT_EQUAL_HEX16(0x0080, ledsVirtuales);   
}