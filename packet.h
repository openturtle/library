/*
 * packet.h
 *
 * Created: 6/28/2017 8:21:46 AM
 * Author: @ibrohimislam
 */ 


#ifndef PACKET_H_
#define PACKET_H_

typedef struct {
	uint8_t dummy[2];
	uint8_t header[4];
	uint8_t body[26];
} Packet;

typedef union {
	Packet data;
	uint8_t byte[32];
} PacketToSend;

#define packetInitializer() { .data = {{0x00, 0x00}, {0xFF, 0xFF, 0xFF, 0x00}}}
#define setPacketPointer(pointer, p) {pointer.dummy = p.data.dummy; pointer.header = p.data.header; pointer.body = p.data.body;}

typedef struct {
	uint8_t* dummy;
	uint8_t* header;
	uint8_t* body;
} PacketPointer;

#endif /* PACKET_H_ */