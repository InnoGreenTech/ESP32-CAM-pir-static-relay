
void indexRoot(){
              if ( server.hasArg("envoi")) 
              {      
                send_mail();
              } 
               server.send ( 200, "text/html", indexPage() );   // envoi de la page
                }
  


String indexPage(){
                    String page =F("<!DOCTYPE html> <html lang=fr-FR> <head> <meta charset='utf-8'><link rel='stylesheet'type='text/css'href='style.css'><title>Caméra de surveillance</title></head>");
                           page +=F("<body>");
                           page +=F("<nav> <ul id='menu'><li><a href='index.html'> Accueil </a></li><li><a href='firmware'> Update </a></li><li><a href='set_mail.html'> Mails </a> </li></ul></nav>");
                           page +=F("<div id='page'>");
                           page +=F("<header><h1>Caméra de surveillance </h1></header>");
                           page +=F("<div id='corp'>");
                           page +=F("<section id='datedujour'><h2>");
                           page +=NTP.getDateStr();
                           page +=F("</h2><h3>");
                           page +=NTP.getTimeStr();
                           page +=F("</h3>");
                           page +=F("<ul><li>Serveur:");
                           page +=ip_server;
                           page +=F("</li><li> Port serveur:");
                           page +=port_server;
                           page +=F("</li><li> Adresse MAC:");
                           page +=local_mac;
                           page +=F("</li><li> Version:");
                           page +=version_module;
                           page +=F("</li></ul></section>");
                           page +=F("<section id='capteur1'><h2> Envoi capture par mail</h2><ul>");
                           page +=F("<li><form method='get'><input type='hidden' name='envoi' value='1'/><input type='submit' value='capture'/>");                      
                           page +=F("</form></li></ul></section>");
                           page +=F("</div>");
                           page +=F("<footer><a href='http://www.Innogreentech.fr'>InnoGreenTech </a><a href='mailto: info@innogreentech.fr'>Contactez moi</a></footer>");
                           page +=F("</div></body></html>");                  
                           return page;
                          }
