
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <string.h>
#include <FastLED.h>

#define LED_PIN     12 //D6
#define NUM_LEDS    16
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS>leds;


// Update these with values suitable for your network.
int rodada = 0;
int luzes[200];
int aux = 0;
int acertou = 1;
char cor;
const char* ssid = "australopitecos";
const char* password = "47639821";
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("ToMQTT", "hello world");
      // ... and resubscribe
      client.subscribe("fromMQTT");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
      cor = (char)payload[0];
      Serial.print(cor);
}


void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.begin(115200);
  Serial.setTimeout(1);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  randomSeed(analogRead(0)); // Garante que os números serão aleatórios quando o jogo é reiniciado
}

void loop() {
delay(2000); 
      if(!client.connected()) {
      reconnect();
      }

  acertou = 1; // Variável usada para determinar se o jogador acertou a sequência de cores ou não // Variável para guardar a sequência de cores
  int quant = 0; // Quantidade de cores na sequência
  int pont = 0; // Pontuação do jogador
  
  while(acertou == 1){ // Enquanto o jogador continuar acertando...
    fill_solid(leds,NUM_LEDS,CRGB::Black);
    FastLED.show();
    int valor = random(1,5);
    luzes[quant] = valor; // Escolhe aleatoriamente qual luz será a próxima a acender
    quant = quant + 1; // Adiciona 1 a quantidade de cores na sequência
    delay(1000);
    for(int i = 0; i < quant; i++){ // Loop para acender a sequência de luzes uma a uma
      if(luzes[i] == 1){ // Verifica se é esta a luz que deve acender
      
        fill_solid( leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
        Serial.print("red");
      } // A mesma lógica é implementada para as demais luzes
      if(luzes[i] == 2){
        fill_solid( leds, NUM_LEDS, CRGB::Blue);
        FastLED.show();
        Serial.print("blue");
      }
      if(luzes[i] == 3){
        fill_solid( leds, NUM_LEDS, CRGB::Green);
        FastLED.show();
        Serial.print("green");
      }
      if(luzes[i] == 4){
        fill_solid( leds, NUM_LEDS, CRGB::Purple);
        FastLED.show();
        Serial.print("purple");
      }
      delay(1000);
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
    }

    aux = 0; // Variável para apontar qual luz deve acender dentro do vetor
    int terminou = 0; // Variável para verificar se o jogador já terminou a sequência de luzes
    rodada++;
    while(terminou == 0 && acertou == 1){ // Enquanto ainda faltarem luzes para serem pressionadas e o jogador ainda não tiver perdido..
      cor = '\0';
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      while(true){
      client.loop();
        if(cor != '\0') break;
      }

      if(cor == 'r'){ // Se o botão dessa luz for pressionado...
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
      if(cor == 'b'){
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
      if(cor == 'g'){
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
      if(cor == 'p'){
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
      delay(500);
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      delay(2000);
    }

  }
  delay(2000);
  Serial.print("Perdeu");
  exit(0);
}
