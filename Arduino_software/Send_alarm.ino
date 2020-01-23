/*void send_alarm() {

  cam.run();

  uint8_t *image = cam.getfb();
  Serial.println(cam.getSize());
  unsigned long lenght_string=cam.getSize();;
  String start_request = ""; String end_request = "";
  start_request = start_request + "\r\n--AaB03x\r\n" + "Content-Disposition: form-data; name=\"userfile\"; filename=\"CAM.jpg\"\n" + "Content-Type: image/jpeg\r\n";
  end_request = end_request + "\r\n--AaB03x--\r\n"; 
  uint16_t full_length;
  full_length = start_request.length() + lenght_string + end_request.length();

if (client.connect(ip_server, port_server))
  {    
    client.print("POST ");
    client.println("/InnoGreenTech/controller/post_camera.php HTTP/1.1");
    client.print ("Host:");
    client.println (ip_server);
    client.println("Content-Type: multipart/form-data; boundary=AaB03x");
    client.print  ("Content-Length: "); client.println(full_length);
    client.println(start_request);
    client.write(image, lenght_string);
    client.println(end_request);
  }
  else {Serial.println("client non coneecté");}
  
  unsigned long time_out_connection = millis();

   /* wait for data to be available*/
   /* unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> client Timeout !");
        goto fin;
      }
    }

    while (client.available()) {
      char ch = static_cast<char>(client.read());
      Serial.print(ch);
    }
  fin:
    client.flush();
    client.stop();      //déconnexion

   client = rtspServer.accept();

}*/
