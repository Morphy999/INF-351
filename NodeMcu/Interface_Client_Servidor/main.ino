#include <ESP8266WiFi.h>
#include <FastLED.h>

#define OUT1 12
#define LED_PIN     12 //D6
#define NUM_LEDS    16
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS>leds;

boolean statusOUT1 = HIGH;
//configurações da rede
const char* ssid = "australopitecos";
const char* senha = "47639821";
WiFiServer server(80); //instncia o server na porta 80
void setup() {
Serial.begin(115200); //configura comunicação serial
delay(10);
//inicializaçao FastLED
FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
FastLED.setBrightness(  BRIGHTNESS );

//configura pino de saída
pinMode(OUT1, OUTPUT);
// mensagem de debug serial
Serial.print("Conectando para a rede ");

Serial.println(ssid);
// Iniciando a conexão WiFi
WiFi.begin(ssid, senha);
// aguarda a conexão WIFI
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
server.begin();
Serial.println("Servidor inicializado!");
//mesagem de conectado
Serial.println("");
Serial.println("WiFi conectado");
Serial.println("Endereço IP: ");
Serial.println(WiFi.localIP());
}
void loop() {
// Verifica se o cliente está conectado
WiFiClient client = server.available();
if (!client) {
return;
}
// Aguarda dados do cliente
Serial.println("connect");
while (!client.available()) {
delay(1);
}
String req = client.readStringUntil('\r');
Serial.println(req);
client.flush();


//A partir daqui verifica qual botao foi clickado

if (req.indexOf("ioonred") != -1)
{
fill_solid( leds, NUM_LEDS, CRGB::Red);
FastLED.show();
statusOUT1 = HIGH;
}
else if (req.indexOf("ioonBlue") != -1)
{
fill_solid( leds, NUM_LEDS, CRGB::Blue);
FastLED.show();
statusOUT1 = HIGH;
}

else if (req.indexOf("ioonGreen") != -1)
{
fill_solid( leds, NUM_LEDS, CRGB::Green);
FastLED.show();
statusOUT1 = HIGH;
}

//Aqui caso tenha clickado em Desligar

else if (req.indexOf("iooff") != -1)
{
fill_solid( leds, NUM_LEDS, CRGB::Black);
FastLED.show();
statusOUT1 = LOW;
}


//HTML para a Criaçao da pagina

client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("");
client.println("<!DOCTYPE HTML>");
client.println("<html>");
client.println("<h1>Escolha qual deseja ligar</h1>");

//caso esteja Desligado,aparece o 3 botoes

if(!statusOUT1){
client.println("<p>RED <a href=\"ioonred\"><button>LIGAR</button></a></p>");
client.println("<p>BLUE <a href=\"ioonBlue\"><button>LIGAR</button></a></p>");
client.println("<p>GREEN <a href=\"ioonGreen\"><button>LIGAR</button></a></p>");
}

//Se tiver ligado,aparece o botao de desligar
else
client.println("<p><a href=\"iooff\"><button>DESLIGAR</button></a></p>");
client.println("</html>");
delay(10);
}

