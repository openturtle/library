/*
 * state_machine.h
 *
 * Created: 6/28/2017 8:21:46 AM
 * Author: @ibrohimislam
 */ 

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

#include <stdint.h>

typedef uint8_t input_t;
typedef uint8_t output_t;

typedef enum _e_state {
  STATE_WAIT = 0x00,
  STATE_HEADER_MATCHING = 0x01,
  STATE_GET_FIRST_DATA = 0x02,
  STATE_DATA_DECODE = 0x03,
  STATE_DATA_IGNORE = 0x04
} state_t;

typedef enum _e_event {
  EVENT_NOTHING = 0x00,
  EVENT_RECEIVE_BAD_PACKET = 0x01,
  EVENT_RECEIVE_PACKET = 0x02,
  EVENT_IGNORE_DATA = 0x03
} event_t;

class StateMachine {

private:
  state_t state = STATE_WAIT;
  event_t event = EVENT_NOTHING;

  // state memory [start] -----------------------------
  int shift;
  int accepted_header;
  int header_length;

  input_t last_character;
  input_t buffer[26];

  uint8_t accepted_body;
  uint8_t zero_byte;
  uint8_t nextIndex = 0;
  // state memory [end] -------------------------------
  
  void handleStateWait(input_t input);
  void handleStateHeaderMatching(input_t input);
  void handleStateGetFirstData(input_t input);
  void handleStateDataDecode(input_t input);
  void handleStateDataIgnore(input_t input);

  void inline setState(state_t s) { state = s; };
  void inline setEvent(event_t e) { event = e; };

public:

  output_t output[26];
  event_t inline getEvent() { return event; }

#ifndef __AVR__
  uint32_t packet_ignored = 0;
  uint32_t packet_received = 0;
  uint32_t packet_accepted = 0;
#endif /* __AVR__ */

  void handleInput(input_t input) {
    switch(state) {
      case STATE_WAIT:
        event = EVENT_NOTHING;
        handleStateWait(input);
        break;
      case STATE_HEADER_MATCHING:
        event = EVENT_NOTHING;
        handleStateHeaderMatching(input);
        break;
      case STATE_GET_FIRST_DATA:
        event = EVENT_NOTHING;
        handleStateGetFirstData(input);
        break;
      case STATE_DATA_DECODE:
        event = EVENT_NOTHING;
        handleStateDataDecode(input);
        break;
      case STATE_DATA_IGNORE:
        event = EVENT_NOTHING;
        handleStateDataIgnore(input);
        break; 
    }
  }

};

#endif /* STATE_MACHINE_H_ */