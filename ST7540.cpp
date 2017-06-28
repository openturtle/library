/*
 * ST7540.cpp
 *
 * Created: 6/26/2017 4:23:44 PM
 * Author: @ibrohimislam
 */ 
#include "ST7540.h"

ISR(PCINT1_vect){
	if ((PIN_CD_PD & (1<<CD_PD))==0) {
		ss_clear();
		}else{
		ss_set();
	}
}

struct bus_t initialize() {
	
	struct bus_t spi_bus;
		
	// set LED as output
	DDR_LED |=  (1<<LED1) | (1<<LED2);
		
	// set SPI_SS as output
	DDR_SS  |=  (1<<SS_PIN);
		
	spi_bus = spi_hw_poll_bus_get();
	spi_hw_poll_init(E_SPI_MODE_SLAVE, E_SPI_SPEED_F2);
	spi_hw_poll_clock_mode_set(E_CLOCK_MODE0);
	
	// ST7540 TX setup
	ST7540_init();
	ST7540_setup(spi_bus);
	
	PORT_LED |=  (1<<LED1);

	data_acc();
	rx_mode();
	
	return spi_bus;
	
}

void ST7540_init(){
	//Configure output
	DDR_RXTX        |=  (1<<RXTX);
	DDR_REG_DATA    |=  (1<<REG_DATA);
	
	//Configure input
	DDR_CD_PD       &=  ~(1<<CD_PD);
	DDR_BU_THERM    &=  ~(1<<BU_THERM);
	
	ss_set();
}

void ST7540_setup(struct bus_t spi_bus){
	reg_acc();
	tx_mode();

	ss_clear();
	spi_bus.f_send(spi_bus.priv, ST7540_REG, sizeof(ST7540_REG)/sizeof(*ST7540_REG), 0);
	ss_set();
}

void ST7540_get_register(struct bus_t spi_bus, uint8_t* reg){
	reg_acc();
	rx_mode();
	
	ss_clear();
	spi_bus.f_recv(spi_bus.priv, reg, 4, 0);
	ss_set();
}