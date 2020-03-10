
#include <SPI.h>
#include <Ethernet.h>
#include <dht.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte ip[] = { 192, 168, 2, 199 };
     
EthernetServer server(80);
  
String readString;
int Pin = 6;

dht DHT; // Cria um objeto da classe dht
void setup(){
   Serial.begin(9600);
  
  pinMode(Pin, OUTPUT);
  Ethernet.begin(mac, ip);
  server.begin();
}
  
void loop(){

 
  
  EthernetClient client = server.available();

  if (client) {
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
  
        if (readString.length() < 100) {
          readString += c;             
        }
       
        DHT.read11(A1);
 
        if (c == '\n') {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
           
          client.println("<HTML>");
          client.println("<BODY>");
          client.println("<br />");
          client.print("<h4> Temperatura: ");
          client.println(DHT.temperature);
          client.print("Humidade: ");
          client.println(DHT.humidity);
          client.println("</h4>");
          if(readString.indexOf("?ligada") > 0)
          {
          client.println("<h4> <a href=\"/?desligado\"> <button style='width:100%; font-size:2rem'> Desligar</button> </a> </h4>"); 
            digitalWrite(Pin, HIGH);
          }
          else {
             client.println("<h4> <a href=\"/?ligada\"> <button style='width:100%; font-size:2rem'> Ligar</button> </a> </h4>");
            if(readString.indexOf("?desligado") > 0)
            {
              digitalWrite(Pin, LOW);
            }
          }
          
           
           
          client.println("</BODY>");
          client.println("</HTML>");
           
          delay(1);
          client.stop();
           
          
          readString="";    
        }
      }
    }
  }
}
