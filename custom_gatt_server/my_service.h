

#include "btstack_defines.h"
#include "ble/att_db.h"
#include "ble/att_server.h"
#include "btstack_util.h"
#include "bluetooth_gatt.h"
#include "btstack_debug.h"

#include "service.h"
//#include "pt_cornell_rp2040_v1_3.h"

typedef struct{
	
  hci_con_handle_t con_handle;
  
  // characteristic a info
  char * characteristic_a_value;
  uint16_t characteristic_a_client_configuration;
  char * characteristic_a_user_description;
  
  // characteristic a handles
  uint16_t characteristic_a_handle;
  uint16_t characteristic_a_client_configuration_handle;
  uint16_t characteristic_a_user_description_handle;
  
  // callback
  btstack_context_callback_registration_t callback_a;
  
} custom_service_t;

// call back reg object and att service handler
static att_service_handler_t service_handler;
static custom_service_t service_object;

// char description
char characteristic_a[] = "led control";

// protothreads semaphone
struct pt_sem BLUETOOTH_READY;

// call back for att notifications on characteristic
static void characteristic_a_callback(void * context){
  //associate void pointer input with custom service object
  custom_service_t * instance = (custom_service_t *) context;
  //send notification
  att_server_notify(instance->con_handle, 
    instance->characteristic_a_handle, 
    instance->characteristic_a_value, 
    strlen(instance->characteristic_a_value));};
 
//read callback
static uint16_t custom_service_read_callback(hci_con_handle_t con_handle,
  uint16_t attribute_handle,
  uint16_t offset,
  uint8_t * buffer,
  uint16_t buffer_size){
	  
  UNUSED(con_handle);
  
  // characteristic a
  if(attribute_handle == service_object.characteristic_a_handle){
  return att_read_callback_handle_blob(service_object.characteristic_a_value,
    strlen(service_object.characteristic_a_value),
    offset,
    buffer,
    buffer_size);}

	if (attribute_handle == service_object.characteristic_a_client_configuration_handle){
	return att_read_callback_handle_little_endian_16(service_object.characteristic_a_client_configuration,
	  offset, 
	  buffer, 
	  buffer_size);}
	
	if (attribute_handle == service_object.characteristic_a_user_description_handle) {
    return att_read_callback_handle_blob(service_object.characteristic_a_user_description, 
      strlen(service_object.characteristic_a_user_description),
      offset, 
      buffer,
      buffer_size);}
      
  return 0;}

//write callback
static int custom_service_write_callback(hci_con_handle_t con_handle,
  uint16_t attribute_handle, 
  uint16_t transaction_mode,
  uint16_t offset, uint8_t *buffer,
  uint16_t buffer_size){
	  
  UNUSED(transaction_mode);
  UNUSED(offset);
  UNUSED(buffer_size);

  // Write characteristic value
  if (attribute_handle == service_object.characteristic_a_handle){
    custom_service_t * instance = &service_object;
	buffer[buffer_size] = 0;
	memset(service_object.characteristic_a_value, 0, sizeof(service_object.characteristic_a_value)) ;
	memcpy(service_object.characteristic_a_value, buffer, buffer_size);
	//null terminate string
	service_object.characteristic_a_value[buffer_size] = 0;
	// Alert the application of a bluetooth RX
	PT_SEM_SAFE_SIGNAL(pt, &BLUETOOTH_READY);}
	  
  return 0;}
  
///////////////user api
//init custom service handler
void custom_service_server_init(char * a_ptr){

  //init semaphore
  PT_SEM_SAFE_INIT(&BLUETOOTH_READY, 0);
  
  //pointer to our service object
  custom_service_t * instance = &service_object;
  
  //assign char value
  instance->characteristic_a_value = a_ptr;
  
  //assign char user description
  instance->characteristic_a_user_description = characteristic_a;
  
  //assign handle values from generated gatt header
  instance->characteristic_a_handle = ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_VALUE_HANDLE;
  instance->characteristic_a_client_configuration_handle = ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_CLIENT_CONFIGURATION_HANDLE;
  instance->characteristic_a_user_description_handle = ATT_CHARACTERISTIC_0000FF11_0000_1000_8000_00805F9B34FB_01_USER_DESCRIPTION_HANDLE;

  //service start and end handles
  service_handler.start_handle = 0;
  service_handler.end_handle = 0xffff;
  service_handler.read_callback = &custom_service_read_callback;
  service_handler.write_callback = &custom_service_write_callback;
  
  //register service handler
  att_server_register_service_handler(&service_handler);
}
  

