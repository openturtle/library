/*
 * ST7540.h
 *
 * Created: 6/26/2017 4:23:58 PM
 *  Author: @ibrohimislam
 */ 


#ifndef ST7540_H_
#define ST7540_H_

extern "C" {
	#include "pca.h"
	#include "bus.h"
}

#include <stdint.h>
#include <avr/interrupt.h>

#define DDR_BU_THERM    DDRC
#define DDR_RXTX        DDRC
#define DDR_REG_DATA    DDRC
#define DDR_CD_PD       DDRC

#define PORT_BU_THERM   PORTC
#define PORT_RXTX       PORTC
#define PORT_REG_DATA   PORTC
#define PORT_CD_PD      PORTC

#define PIN_BU_THERM    PINC
#define PIN_CD_PD       PINC

#define BU_THERM        PC0
#define RXTX            PC1
#define REG_DATA        PC4
#define CD_PD           PC5

#define reg_acc()       PORT_REG_DATA   |=  (1<<REG_DATA);
#define data_acc()      PORT_REG_DATA   &=  ~(1<<REG_DATA);

#define tx_mode()       PORT_RXTX   &=  ~(1<<RXTX);
#define rx_mode()       PORT_RXTX   |=  (1<<RXTX);

// SPI
#define PORT_SPI        PORTB
#define PIN_SPI         PINB
#define DDR_SS          DDRB
#define PORT_SS         PORTB
#define ss_clear()      PORT_SS &=  ~(1<<SS_PIN);
#define ss_set()        PORT_SS |=  (1<<SS_PIN);

// LED
#define SS_PIN          PB1
#define DDR_LED         DDRD
#define PORT_LED        PORTD
#define LED1            PD6
#define LED2            PD7

extern uint8_t ST7540_REG[3];

// private

void ST7540_init();
void ST7540_setup(struct bus_t spi_bus);
void ST7540_get_register(struct bus_t spi_bus, uint8_t* reg);

// public

struct bus_t initialize();

#endif /* ST7540_H_ */