#include <WiFi.h>

// Informações do servidor wi-fi
const char* ssid = "lorena-notebook";
const char* password = "ultrabots3";
WiFiServer server(80);
String texto = "HTTP/1.1 200 OK\nContent-type:text/html\n\n";
String responses;

void setup() {

  texto += "Click <a href=\"/R\">here</a> to make the robot run.<br>";
  texto += "Click <a href=\"/S\">here</a> to make the robot stop.<br>";
  texto += "Click <a href=\"/E\">here</a> to know the error.<br>";
  texto += "Click <a href=\"/RESET\">here</a> to reset the error.<br>";
  texto += "Click <a href=\"/P\">here</a> to know the PID parameters.<br>";
  texto += "<p><form action=\"/K\">";
    texto += "<label for=\"kp\">Kp: </label>";
    texto += "<input type=\"text\" id=\"kp\" name=\"kp\"><br>";
    texto += "<label for=\"ki\">Ki: </label>";
    texto += "<input type=\"text\" id=\"ki\" name=\"ki\"><br>";
    texto += "<label for=\"kd\">Kd: </label>";
    texto += "<input type=\"text\" id=\"kd\" name=\"kd\"><br>";
    texto += "<input type=\"submit\" value=\"Submit\">";
  texto += "</form></p><br>";
  texto += "RESPONSES<hr>Click <a href=\"/CLEAR\">here</a> to clear responses.<br><br>";
  
  // Iniciando comunicação serial
  Serial.begin(9600);

  // Inicializando WiFi da placa ESP32
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
  }
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (client) {

    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println(texto);
            client.println(responses);
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET /R")) {
          Serial.write("r");
        }
        if (currentLine.endsWith("GET /S")) {
          Serial.write("s");
        }
        if (currentLine.endsWith("GET /P")){
          Serial.write("k-");
          responses += "[" + String(millis()) + "] " + Serial.readString() + "<br>";
        }
        if (currentLine.endsWith("GET /K")){
          do{
            c = client.read();
            currentLine += c;
          }while(c != ' ');
          Serial.println(currentLine);
          int index1 = currentLine.indexOf("kp");
          currentLine = currentLine.substring(index1);
          currentLine.replace("&", "");
          currentLine.replace("=","");
          Serial.println(currentLine);
        }
        if (currentLine.endsWith("GET /E")) {
          Serial.write("e+");
          float error = Serial.parseFloat();
          responses += "[" + String(millis()) + "] Error at " + String(error) + "<br>";
        }
        if (currentLine.endsWith("GET /RESET")){
          Serial.write("e-");
        }
        if (currentLine.endsWith("GET /CLEAR")){
          responses = String();
        }
      }
    }
    client.stop();
  }
}
