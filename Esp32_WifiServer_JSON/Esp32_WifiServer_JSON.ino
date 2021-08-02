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
String response_header = "HTTP/1.1 200 OK\nContent-type:application/json\n\n";
String response;
String self_ip;

void setup() {
  
  // Iniciando comunicação serial
  Serial.begin(9600);

  // Inicializando WiFi da placa ESP32
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
  }
  server.begin();

  self_ip = WiFi.localIP().toString();
  
  // Construção do texto base da página HTML renderizada pelo servidor
  texto += "Check <a href=\"/R\">" + self_ip + "/R</a> to make the robot run.<br>";
  texto += "Check <a href=\"/STOP\">" + self_ip + "/S</a> to make the robot stop.<br>";
  texto += "Check <a href=\"/SPEED\">" + self_ip + "/MAX</a> to know the maximum speed.<br>";
  texto += "Check <a href=\"/E\">" + self_ip + "/E</a> to know the error.<br>";
  texto += "Check <a href=\"/RESET\">" + self_ip + "/RESET</a> to reset the error.<br>";
  texto += "Check <a href=\"/P\">" + self_ip + "/P</a> to know the PID parameters.<br>";
  texto += "Check <a href=\"/K?kp=1&ki=2&kd=3\">" + self_ip + "/K?kp=v1&ki=v2&kd=v3</a> where v1, v2 and v3 will be the new kp, ki and kd values.<br>";
  texto += "Check <a href=\"/M?m=100\">" + self_ip + "/M?m=v1</a> where v1 will be the new maximum speed.<br>";
  
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (client) {

    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
        if (currentLine.endsWith("GET / ")){
          client.println(texto);
        }
        if (currentLine.endsWith("GET /R")) {
          //Serial.write("r");
          response = response_header + "{}";
          client.print(response);
        }
        if (currentLine.endsWith("GET /STOP")) {
          //Serial.write("s");
          response = response_header + "{}";
          client.print(response);
        }
        if (currentLine.endsWith("GET /P")){
          //Serial.write("w");
          while(!Serial.available()){
            
          }
          response = response_header + "{ 'time' : " + String(millis()) + "," + Serial.readString() + "}";
          client.print(response);
        }
        if (currentLine.endsWith("GET /SPEED")){
          //Serial.write("g");
          while(!Serial.available()){
            
          }
          response = response_header + "{ 'time' : " + String(millis()) + ", 'maximum':" + Serial.readString() + "}";
          client.print(response);
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
          //Serial.println(currentLine);
          response = response_header + "{}";
          client.print(response);
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
          //Serial.println(currentLine);
          response = response_header + "{}";
          client.print(response);
        }
        if (currentLine.endsWith("GET /E")) {
          //Serial.write("e");
          while(!Serial.available()){
            
          }
          float error = Serial.parseFloat();
          response = response_header + "{ 'time' : " + String(millis()) + ", 'error':" +  String(error, 6) + "}";
          client.print(response);
        }
        if (currentLine.endsWith("GET /RESET")){
          //Serial.write("z");
          response = response_header + "{}";
          client.print(response);
        }
      }
    }
    client.stop();
  }
}
