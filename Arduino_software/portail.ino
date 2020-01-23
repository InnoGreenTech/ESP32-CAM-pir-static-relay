
void handleRoot(){ 

      /* Pour réglage SSID */
      
      if ( server.hasArg("SSID") ) {       // si reception 
      ssid=server.arg("SSID");
      int c = ssid.length() + 1;         // longueur de la chaîne de caractéres
      for (int i = 0; i < c; i++) { 
                                  EEPROM.write((i+MEM_SSID),ssid[i]);  // Réglage du pointeur sur l'adresse mémoire
                                  }                       
      } 

      /*if ( server.hasArg("set_temp") ) {       // si reception 
      ssid=server.arg("seuil");
      int c = ssid.length() + 1;         // longueur de la chaîne de caractéres
      for (int i = 0; i < c; i++) { 
                                  EEPROM.write((i+MEM_SET_TEMP),ssid[i]);  // Réglage du pointeur sur l'adresse mémoire
                                  }                    
      } */
      
      if ( server.hasArg("PASSWORD") ) {       // si reception 
      password=server.arg("PASSWORD");
      int c = password.length() + 1;         // longueur de la chaîne de caractéres
      for (int i = 0; i < c; i++) { 
                                  EEPROM.write((i+MEM_PASSWORD),password[i]);  // Réglage du pointeur sur l'adresse mémoire
                                  }
             EEPROM.commit();     // Enregistrement dnas la mémoire flash
             delay (500);
             ESP.restart();
                                                               
      } 
      
               
       server.send ( 200, "text/html", page_handleroot() );   // envoi de la page
    }
  


String page_handleroot(){
  String page =F("<!DOCTYPE html> <html lang=fr-FR> <head> <meta charset='utf-8'><link rel='stylesheet'type='text/css'href='style.css'><title>Afficheur</title></head>"); 
         page +=F("<body>");
         
        

         page +=F("<section id='configuration'>");
         page +=F("<h2> Configuration de la connexion wifi</h2>"); 
         page +=F("<form method='get' accept='' >");

         page +=F("<p>adresse MAC: ");
         page +=local_mac;
         page +=F("<p/>");
         
         page +=F("<br/>");
         page +=F("<br/>");
                  
         page += F("<label>SSID: </label><br/>");
         page += F("<input type='texte' name='SSID' id='SSID' maxlength='45' value='");page +=ssid; page +=F("' />");
         
         page +=F("<br/>");
         page +=F("<br/>");
                  
         page += F("<label>Password: <br/></label>");
         page += F("<input type='password' name='PASSWORD' id='PASSWORD' maxlength='45' />");
         
         page +=F("<br/>");
         page +=F("<br/>"); 
                 
                
         page += F("<input type='submit'  value='Envoi'>");
         page += F("</p>");
         page += F("</form>");
         page += F("</section>");
     
         
         page +=F("</div>");
         page +=F("<footer><a href='http://www.innogreentech.fr'>InnoGreenTech</a><br/><a href='mailto: info@innogreentech.fr'>Contactez moi</a></footer>");
         page +=F("</div></body></html>");

return page;
}
