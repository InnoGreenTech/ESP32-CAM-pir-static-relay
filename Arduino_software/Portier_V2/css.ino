
void cssRoot(){
   server.send ( 200, "text/css", cssPage() );   // envoi de la page
  }

String cssPage(){

String page =F("body{display: flex;flex-direction: row;font-family: Arial,sans-serif;color: rgb(72,11,08);}");
       page +=F("h1{text-align: center;font-size: 1.5em; font-weight: bold;}");
       page +=F("h2{text-align: center;font-size: 0.9em;font-weight: bold;}");
       page +=F("h3{text-align: center;font-size: 0.8em;font-weight: bold;}");
       page +=F("#menu li{display: block;}");
       page +=F("#menu{display: flex;flex-direction: column;align-content: space-between;}");
       page +=F("nav li{background-color: rgb(211,218,234);padding: 3px;margin: 5px;border-radius: 10px;box-shadow: 6PX 6PX 6PX silver;}");
       page +=F("nav a{font-size: 1.5em;font-weight: bold;text-decoration: none;color: rgb(72,11,08);}");
       page +=F("#page{width:88%;display: flex;flex-direction: column;}");
       page +=F("header{padding: 2%;margin: 1%;background-color: rgb(211,218,234);border-radius: 10px;box-shadow: 6PX 6PX 6PX silver;}");
       page +=F("#corp{ display: flex;flex-direction: row;flex-wrap: wrap;}");
       page +=F("footer a{color: rgb(148,58,61);}");     
       page +=F("section {width:44%; display: block; background-color:rgb(211,218,234);; padding:2%;margin:1%; border-radius:10px; box-shadow:6PX 6PX 6PX silver;}");
       page +=F("footer{margin:auto;}");     
       page +=F("ul{list-style-type: none;}");
       page +=F("form{text-align: center;}");
       page +=F("@media all and  (orientation: portrait){body{flex-direction: column;} #corp{flex-direction:column;} section{width: 94%;} #menu{display: flex;flex-direction: row;flex-wrap: wrap;justify-content: space-around;}}");
       //page +=F("@media all and  (max-device-width: 480px){body{flex-direction: column;} #corp{flex-direction:column;} section{width: 94%;} #menu{display: flex;flex-direction: row;flex-wrap: wrap;justify-content: space-around;}}");

return page;

}
