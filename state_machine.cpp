#include <stdio.h>

#include "state_machine.h"
#include "ldpc.h"

input_t _h_symbols[8] = {0xFF, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
input_t _m_symbols[8] = {0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80};
input_t err[26] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

void StateMachine::handleStateWait(input_t input) {
  
  if (!input) return;
  
  for (int i = 0; i < 8; ++i) {
    if (input == _h_symbols[i]) {
      setState(STATE_HEADER_MATCHING);

      shift = i;

      if (shift == 0) {
        header_length = 1;
      } else {
        header_length = 2;
      }

      accepted_header = 0;

    }
  }

}

void StateMachine::handleStateHeaderMatching(input_t input) {
  
  if ((accepted_header < header_length) && (input == 0xFF)) {
  
    accepted_header += 1;
  
  } else if ((accepted_header == header_length) && (input == _m_symbols[shift])) {
  
    setState(STATE_GET_FIRST_DATA);
  
  } else {
  
    zero_byte = 0;
    setState(STATE_DATA_IGNORE);
  
  }

}

void StateMachine::handleStateGetFirstData(input_t input) {
  
  last_character = input;
  
  accepted_body = 0;
  setState(STATE_DATA_DECODE);

}

void StateMachine::handleStateDataDecode(input_t input) {

  input_t a = last_character;
  input_t b = input;

  input_t mask_a = _h_symbols[shift];
  input_t decoded = ((a & mask_a) << (8 - shift)) | (b >> shift);

  buffer[nextIndex] = decoded;
  nextIndex += 1;

  last_character = input;
  accepted_body += 1;
  
  if (accepted_body == 26) {
	  
    uint8_t check = LDPC_Check(buffer);

    #ifndef __AVR__
    packet_received += 1;
	
    LDPC_Decoder Decoder;
    Decoder.Input(buffer,err);

    if (check != 0) {

      for(uint8_t Iter=52; Iter; Iter--) {
        check=Decoder.ProcessChecks();
        if(check==0) break;
      }

    }
    #endif /* __AVR__ */

    if (check == 0) {

      #ifndef __AVR__
      packet_accepted += 1;
	  Decoder.Output(output);
	  #else
	  for (int i=25; i>=0; i--) { output[i] = buffer[i]; }
      #endif /* __AVR__ */

      setEvent(EVENT_RECEIVE_PACKET);

    } else {

      setEvent(EVENT_RECEIVE_BAD_PACKET);

    }
    
    state = STATE_WAIT;
    nextIndex = 0;

  }
}

void StateMachine::handleStateDataIgnore(input_t input) {

  if (input == 0x00) {
    zero_byte += 1;

    if (zero_byte > 4) {
            
      #ifndef __AVR__
      packet_ignored += 1;
      #endif /* __AVR__ */

      setEvent(EVENT_IGNORE_DATA);
      state = STATE_WAIT;
    }

  } else {
    
    zero_byte = 0;

  }

}