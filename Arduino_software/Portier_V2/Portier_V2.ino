/************************************************************************************
    
    <ESP32-CAM contrl. This a camera with a PIR sensor. It can detect body and send a email>
    Copyright (C) <2019>  <Fabrice BAUDIN>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

    info@InnoGreenTech.fr

****************************************************************************************/


#include "OV2640.h"
#include <WiFi.h>
#include <WebServer.h>
#include <WiFiClient.h>
#include <TimeLib.h>
#include <NtpClientLib.h>
#include <ESP32_MailClient.h>
#include "SimStreamer.h"
#include "OV2640Streamer.h"
#include "CRtspSession.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems*





#include <EEPROM.h> // Gestion de la mémoire de type eeprom pour la sauvegarde des choix
#include <Update.h>



/* define memory adress */

#define ALARME_EEPROM   0   // Addresse de mémorisations des alarmes
#define NAME_MODULE     1   //Name of module
#define ADRESS_SERVER   22  // adress of server php
#define PORT_SERVER     43  // Port of server
#define PERIOD          50  // Period to send datas in seconds
#define MEM_SSID        70
#define MEM_PASSWORD    170


int           period;                // period to sent datas
char          name_module[20];       // name of module
String        ref_module = "camera";          // référence du module
String        version_module = "V1";      // version of module
char          ip_server[15];         // adress ip of the module
int unsigned  port_server;           // port of server
String        local_mac;             // adress mac of the module
String        local_ip;              // adress ip of module

byte movement = 0;
byte activation_alarmes = 0; // Mot utilisé pour l'activation des alarmes
byte alarmes_actives = 1;   // Drapeau d'une alarme active
byte enableServer = 1 ;     // Use to control if the parameters are good
String reception;             // Variable pour la réception sur le port série
int code;                     // Variable code pour l'échange d'information

int nbenvois = 0;         //nombre d'envoi de mail





unsigned long  lastmessage;         // heures d'envoi des derniers relevés
unsigned long  millisecondes;       // Intervalle des relevés en millisecondes
//int nb_cycle_lost_wifi=0;

String ssid = "";
String password = "";

/* Set web server */

WebServer server(80);

/* Set update server */

const char* update_path = "/firmware";
const char* update_username = "admin";
const char* update_password = "admin";


/* Set mail server */
#define MEM_USER_MAIL 200
#define MEM_PASSWORD_MAIL 260
#define MEM_SMTP_MAIL     320
#define MEM_PORT_MAIL     380
#define MEM_SENDER_MAIL   390
#define MEM_RECIPIENTS_MAIL 450
#define MEM_SUBJECT_MAIL  580
#define MEM_USER          640

SMTPData smtpData;

char user_mail[60] = "";
char user[60] = "";
char password_mail[60] = "";
char recipients_mail[230] = "";
char recipient_mail[60] = "";
char message_mail[500] = "";
char sender_mail[60] = "";
char smtp_mail[60] = "";
char port_mail[10] = "";
char subject_mail[60] = "";
int  portint;
int  freint;

void sendCallback(SendStatus info);

/*set cam*/

OV2640 cam;

/* set rtspServer */
WiFiServer rtspServer(8554);

/*Set PWM Light */
const int freq = 5000;
const int ledChannel = 0;
const int finess = 8;
const int light = 4;




