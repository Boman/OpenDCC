/*
 * main.c
 *
 *  Created on: 06.01.2012
 *      Author: falko
 */

#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t readSwitches() {
	uint8_t switches = ((~PINC) & 0x3F);
	switches |= ((~(PINB >> 7)) & 0x01) << 6;
	return switches;
}

volatile uint8_t blinkCount = 6;
uint16_t blinkInterval = 200;
#define SWITCH_REREAD 50

#define REREAD_SWITCHES 0
volatile uint8_t flags = 0;

uint16_t msClock = 0;
uint16_t msCount = 0;

ISR (TIMER0_OVF_vect) {
	/* Interrupt Aktion alle
	 (8000000/8)/256 Hz = 3906 Hz
	 bzw. 0.256 ms
	 */

	// true if a millisecond has passed
	if (msClock++ > F_CPU / 1000 / 8 / 256) {
		msClock = 0;

		// increment the milliSeconds
		msCount++;

		// reread the switches
		if (msCount % SWITCH_REREAD == 0) {
			flags |= 1 << REREAD_SWITCHES;
		}

		// let the LED blink
		if (msCount % blinkInterval == 0) {
//			if (PINC & (1 << PINC0)) {
//				blinkCount += 1;
//			}
			if (blinkCount > 0) {
				PORTB ^= 1 << DDB6;
				blinkCount--;
			}
		}
	}
}

uint8_t switches = 0;

int main() {
	/* Setze Eingang für Taster */
	DDRB = 0;
	DDRC = 0;

	/* Setze Ausgang für LED und UART */
	DDRB |= 1 << DDB6;
	DDRD |= (1 << DDD2) | (1 << DDD1) | (1 << DDD0);

	/* Setze PULLUPS an und LED aus */
	PORTB = (1 << DDB7);
	PORTC = 0xFF;

	// Timer 0 konfigurieren
	TCCR0B = (1 << CS01); // Prescaler 8

	// Overflow Interrupt erlauben
	TIMSK0 |= (1 << TOIE0);

	// Global Interrupts aktivieren
	sei();

	while (1) {
		if (flags & (1 << REREAD_SWITCHES)) {
			uint8_t newSwitches = readSwitches();
			if (newSwitches != switches) {
				switches = newSwitches;
				blinkCount += switches * 2;
			}
			flags &= ~(1 << REREAD_SWITCHES);
		}
	}
}
