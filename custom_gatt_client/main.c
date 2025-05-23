//
// use gedit with preferences / font & colors / cobalt blue based
//
//   we are only searching for address prefix of 28:CD:C1 (pico w)
//   we are only searching service               0xFF10
//   we are only searching characteristic        0xFF11
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btstack.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "pt_cornell_rp2040_v1_3_client.h"

#define SERVICE_TO_SEARCH        0xFF10
#define CHARACTERISTIC_TO_SEARCH 0xFF11
#define ADDRESS_TO_SEARCH        "28:CD:C1"

// Bluetooth control structures
static btstack_packet_callback_registration_t hci_event_callback_registration;
static bd_addr_t server_addr;
static bd_addr_type_t server_addr_type;
static hci_con_handle_t connection_handle;
static gatt_client_service_t server_service;

static gatt_client_notification_t notification_listener;

// Protothreads semaphore
struct pt_sem characteristics_discovered ;

typedef struct advertising_report {
    uint8_t   type;
    uint8_t   event_type;
    uint8_t   address_type;
    bd_addr_t address;
    uint8_t   rssi;
    uint8_t   length;
    const uint8_t * data;
} advertising_report_t;

static bd_addr_t cmdline_addr;
static int cmdline_addr_found = 0;

static hci_con_handle_t connection_handle;
static gatt_client_service_t my_service;
static gatt_client_characteristic_t characteristic;
static bool service_found = false;
static bool show_characteristic_once = true;

static btstack_packet_callback_registration_t hci_event_callback_registration;

static void fill_advertising_report_from_packet(advertising_report_t * report, uint8_t *packet){
    gap_event_advertising_report_get_address(packet, report->address);
    report->event_type = gap_event_advertising_report_get_advertising_event_type(packet);
    report->address_type = gap_event_advertising_report_get_address_type(packet);
    report->rssi = gap_event_advertising_report_get_rssi(packet);
    report->length = gap_event_advertising_report_get_data_length(packet);
    report->data = gap_event_advertising_report_get_data(packet);}

static void handle_gatt_client_event(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(packet_type);
    UNUSED(channel);
    UNUSED(size);

    gatt_client_service_t service;
    switch(hci_event_packet_get_type(packet)){
        case GATT_EVENT_SERVICE_QUERY_RESULT:
            gatt_event_service_query_result_get_service(packet, &service);
            if (service.uuid16 == SERVICE_TO_SEARCH) {
              my_service = service;
              service_found = true;}
  
            break;
        case GATT_EVENT_CHARACTERISTIC_QUERY_RESULT:
          if (show_characteristic_once) {
            gatt_event_characteristic_query_result_get_characteristic(packet, &characteristic);
           if (characteristic.uuid16 == CHARACTERISTIC_TO_SEARCH){
              //printf("characteristic ready for writing...\n");
              cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);           
            }
            show_characteristic_once = false;}
            break;
        case GATT_EVENT_QUERY_COMPLETE:
               if (service_found) gatt_client_discover_characteristics_for_service(handle_gatt_client_event, connection_handle, &my_service);        
            break;
        default:
            break;
    }}

static void handle_hci_event(uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size){
    UNUSED(channel);
    UNUSED(size);

    if (packet_type != HCI_EVENT_PACKET) return;
    advertising_report_t report;
    
    uint8_t event = hci_event_packet_get_type(packet);
    switch (event) {
        case BTSTACK_EVENT_STATE:
            // BTstack activated, get started
            if (btstack_event_state_get_state(packet) != HCI_STATE_WORKING) break;
            printf("BTstack activated, start scanning!\n");
            gap_set_scan_parameters(0,0x0030, 0x0030);
            gap_start_scan();
            break;
        case GAP_EVENT_ADVERTISING_REPORT:
            fill_advertising_report_from_packet(&report, packet);
            if (strncmp(bd_addr_to_str(report.address), ADDRESS_TO_SEARCH, 8) == 0) {
              //printf("********************found device\n");
              gap_stop_scan();
              gap_connect(report.address,report.address_type);}
            break;
        case HCI_EVENT_META_GAP:
            if (hci_event_gap_meta_get_subevent_code(packet) !=  GAP_SUBEVENT_LE_CONNECTION_COMPLETE) break;
            //printf("\nGATT browser - CONNECTED\n");
            connection_handle = gap_subevent_le_connection_complete_get_connection_handle(packet);
            gatt_client_discover_primary_services(handle_gatt_client_event, connection_handle);
            break;
        case HCI_EVENT_DISCONNECTION_COMPLETE:
            //printf("\nGATT browser - DISCONNECTED\n");
            break;
        default:
            break;
    }}

static void gatt_client_setup(void){
  l2cap_init();
  gatt_client_init();
  sm_init();
  sm_set_io_capabilities(IO_CAPABILITY_NO_INPUT_NO_OUTPUT);
  hci_event_callback_registration.callback = &handle_hci_event;
  hci_add_event_handler(&hci_event_callback_registration);}

static PT_THREAD (blink_thread(struct pt *pt)){
  PT_BEGIN(pt);
  static bool led_state = false;
  gpio_init(15);
  gpio_set_dir(15, GPIO_OUT);
  PT_INTERVAL_INIT();
  while(1){
    led_state = !led_state;
    gpio_put(15, led_state);
    PT_YIELD_INTERVAL(1000000);} PT_END(pt);}

static PT_THREAD (button_thread(struct pt *pt)){
  PT_BEGIN(pt);
  static int status;
  static char on[] = "ON";
  static char off[] = "OFF";
  gpio_init(4);
  gpio_set_dir(4,GPIO_IN);
  gpio_pull_up(4);
  PT_INTERVAL_INIT();
  while(1){
    if (!gpio_get(4)) gatt_client_write_value_of_characteristic_without_response(connection_handle,
      characteristic.value_handle, strlen(on), on);
    else gatt_client_write_value_of_characteristic_without_response(
      connection_handle,
      characteristic.value_handle,
      strlen(off),
      off);
    PT_YIELD_INTERVAL(100000);
  } PT_END(pt);}
     
int main() {
  stdio_init_all();
  //printf("starting gatt client...\n");
  if (cyw43_arch_init()) {return -1; }
  cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
  gatt_client_setup();
  att_server_init(NULL, NULL, NULL);
  hci_power_control(HCI_POWER_ON);
  pt_add_thread(blink_thread);
  pt_add_thread(button_thread);
  pt_sched_method = SCHED_ROUND_ROBIN;
  pt_schedule_start;}

