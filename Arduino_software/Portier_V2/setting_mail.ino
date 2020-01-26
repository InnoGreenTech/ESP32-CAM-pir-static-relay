
void set_mail() {

  /* active alarm mail */
  
  if ( server.hasArg("MAIL") ) {       
      reception=server.arg("MAIL");
      if (reception.equals("1")) {activation_alarmes=1;}
      else {activation_alarmes=0;}                 
      EEPROM.write(ALARME_EEPROM, activation_alarmes);
  }

  
  /* mail sender setting */
  
  if ( server.hasArg("MAILEXP") ) {       
    reception = server.arg("MAILEXP");
    int c = reception.length() + 1; 
    reception.toCharArray(sender_mail, c);        
    for (int i = 0; i < c; i++) { 
                                   EEPROM.write((i+MEM_SENDER_MAIL),sender_mail[i]);
                                  }
  }


  /* Password mail setting */
  
  if ( server.hasArg("PW") ) {              
     reception= server.arg("PW");
     int c = reception.length() + 1;         
     reception.toCharArray(password_mail, c);
     for (int i = 0; i < c; i++) { 
                                  EEPROM.write((i+MEM_PASSWORD_MAIL),password_mail[i]);
                                  }
    }

  /* SMTP mail setting */

  if ( server.hasArg("SMTP") ) {       
     reception= server.arg("SMTP");
     int c = reception.length() + 1;        
     reception.toCharArray(smtp_mail, c);
     for (int i = 0; i < c; i++) { 
                                  EEPROM.write((i+MEM_SMTP_MAIL),smtp_mail[i]); 
                                  }
  }

  /* PORT mail setting */

  if ( server.hasArg("PORT") ) {      
    reception = server.arg("PORT");
    int c = reception.length() + 1; 
    reception.toCharArray(port_mail, c);
    portint=reception.toInt();            
    for (int i = 0; i < c; i++) { 
                                   EEPROM.write((i+MEM_PORT_MAIL),port_mail[i]);  
                                  }
    EEPROM.commit();
  }

    /* User mail setting */
  
  if ( server.hasArg("USER") ) {       
     reception= server.arg("USER");
     int c = reception.length() + 1;         
     reception.toCharArray(user, c);
     for (int i = 0; i < c; i++) { 
                                  EEPROM.write((i+MEM_USER_MAIL),user[i]);
                                  }
    }
    
  /* Recipients setting */
  
  if ( server.hasArg("MAILDEST") ) {       // si reception
    reception = server.arg("MAILDEST");
    int c = reception.length() + 1; 
    reception.toCharArray(recipients_mail, c);        
    for (int i = 0; i < c; i++) { 
                                   EEPROM.write((i+MEM_RECIPIENTS_MAIL),recipients_mail[i]);
                                  }
  }

  /* Subject setting */
  
  if ( server.hasArg("MAILOBJ") ) {       
    reception = server.arg("MAILOBJ");
    int c = reception.length() + 1; 
    reception.toCharArray(subject_mail, c);        
    for (int i = 0; i < c; i++) { 
                                   EEPROM.write((i+MEM_SUBJECT_MAIL),subject_mail[i]);
                                  }

    EEPROM.commit();     // Enregistrement
    }


  server.send ( 200, "text/html", mailPage() ); 
}




String mailPage() {
  String  page =F("<!DOCTYPE html> <html lang=fr-FR> <head> <meta charset='utf-8'><link rel='stylesheet'type='text/css'href='style.css'><title>Caméra de surveillance</title></head>");
          page += F("<body>");
          page += F("<nav><ul id='menu'><li><a href='index.html'> Accueil </a></li><li><a href='firmware'> Update </a> </li><li><a href='set_mail'> Mails </a> </li></ul></nav>");
          page += F("<div id='page'><header><h1>Réglage des mails</h1></header>");
          page += F("<div id='corp'><section id='serveur'><h2> Serveur d'envoi</h2><form method='post' accept='' ><p>");
          page += F("<label>Activation des alarmes</label>");
          page += F("<input type='radio' name='MAIL'  value='1' required />");
          page += F("<br/>");
          page += F("<br/>");
          page += F("<label>Désactivations des alarmes</label>");
          page += F("<input type='radio' name='MAIL'  value='0' />");
          page += F("<br/>");
          page += F("<br/>");
          page += F("<label>Mail boîte d'envoi</label>");
          page += F("<input type='mail' name='MAILEXP' id='MAILEXP' value='");page +=user_mail; page +=F("' maxlength='60'/>");
          page += F("<br/>");
          page += F("<br/>");
          page += F("<label>Mots de passe:</label>");
          page += F("<input type='password' name='PW' id='PW' maxlength='60'required/>");
          page += F("<br/>");
          page += F("<br/>");
          page += F("<label>Serveur SMTP:</label>");
          page += F("<input type='text' name='SMTP'value='");page +=smtp_mail; page +=F("' id='SMTP'maxlength='60'/>");
          page += F("<br/>");
          page += F("<br/>");
          page += F("<label>Port:</label>");
          page += F("<input type='number' name='PORT' id='PORT' max='1000' value=' ");page +=port_mail; page +=F("'/>");
          page += F("<br/>");
          page += F("<br/>");
          page += F("<input type='submit'  value='Envoi'>");
          page += F("</p>");
          page += F("</form>");
          page += F("</section>");
          page += F("<section id='Destinataire'>");
          page += F("<h2> Destinataires:</h2>");
          page += F("<form method='post' accept='' >");
          page += F("<p>");
          page += F("<label>Nom de l'expéditeur:</label>");
          page += F("<input type='text' name='USER' id='USER' value='");page +=user; page +=F(" ' maxlength='60' />");
          page += F("<br/>");
          page += F("<br/>");
          page += F("<label>Destinataires:</label>");
          page += F("<input type='mail' name='MAILDEST' id='MAILDEST' value='");page +=recipients_mail; page +=F(" ' maxlength='230' />");
          page += F("<br/>");
          page += F("<br/>");
          page += F("<label>Objet:</label>");
          page += F("<input type='mail' name='MAILOBJ' id='MAILOBJ' value='");page +=subject_mail; page +=F(" ' maxlength='60' />");
          page += F("<br/>");
          page += F("<br/>");
          page += F("<input type='submit'  value='Envoi'>");
          page += F("</p>");
          page += F("</form>");
          page += F("</section>");
          page += F("</div>");
          page += F("<footer>");
          page += F("<a href='http://www.innogreentech.fr'>InnoGreenTech </a>");
          page += F("<a href='mailto: info@innogreentech.fr'>Contactez moi</a>");
          page += F("</footer>");
          page += F("</div>");
          page += F("</body>");
          page += F("</html>");


  return page;
}
