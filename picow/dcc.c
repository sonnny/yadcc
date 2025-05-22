#include "dcc.h"
#include "pulse.pio.h"

uint64_t packet;
uint8_t bytes[8];

void init_dcc(){
  uint offset = pio_add_program(pio0, &pulse_program);
 pulse_program_init(offset, 2);
  //pulse_program_init(offset, 3); //testing only, real is 2
}

void send_speed(uint8_t address, uint8_t speed){
  uint8_t checksum;

  checksum = address ^ SPEED_128 ^ speed;
  
  packet =  ((uint64_t)PREAMBLE<<48)
           |((uint64_t)address<<40)
           |((uint64_t)SPEED_128<<31)
           |((uint64_t)speed<<22)
           |((uint64_t)checksum<<13)
           |((uint64_t)0x01<<12);  
           
  for(int i=0; i<8; i++) bytes[i] = (packet >> (i * 8)) & 0xFF;
  pulse_wait_idle();
  for(int i=7; i>0; i--) pulse_put(bytes[i]);
}

void send_function(uint8_t address, uint8_t function){
  uint8_t checksum;
  checksum = address ^ function;
  packet =  ((uint64_t)PREAMBLE<<48)
           |((uint64_t)address<<40)
           |((uint64_t)function<<31)
	       |((uint64_t)checksum<<22)
	       |((uint64_t)0x01<<21);
  for(int i=0; i<8; i++) bytes[i] = (packet >> (i * 8)) & 0xFF;
  pulse_wait_idle();
  for(int i=7; i>1; i--) pulse_put(bytes[i]);
}
  
