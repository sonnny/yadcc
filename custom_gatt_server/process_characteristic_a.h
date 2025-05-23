
void process_characteristic_a(char * str){

    if(!strcmp(str,"OFF")) cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,0);
    else if(!strcmp(str,"ON")) cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN,1); } 
