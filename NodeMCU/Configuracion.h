#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <String.h>
/*################## Informacion ################*/
static const char *VERSION = "1.3.1";
/*################## Trenes #####################*/
#define L0 "Invalido"
#define L1 "Roca"
#define L2 "San_Martin"
#define L3 "Sarmiento"
#define L4 "Mitre"
#define L5 "Belgrano_Sur"

typedef enum{LINEA0,LINEA1,LINEA2,LINEA3,LINEA4,LINEA5}t_lines;

static const char* lines[]={L0,L1,L2,L3,L4,L5};
/*################## Configuracion ################*/
ADC_MODE(ADC_VCC);                            // Pin a sensar (Conectar A0 con 3V para medir 3.3V)
#define SERIAL_SET_DEBUG_OUTPUT  false        // Activar modo debug
static const uint16_t MAX_DEVICES = 250;             // Maxima cantidad de dispositivos a almacenar
#define FREQ 1*1000                           
//#define CHOP 25                                // Cantidad de elementos en la trama de datos
#define LINE LINEA1                           // Linea de Trenes Argentinos (ver Trenes)
#define TRAIN 1                               // Numero de formacion
#define CAR 5                                 // Numero de coche (1a7 o 1a9 segun linea)
#define TXPOWER -20.5
static uint16_t N_devices;
uint16_t people;
uint16_t list_timeout_var = 150;
uint16_t reported_var = 3;
uint16_t sendtime_var = 30;
static uint8_t chop_var = 25;
int8_t min_rssi_var = -99;
/* ####################### Interrupciones ######################## */
#define timer0_preload 40161290/500
#define my_delay 100
/*################## WiFi ################*/
static const char *ESP_NAME = "GICSAFe1"; // Host name
static const char *AP_SSID = "GICSAFe1";                // Nombre del AP
static const char *AP_PASSWORD = "GICSAFeMOS";         // Contraseña del AP

/*################## MQTT ################*/
//static const char* MQTT_HOST = "190.3.92.172";                               // Direccion del broker
//static const char* MQTT_HOST = "190.3.92.172";                               // Direccion del broker
static const char* MQTT_HOST = "192.168.0.22";                               // Direccion del broker
//const char* MQTT_HOST = "linsse.com.ar";                             
#define MQTT_PORT  1883                                                  // Puerto del broker   
//static String Address = "/cdp/"+String(lines[LINE])+"/F_"+TRAIN+"/C_"+CAR+"/";
static String Address = "martin/macs/";
//const char* MQTT_OUT_TOPIC = Address.c_str();                            // Topico del broker                 
static String MQTT_OUT_TOPIC = Address;                               // Topico del broker
static String MQTT_CONFIG_TOPIC1 = "/cdp/configout/timeout/";
static String MQTT_CONFIG_TOPIC2 = "/cdp/configout/reported/";
static String MQTT_CONFIG_TOPIC3 = "/cdp/configout/sendtime/";
static String MQTT_CONFIG_TOPIC4 = "/cdp/configout/chop";
static String MQTT_CONFIG_TOPIC5 = "/cdp/configout/rssi";
/*################## Timer ################*/
#define day  86400000 // 86400000 milisegundos por dia 
#define hour  3600000 // 3600000 milisegundos en una hora
#define minute  60000 // 60000 millisegundos en un minuto
#define second   1000 // 1000 millisegundos en un segundo

#endif //CONFIGURACION_H
