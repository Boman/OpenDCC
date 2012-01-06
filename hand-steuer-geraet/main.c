/*
 * main.c
 *
 *  Created on: 06.01.2012
 *      Author: falko
 */

#include <avr/io.h>
#ifndef F_CPU
#warning "F_CPU war noch nicht definiert, wird nun mit 8MHz definiert"
#define F_CPU 8000000UL     /* Quarz mit 8 Mhz */
#endif
#include <util/delay.h>

#define LED_PORT	PORTD
#define LED_PIN		5

#define SWITCH_0_0_Port	PORTD
#define SWITCH_0_0_Pin	2
#define SWITCH_1_0_Port	PORTD
#define SWITCH_1_1_Pin	3
#define SWITCH_2_0_Port	PORTD
#define SWITCH_2_1_Pin	4

uint8_t switches = 0;

void readSwitches() {
	switches = PINC & 0x3F;
	switches |= ((PINB >> 7) & 0x01) << 6;
}

int main() {
	/* Setze Eingang für Taster */
	DDRB = 0;
	DDRC = 0;

	/* Setze Ausgang für LED an PortD */
	DDRD = 1 << LED_PIN;

	/* Setze LED an und die PULLUPS aus */
	PORTD = 1 << LED_PIN;

	while (1) {
		_delay_ms(50);
		PORTD ^= 1 << LED_PIN;
	}
}
