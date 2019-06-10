// WiFi Configuration
const char* ssid = "MiFibra-2D79";
const char* password = "oqnQYSp3";

// MQTT Cibfiguration
const char* mqtt_server = "192.168.1.114";
const int mqtt_port = 1883;
const char* mqtt_id = "kpi-booking-gadget-device";
const char* mqtt_sub_topic_healthcheck = "/hbg/kpi";
const char* mqtt_sub_topic_panic = "/hbg/kpi/panic";
const char* mqtt_sub_topic_operation= "/hbg/kpi/operation";

// Other params
const int update_time = 5000;
