/* Código de servidor Wi-Fi ESP32 para controlar robô Pololu 3pi
 * @author: Lorena Bassani
 * @date: Julho 2021
 * @version: 1.1
 * @descrição:  Baseado no tutorial "ESP32 WIFI: COMUNICAÇÃO COM A INTERNET" por Gustavo Teixeira. Acesso https://www.usinainfo.com.br/blog/esp32-wifi-comunicacao-com-a-internet/ em 29/07/2021.
 *                Roda pequeno servidor disponibilizado na rede wi-fi local para configuração e controle de robô programado com controlador PID. Utilizado na realização de IC.
 *                Versão mais recente disponível em https://github.com/LBBassani/PID-Controller.
 */

#include <WiFi.h>

// Informações do servidor wi-fi
const char* ssid = "lorena-notebook";
const char* password = "ultrabots3";
WiFiServer server(80);

// Variáveis de resposta do servidor
String texto = "HTTP/1.1 200 OK\nContent-type:text/html\n\n";
String responses;

void setup() {

  // Construção do texto base da página HTML renderizada pelo servidor
  texto += "Click <a href=\"/R\">here</a> to make the robot run.<br>";
  texto += "Click <a href=\"/STOP\">here</a> to make the robot stop.<br>";
  texto += "Click <a href=\"/SPEED\">here</a> to know the maximum speed.<br>";
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
  texto += "<p><form action=\"/M\">";
    texto += "<label for=\"m\">Maximum: </label>";
    texto += "<input type=\"text\" id=\"m\" name=\"m\"><br>";
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
        if (currentLine.endsWith("GET /STOP")) {
          Serial.write("s");
        }
        if (currentLine.endsWith("GET /P")){
          Serial.write("w");
          while(!Serial.available()){
            
          }
          responses += "[" + String(millis()) + "] " + Serial.readString() + "<br>";
        }
        if (currentLine.endsWith("GET /SPEED")){
          Serial.write("g");
          while(!Serial.available()){
            
          }
          responses += "[" + String(millis()) + "] Maximum speed: " + Serial.readString() + "<br>";
        }
        if (currentLine.endsWith("GET /K")){
          do{
            c = client.read();
            currentLine += c;
          }while(c != ' ');
          int index1 = currentLine.indexOf("kp");
          currentLine = currentLine.substring(index1);
          currentLine.replace("&", "");
          currentLine.replace("=", "");
          currentLine.replace("k", "");
          Serial.println(currentLine);
        }
        if (currentLine.endsWith("GET /M")) {
          do{
            c = client.read();
            currentLine += c;
          }while(c != ' ');
          int index1 = currentLine.indexOf("m");
          currentLine = currentLine.substring(index1);
          currentLine.replace("&", "");
          currentLine.replace("=", "");
          Serial.println(currentLine);
        }
        if (currentLine.endsWith("GET /E")) {
          Serial.write("e");
          while(!Serial.available()){
            
          }
          float error = Serial.parseFloat();
          responses += "[" + String(millis()) + "] Error at " + String(error, 6) + "<br>";
        }
        if (currentLine.endsWith("GET /RESET")){
          Serial.write("z");
        }
        if (currentLine.endsWith("GET /CLEAR")){
          responses = String();
        }
      }
    }
    client.stop();
  }
}
