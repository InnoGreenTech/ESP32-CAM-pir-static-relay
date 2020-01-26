void send_mail(){

      /*set configuration of mail*/

      smtpData.setLogin(smtp_mail,portint,user_mail,password_mail);
      smtpData.setSender(user,user_mail);
      smtpData.setPriority("High");
      smtpData.setSubject(subject_mail);
      
      
      /*Create the Body of the mail*/

      String corp=F("<!DOCTYPE html><html><head><meta charset=''utf-8'/></head><body>");
      corp +=F("<p>Bonjour</p>");
      corp +=F("<p>Une présence a été détectée :</p>");
      corp +=F("<ul>");
      corp +=F("<li>Date: "); corp +=NTP.getDateStr();corp +=F(" </li>");
      corp +=F("<li>Heure: "); corp +=NTP.getTimeStr();corp +=F(" </li>");
      corp +=F("</ul>");
      corp+=F("<p>Cordialement</p>");
      corp+=F("<a href='http://www.innogreentech.fr'>InnoGreenTech</a></body></html>"); 
      int y = corp.length() + 1;
      corp.toCharArray(message_mail,y);

      smtpData.setMessage(message_mail,true);

     /* Add every recipients */ 

      char c='1';  
      int a=0;
      int b=0;
      while (!( c=='\0')&&a<250){
                                c=recipients_mail[a];
                                if (c==';'||c=='\0'){
                                    recipient_mail[b]='\0';
                                    smtpData.addRecipient(recipient_mail);
                                    b=0;
                                    }
                                else {   
                                    recipient_mail[b]=c;
                                    b++;
                                     }                       
                                a++;                                         
                                }
      
    /* Add picture of the camera */ 
      
     cam.run();
     smtpData.addAttachData("capture.jpeg", "image/jpeg", cam.getfb(), cam.getSize());

   /*send email and clear memory*/
      smtpData.setSendCallback(sendCallback);
      if (!MailClient.sendMail(smtpData))
      Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
      smtpData.empty();


}
void sendCallback(SendStatus msg)
{
  //Print the current status
  Serial.println(msg.info());

  //Do something when complete
  if (msg.success())
  {
    Serial.println("----------------");
  }
}

  
