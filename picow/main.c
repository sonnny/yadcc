
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "thread/pt_cornell_rp2040_v1_3.h"
#include "ble/ble.h"
#include "pico/cyw43_arch.h"
#include "dcc.h"

char ble_data[80]; // this is declared extern in ble.h and copy new ble data to this variable

static uint8_t address = 3;
static uint8_t speed = 0;
static uint8_t direction = 0;

static uint8_t function_address = 3;
static uint8_t function = 0;

static bool new_function = true;

void process_string(char *str){
  char *token;
  char *buffer = str;
  char tokens[5][80];
  int x = 0;

  token = strtok(buffer, " ");
  while(token != NULL){
    strcpy(tokens[x], token);
    token = strtok(NULL, " ");
    x++;}

  // possible commands:
  //
  //   sp - speed
  //     address, speed, direction
  //
  //   fn - function
  //     function_address, function

  if (strcmp(tokens[0], "sp") == 0){ // is this command speed
    address =   (uint8_t) atoi(tokens[1]);
    speed =     (uint8_t) atoi(tokens[2]);
    direction = (uint8_t) atoi(tokens[3]);
    if (direction) speed += 0x80;}
    
  // function group one
  // 0b100D_DDDD
  // 0b100 F0____F4_F3_F2_F1
  else if (strcmp(tokens[0], "fn") == 0){ 
	new_function = true; 
    function_address = (uint8_t) atoi(tokens[1]);
    function = (uint8_t) atoi(tokens[2]);}
}


static PT_THREAD (ble_thread(struct pt *pt)){
  PT_BEGIN(pt);
  static uint16_t data;
  while(1){
 
    PT_SEM_SAFE_WAIT(pt, &BLUETOOTH_READY); // wait until a new data received from bluetooth
    ble_data[20] = '\0';
    //printf("%s\n",ble_data); // debugging
    process_string(ble_data);

  }
  PT_END(pt);}
  
static PT_THREAD (dcc_thread(struct pt *pt)){
  PT_BEGIN(pt);
  PT_INTERVAL_INIT();
  while(1){
    send_speed(address, speed);
    if (new_function) {
      send_function(function_address, function);
      new_function = false;}
    PT_YIELD_usec(10000);}  // adjust this to give decoder time between packets
  PT_END(pt);} // dcc_thread

void main(void){

stdio_init_all();
sleep_ms(500);
init_dcc();
multicore_launch_core1(bt_main);
pt_add_thread(ble_thread);
pt_add_thread(dcc_thread);
pt_sched_method = SCHED_ROUND_ROBIN;
pt_schedule_start;}

