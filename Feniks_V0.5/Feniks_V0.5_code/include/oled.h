bool oled_setup();
void update_oled(double meat_temp, double ambient_temp, int battery, int RSSI);
void display_connected_network(String SSID);
void display_connecting_wifi();
void display_wifi_setup(String SSID);