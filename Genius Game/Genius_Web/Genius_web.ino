#include <string.h>
#include <FastLED.h>

#include <ESP8266WiFi.h>
#define LED_PIN     12
#define NUM_LEDS    16
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS>leds;

int rodada = 0;
boolean statusOUT1 = HIGH;
//configurações da rede
const char* ssid = "australopitecos";
const char* senha = "47639821";
WiFiServer server(80); //instncia o server na porta 80

void setup() {
  // Declaração dos pinos usados
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.begin(115200);
  Serial.setTimeout(1);
  randomSeed(analogRead(0)); // Garante que os números serão aleatórios quando o jogo é reiniciado

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
  delay(2000); 

  int acertou = 1; // Variável usada para determinar se o jogador acertou a sequência de cores ou não
  int luzes[200]; // Variável para guardar a sequência de cores
  int quant = 0; // Quantidade de cores na sequência
  int pont = 0; // Pontuação do jogador
  
  while(acertou == 1){ // Enquanto o jogador continuar acertando...
    fill_solid(leds,NUM_LEDS,CRGB::Black);
    FastLED.show();
    int valor = random(1,5);
    luzes[quant] = valor; // Escolhe aleatoriamente qual luz será a próxima a acender
    quant = quant + 1; // Adiciona 1 a quantidade de cores na sequência
    
    for(int i = 0; i < quant; i++){ // Loop para acender a sequência de luzes uma a uma
      if(luzes[i] == 1){ // Verifica se é esta a luz que deve acender
      
        fill_solid( leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
      } // A mesma lógica é implementada para as demais luzes
      if(luzes[i] == 2){
        fill_solid( leds, NUM_LEDS, CRGB::Blue);
        FastLED.show();
      }
      if(luzes[i] == 3){
        fill_solid( leds, NUM_LEDS, CRGB::Green);
        FastLED.show();
      }
      if(luzes[i] == 4){
        fill_solid( leds, NUM_LEDS, CRGB::Purple);
        FastLED.show();
      }
      delay(1000);
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      delay(1000);
      FastLED.show();
    }

    int aux = 0; // Variável para apontar qual luz deve acender dentro do vetor
    int terminou = 0; // Variável para verificar se o jogador já terminou a sequência de luzes
    rodada++;
    while(terminou == 0 && acertou == 1){ // Enquanto ainda faltarem luzes para serem pressionadas e o jogador ainda não tiver perdido..
      WiFiClient client = server.available();
      while(!client) client = server.available();
      Serial.println("connect");
      while (!client.available()) {
          delay(1);
      }
      String req = client.readStringUntil('\r');
      Serial.println(req);
      client.flush();
      Serial.print("html.....");
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("");
      client.println("<!DOCTYPE HTML>");
      client.println("<html>");
      client.println("<h1>Escolha uma cor </h1>");
    

      if (req.indexOf("ioonred") != -1)
      {
        Serial.print("red");
        fill_solid( leds, NUM_LEDS, CRGB::Red);
        FastLED.show();  
        if(luzes[aux] == 1){ // Verifica se o jogador acertou a luz correta
            aux++; // Se sim, a próxima luz é apontada
          }
          else{
            acertou = 0; // Se não, o jogador errou
            break;
          }
        }
       // A mesma lógica é implementada para todas as luzes
      if(req.indexOf("ioonBlue") != -1){
        Serial.print("b");
          fill_solid( leds, NUM_LEDS, CRGB::Blue);
          FastLED.show();
        if(luzes[aux] == 2){
          aux++;
        }
        else{
          acertou = 0;
          break;
        }
      }
      if(req.indexOf("ioonGreen") != -1){
        Serial.print("G");
        fill_solid( leds, NUM_LEDS, CRGB::Green);
        FastLED.show();
        if(luzes[aux] == 3){
          aux++;
        }
        else{
          acertou = 0;
          break;
        }
      }
      if(req.indexOf("ioonPurple") != -1){
        Serial.print("P");
        fill_solid( leds, NUM_LEDS, CRGB::Purple);
        FastLED.show();
        if(luzes[aux] == 4){
          aux++;
        }
        else{
          acertou = 0;
          break;
        }
      }

      if(aux >= quant){ // Se o jogador já terminou a sequência...
        terminou = 1;
      } 
      if(acertou == 0){
        rodada = 0;
        break;
      }
      delay(1000);
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      FastLED.show();

      client.println("<p>RED <a href=\"ioonred\"><button>LIGAR</button></a></p>");
      client.println("<p>BLUE <a href=\"ioonBlue\"><button>LIGAR</button></a></p>");
      client.println("<p>GREEN <a href=\"ioonGreen\"><button>LIGAR</button></a></p>");
      client.println("<p>PURPLE <a href=\"ioonPurple\"><button>LIGAR</button></a></p>");
    }

  }
  delay(2000);
  Serial.print("Perdeu");
  exit(0);
}