void setup()
{
  
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, finess);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(light, ledChannel);

  pinMode(13, INPUT);
  pinMode(14, INPUT);
  pinMode(2, OUTPUT);
  //attachInterrupt(13, move_detect, RISING);




  EEPROM.begin(1024);                              // allocations  de 512 adresses Mémoires

  activation_alarmes = EEPROM.read(ALARME_EEPROM); // lecture du réglage des alarmes

  /* read connection configuration */

  char carac = '1'; // Name of module
  int a = 0;
  while (!(carac == '\0') && a < 20) {
    carac = char (EEPROM.read(NAME_MODULE + a));
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 20) {
    reception = F("InnoGreenTech");
  }
  int c = reception.length() + 1;                               // longueur de la chaîne de caractéres
  reception.toCharArray(name_module, c);

  reception = "";  // read memory ip adress server
  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 20) {
    carac = char (EEPROM.read(ADRESS_SERVER + a));
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 20) {
    reception = F("InnoGreenTech");
    enableServer = 0;
  }
  c = reception.length() + 1;
  reception.toCharArray(ip_server, c);

  byte mem[2];
  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(PORT_SERVER + a); // load the port server
  }
  port_server = mem[0] | mem[1] << 8;


  for (int a = 0; a < 3; a++) {
    mem[a] = EEPROM.read(PERIOD + a); // load the time period
  }
  period = mem[0] | mem[1] << 8;
  millisecondes = period * 1000;


  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 100) {
    carac = char (EEPROM.read(MEM_SSID + a));  //récupération du lien d'information
    if (!(carac == '\0')) {
      ssid += carac;
    } a++;
  }
  if (ssid == '\0' || a > 50) {
    ssid = F("");
  }

  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 100) {
    carac = char (EEPROM.read(MEM_PASSWORD + a));  //récupération du lien d'information
    if (!(carac == '\0')) {
      password += carac;
    } a++;
  }
  if (password == '\0' || a > 50) {
    password = F("");
  }

  /* read mail configuration */

  reception = "";
  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 60) {
    carac = char (EEPROM.read(MEM_SENDER_MAIL + a));
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 60) {
    reception = F("test@innogreentech.fr");
  }
  c = reception.length() + 1;
  reception.toCharArray(user_mail, c);

  reception = "";
  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 60) {
    carac = char (EEPROM.read(MEM_USER + a));
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 60) {
    reception = F("InnoGreenTech");
  }
  c = reception.length() + 1;
  reception.toCharArray(user, c);

  reception = "";
  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 60) {
    carac = char (EEPROM.read(MEM_PASSWORD_MAIL + a));
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 60) {
    reception = F("****");
  }
  c = reception.length() + 1;
  reception.toCharArray(password_mail, c);

  reception = "";
  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 60) {
    carac = char (EEPROM.read(MEM_SMTP_MAIL + a));
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 60) {
    reception = F("smtp.fr");
  }
  c = reception.length() + 1;
  reception.toCharArray(smtp_mail, c);

  reception = "";
  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 230) {
    carac = char (EEPROM.read(MEM_RECIPIENTS_MAIL + a));  //
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 230) {
    reception = F("test@innogreentech.fr");
  }
  c = reception.length() + 1;
  reception.toCharArray(recipients_mail, c);


  reception = "";
  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 60) {
    carac = char (EEPROM.read(MEM_SENDER_MAIL + a));  //
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 60) {
    reception = F("test@innogreentech.fr");
  }
  c = reception.length() + 1;
  reception.toCharArray(sender_mail, c);


  reception = "";
  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 60) {
    carac = char (EEPROM.read(MEM_SUBJECT_MAIL + a));
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 60) {
    reception = F("Présence détectée"); //
  }
  c = reception.length() + 1;
  reception.toCharArray(subject_mail, c);

  reception = "";
  carac = '1';
  a = 0;
  while (!(carac == '\0') && a < 4) {
    carac = char (EEPROM.read(MEM_PORT_MAIL + a));
    reception += carac;
    a++;
  }
  if (reception == '\0' || a == 4) {
    reception = F("587");
  }
  c = reception.length() + 1;
  reception.toCharArray(port_mail, c);
  portint = reception.toInt();


  Serial.begin(115200);
  while (!Serial)
  {
    ;
  }
  

    /* Start cam */
    
  cam.init(esp32cam_aithinker_config);
 
   /*:sensor_t * s = esp_camera_sensor_get();
    Serial.println(s->id.PID);
    
    //s->set_brightness(s, 1);//up the blightness just a bit
   // s->set_saturation(s, -1);//lower the saturation
    s->set_gain_ctrl(s,1);
    s->set_awb_gain(s,1);
    s->set_agc_gain(s,1);
    s->set_agc_gain(s,1);
    //s->set_vflip(s, 1);//flip it backµ*/



  /*Connexion wifi*/

  WiFi.mode(WIFI_STA);

  c = ssid.length() + 1;
  char char_ssid[50];
  ssid.toCharArray(char_ssid, c);

  c = password.length() + 1;
  char char_password[50];
  password.toCharArray(char_password, c);

  WiFi.begin(char_ssid, char_password);

  unsigned long time_out_connection = millis();
  while (WiFi.status() != WL_CONNECTED)
  {
    if (millis() - time_out_connection > 20000)
    {
      Serial.println F("");
      Serial.println F ("connection Wifi fail !");

      break;
    }
    delay(100);
    Serial.print('-');
  }

  if ( WiFi.status() != WL_CONNECTED) // Portail de connexion
  {
    Serial.println F ("Open setting gate !");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    delay(200);
    WiFi.softAP("InnoGreenTech", "innogreentech");
    server.on ( "/index.html", handleRoot );    // go to set up wifi page
    delay(100);
    server.on ("/style.css", cssRoot);
    local_mac = WiFi.macAddress();
    delay(100);
    server.begin();

  }

  else

  {

    Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );


    /*Configuration de la synchronisation de l'heure */

    NTP.begin("pool.ntp.org", 1, true);  //configuration de la récupération de la date,  Serveur+1 heure, Heure été/hivers
    NTP.setInterval(3600);                // Toute les heures

    delay(2000);


    /*informations du module*/

    local_ip = WiFi.localIP().toString().c_str();
    local_mac = WiFi.macAddress(); 
    ref_module = "camera";
    version_module = "V1";

    Serial.print(F("référence:")); Serial.println(ref_module);
    Serial.print(F("Version:")); Serial.println(version_module);
    Serial.print(F("Adresse IP:")); Serial.println(local_ip);
    Serial.print(F("Adresse MAC:")); Serial.println(local_mac);
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.dnsIP(1));

    /* Configuration du serveur Web */

    server.on ( "/index.html", indexRoot );    // Renvoi à la page de index
    delay(100);
    server.on ( "/", indexRoot );    // Renvoi à la page de index
    delay(100);
    server.on ( "/add_module.csv", add_module );    // Page to add or modify  module by php server
    delay(100);
    server.on ( "/set_mail.html", set_mail );    // Page to set mail configuration
    delay(100);
    server.on ("/style.css", cssRoot);     // envoi au fichier de configuration de style
    delay(100);

    /*return index page which is stored in serverIndex */
    server.on("/firmware", HTTP_GET, update_login);
    server.on("/serverIndex", HTTP_GET, update_index);

    /*handling uploading firmware file */

    server.on("/update", HTTP_POST, update_error, update_file);
    server.begin();
    rtspServer.begin();

  }
}
CStreamer *streamer;
CRtspSession *session;
WiFiClient client;

