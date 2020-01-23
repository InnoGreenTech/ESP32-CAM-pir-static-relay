void add_module(){


      if ( server.hasArg("ip_server") ) 
              {      
              reception=server.arg("ip_server");
              int c = reception.length() + 1;                            
              if ( c>15){ Serial.println(F("adresse ip invalide")); return;}
              reception.toCharArray(ip_server, c); 
              for (int i = 0; i < c; i++) { 
                                            EEPROM.write((i+ADRESS_SERVER),reception[i]);  // save adresse IP in EEPROM
                                            }  
              }

      if ( server.hasArg("port") ) 
              {       
              reception = server.arg("port");
              port_server=reception.toInt();           
              int b=port_server;
              for (int a=0;a<2;a++){EEPROM.write(PORT_SERVER+a ,b);b=b>>8;} // Save configuration port
              } 

      if ( server.hasArg("name") ) 
              { 
              reception=server.arg("name"); 
              int c = reception.length() + 1;         // longueur de la chaîne de caractéres                        
              if ( c>20){Serial.println(F("Nom trop long"));return;} 
              reception.toCharArray(name_module, c); 
              for (int i = 0; i < c; i++) { 
                                            EEPROM.write((i+NAME_MODULE),reception[i]);  // save adresse IP in EEPROM
                                            }  
              } 

      if ( server.hasArg("period") )       // Perios to send datas
              {       
              reception = server.arg("period");
              period=reception.toInt();           
              int b=period;
              millisecondes=period*1000;
              for (int a=0;a<2;a++){EEPROM.write(PERIOD+a ,b);b=b>>8;} 
              } 

      enableServer=1;
      EEPROM.commit();     // Enregistrement

      server.send ( 200, "text/csv", answer_add_module() );   // envoi de la page  
}

/* file csv with information for php server */

String answer_add_module(){
  String page =name_module; page +=F(";");  page +=ref_module;  page +=F(";");  page +=local_mac;  page +=F(";");
         page +=local_ip; page +=F(";");  page +=version_module; page +=F("\r\n");                                      // line 1 informations module
         
         // type( probe or actuator,id,name,type_probe)

         /* DS18b20 Temperature*/
         
        // page +=F("probe;1;temp_1;DS18b20\r\n");    // line 2 informations probe 1

         page +=F("camera;c1;Camera;ESP32_CAM\r\n");    // Dereniére ligne informations probe 1

         

         
         return page;
  }
