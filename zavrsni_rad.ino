#include <ESP32Servo.h>
#include <WiFi.h>

const char* ssid = "HT Internet 4G_2D7C8D";
const char* password = "25289353";

WiFiServer server(80); //defaultni port 80

String zahtjev;

const int pumpa = 25;
const int naprijed = 26;
const int nazad = 27;


String p25s = "off"; //pin 25 stanje, pumpa
String p26s = "off"; //pin 26 stanje, naprijed
String p27s = "off"; //pin 27 stanje, nazad

Servo servo;

unsigned long trenutnoVrijeme = millis();
unsigned long prosloVrijeme = 0; 
const long odbrojavanje = 2000;//2s

void setup() {
  
servo.attach(16,600,2300);  // (pin, PWMmin, PWMmax)
Serial.begin(9600);
pinMode(pumpa, OUTPUT);
pinMode(naprijed, OUTPUT);
pinMode(nazad, OUTPUT);
digitalWrite(pumpa, LOW);
digitalWrite(naprijed, LOW);
digitalWrite(nazad, LOW);

Serial.print("Spajanje sa ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
  }
Serial.println("");
Serial.println("WiFi spojen.");
Serial.println("IP adresa: ");
Serial.println(WiFi.localIP());
server.begin();  
}

void loop() {
  
WiFiClient client= server.available();

if(client){
  trenutnoVrijeme = millis();
  prosloVrijeme = trenutnoVrijeme;
  Serial.println("Novi korisnik.");
  String currentLine = "";
  while (client.connected() && trenutnoVrijeme - prosloVrijeme <= odbrojavanje){
    trenutnoVrijeme = millis();
    if(client.available()){
      char c = client.read();
      Serial.write(c);
      zahtjev += c;
      if(c == '\n') {
        if (currentLine.length() == 0){
          client.println("Spajanje na stranicu");
            client.println("Sadržaj u tekstualnom html-u");
            client.println("Con-Zatv-Beta");
            client.println();
if(zahtjev.indexOf("GET /26/on") >= 0) {
              Serial.println("naprijed");
              p26s = "on";
              digitalWrite(naprijed, HIGH);
            } else if (zahtjev.indexOf("GET /26/off") >= 0) {
              Serial.println("naprijedStop");
              p26s = "off";
              digitalWrite(naprijed, LOW);
            } else if (zahtjev.indexOf("GET /27/on") >= 0) {
              Serial.println("nazad");
              p27s = "on";
              digitalWrite(nazad, HIGH);
            } else if (zahtjev.indexOf("GET /27/off") >= 0) {
              Serial.println("nazadStop");
              p27s = "off";
              digitalWrite(nazad, LOW);
            } else if (zahtjev.indexOf("GET /25/on") >= 0) {
              Serial.println("pumpa");
              p25s = "on";
              digitalWrite(pumpa, HIGH);
            } else if (zahtjev.indexOf("GET /25/off") >= 0) {
              Serial.println("pumpaStop");
              p25s = "off";
              digitalWrite(pumpa, LOW);
            }
            else if (zahtjev.indexOf("GET /lijevo") >=0) {
              Serial.println("lijevo");
              servo.write(0);
            }
            else if (zahtjev.indexOf("GET /ravno") >=0) {
              Serial.println("ravno");
              servo.write(90);
            }
            else if (zahtjev.indexOf("GET /desno") >=0) {
              Serial.println("desno");
              servo.write(180);
            }
             client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons 
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
            client.println("<body><h1>ESP32 Web Server</h1>");
            // Display current state, and ON/OFF buttons for GPIO 26  
            client.println("<p>Naprijed- Stanje " + p26s + "</p>");
            // If the output26State is off, it displays the ON button       
            if (p26s=="off") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">OFF</button></a></p>");
            } 
            client.println("<p>Nazad - Stanje " + p27s + "</p>");
            // If the output27State is off, it displays the ON button       
            if (p27s=="off") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("<p>Pumpa - Stanje " + p25s + "</p>");
            // If the output27State is off, it displays the ON button       
            if (p25s=="off") {
              client.println("<p><a href=\"/25/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/25/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            // client.println("<p>GPIO 25 - State " + p25s + "</p>");
            // If the output27State is off, it displays the ON button       
       
              client.println("<p><a href=\"/lijevo\"><button class=\"button\">Lijevo</button></a></p>");
              client.println("<p><a href=\"/ravno\"><button class=\"button\">Ravno</button></a></p>");
              client.println("<p><a href=\"/desno\"><button class=\"button\">Desno</button></a></p>");
        client.println("</body></html>");
        client.println();
         break;
        }else{
          currentLine = "";
        }
      }else if( c!='\r'){
        currentLine +=c;
      }
    }
  }
  zahtjev = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
}}