void loop()
{



  server.handleClient();

  /* Gest detection movement*/
  if (digitalRead(14) and (digitalRead(13) | digitalRead(2) ))
  {
    ledcWrite(ledChannel, 5);
    digitalWrite(2, 1);
  }
  else {
    ledcWrite(ledChannel, 0);
    digitalWrite(2, 0);
  }
  if (!digitalRead(14))
  {
    alarmes_actives = 0;
  }
  if (digitalRead(14) == 1 and alarmes_actives == 0 and activation_alarmes == 1)
  {
    delay(1000);
    Serial.println("Envoi d'une image");
    alarmes_actives = 1;
    send_mail();
  }


  uint32_t msecPerFrame = 200;
  static uint32_t lastimage = millis();

  // If we have an active client connection, just service that until gone
  // (FIXME - support multiple simultaneous clients)
  if (session) {
    session->handleRequests(0); // we don't use a timeout here,
    // instead we send only if we have new enough frames

    uint32_t now = millis();
    if (now > lastimage + msecPerFrame || now < lastimage) { // handle clock rollover
      session->broadcastCurrentFrame(now);
      lastimage = now;

      // check if we are overrunning our max frame rate
      now = millis();
      if (now > lastimage + msecPerFrame)
        printf("warning exceeding max frame rate of %d ms\n", now - lastimage);
    }

    if (session->m_stopped) {
      delete session;
      delete streamer;
      session = NULL;
      streamer = NULL;
    }
  }
  else {
    client = rtspServer.accept();

    if (client) {
      //streamer = new SimStreamer(&client, true);             // our streamer for UDP/TCP based RTP transport
      streamer = new OV2640Streamer(&client, cam);             // our streamer for UDP/TCP based RTP transport
      session = new CRtspSession(&client, streamer); // our threads RTSP session and state
    }
  } 

}
