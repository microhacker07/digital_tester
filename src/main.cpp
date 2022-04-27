#include "scheduler.hpp"

#include <SPI.h>
#include <Wire.h>

#define CLK_PIN           11

#define BUTTON_LED        LED3

#define OFFSET_INPUT_PIN  0
#define SPAN_INPUT_PIN    4

// number of times to do a full cycle in a second
#define CLK_FULL_FREQUENCY     20

// // The number of times to toggle the CLK pin in a second
// #define CLK_TOGGLE_FREQUENCY   (FULL_CLK_FREQUENCY /2)

// The time between each toggle
#define CLK_TOGGLE_PERIOD             (1'000'000L / CLK_FULL_FREQUENCY / 2)

void blink_step();
void button_step();
void clock_step();

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	pinMode(CLK_PIN, OUTPUT);
	digitalWrite(CLK_PIN, LOW);

	pinMode(USER_BTN, INPUT_PULLDOWN);

	pinMode(BUTTON_LED, OUTPUT);
	digitalWrite(BUTTON_LED, LOW);

	for (int i = 0; i < SPAN_INPUT_PIN; i++) {
		int pin = i + OFFSET_INPUT_PIN;
		pinMode(pin, OUTPUT);
		digitalWrite(pin, LOW);
	}

	Serial.begin(9'600);
	Serial.printf("\n\n\n");
	Serial.println("Digital Circuit Tester Running!");

	scheduler_add(TaskId::Clock, Task(clock_step, CLK_TOGGLE_PERIOD));
	scheduler_add(TaskId::Button, Task(button_step, 50'000L));
	scheduler_add(TaskId::Blink, Task(blink_step, 500'000L));
}

void loop()
{
	uint32_t wait_time = schedule();
	if (wait_time > 4) {
		delayMicroseconds(wait_time - 4);
	}
}

bool pressed_button()
{
	static int buttonPrev = LOW;
	int buttonCurrent = LOW;

	buttonCurrent = digitalRead(USER_BTN);
	digitalWrite(BUTTON_LED, buttonCurrent);

	if (buttonCurrent != buttonPrev) {
		buttonPrev = buttonCurrent;

		if (buttonPrev == LOW) {
			return true;
		}
	}

	return false;
}

void button_step()
{
	static int counter = 0;

	if (pressed_button()) {
		counter++;

		if (counter >= 1<<SPAN_INPUT_PIN) {
			counter = 0;
		}

		for (int i = 0; i < SPAN_INPUT_PIN; i++) {
			int pin = i + OFFSET_INPUT_PIN;
			int bit = (counter & 1<<i);
			digitalWrite(pin, bit ? HIGH : LOW);
		}
	}
}

void clock_step()
{
	static bool on = false;
	digitalWrite(CLK_PIN, on ? HIGH : LOW);
	on = !on;
}

void blink_step()
{
	static bool on = false;
	digitalWrite(LED_BUILTIN, on ? HIGH : LOW);
	on = !on;
}