//
// use gedit with preferences / font & colors / cobalt blue based
//

//standard lib
#include <stdio.h>
#include <math.h>

//btstack
#include "btstack.h"

//high level lib
#include "pico/cyw43_arch.h"
#include "pico/btstack_cyw43.h"
#include "pico/stdlib.h"

#include "pico/multicore.h"
#include "pt_cornell_rp2040_v1_3.h"

//gap and gatt
#include "gap_advertise.h"
#include "my_service.h"
#include "process_characteristic_a.h"

static btstack_packet_callback_registration_t hci_event_callback_registration;
static char characteristic_a_tx[100];

static PT_THREAD (thread_ble(struct pt *pt)){
  PT_BEGIN(pt);
  
  while(1){
	  
	//we receive ble data do some work with characteristic_a_tx
    PT_SEM_SAFE_WAIT(pt, &BLUETOOTH_READY);
    
    process_characteristic_a(characteristic_a_tx);
     
  } PT_END(pt);}
  
int main(){
  stdio_init_all();
  if(cyw43_arch_init()){return -1;}
  l2cap_init();
  sm_init();
  
  //init att server no general read/write callbacks
  //because we'll set oneup for each service
  att_server_init(profile_data,NULL,NULL);
  
  //instantiate our custom service handler
  custom_service_server_init(characteristic_a_tx);
  
  //inform btstack state
  hci_event_callback_registration.callback = &packet_handler;
  hci_add_event_handler(&hci_event_callback_registration);
  
  //register att event
  att_server_register_packet_handler(packet_handler);
  
  //turn on bluetooth
  hci_power_control(HCI_POWER_ON);
  
  pt_add_thread(thread_ble);
  pt_sched_method = SCHED_ROUND_ROBIN;
  pt_schedule_start;}
  
  
  
