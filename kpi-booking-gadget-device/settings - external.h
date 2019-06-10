// WiFi Configuration
const char* ssid = "McOrts's iPhone";
const char* password = "ccitt6969";

// MQTT Cibfiguration
const char* mqtt_server = "domohome.ddns.jazztel.es";
const int mqtt_port = 11883;
const char* mqtt_id = "kpi-booking-gadget-device";
const char* mqtt_sub_topic_healthcheck = "/hbg/kpi";
const char* mqtt_sub_topic_panic = "/hbg/kpi/panic";
const char* mqtt_sub_topic_operation= "/hbg/kpi/operation";

// Other params
const int update_time = 5000;
