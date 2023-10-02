#include <FastLED.h>
#include <string.h>
const int trigPin = 15;
const int echoPin = 13;
const int ledPin = 12;


//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define CM_TO_INCH 0.393701
#define NUM_LEDS    16
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS>leds;

long duration;
float distanceCm;

int rodada = 0;

void setup() {
  // Declaração dos pinos usados
  FastLED.addLeds<LED_TYPE, ledPin, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  Serial.begin(115200);
  Serial.setTimeout(1);
  randomSeed(analogRead(0)); // Garante que os números serão aleatórios quando o jogo é reiniciado
}

void loop() {
  delay(2000);
  Serial.print("começando\n");
  delay(2000); 

  int acertou = 1; // Variável usada para determinar se o jogador acertou a sequência de cores ou não
  int luzes[200]; // Variável para guardar a sequência de cores
  int quant = 0; // Quantidade de cores na sequência
  int pont = 0; // Pontuação do jogador
  
  while(acertou == 1)
  { // Enquanto o jogador continuar acertando...
    fill_solid(leds,NUM_LEDS,CRGB::Black);
    FastLED.show();
    int valor = random(1,5);
    luzes[quant] = valor; // Escolhe aleatoriamente qual luz será a próxima a acender
    quant = quant + 1; // Adiciona 1 a quantidade de cores na sequência
    
    for(int i = 0; i < quant; i++){ // Loop para acender a sequência de luzes uma a uma
      if(luzes[i] == 1){ // Verifica se é esta a luz que deve acender
      
        fill_solid( leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
        // Serial.print("red\n");
      } // A mesma lógica é implementada para as demais luzes
      if(luzes[i] == 2){
        fill_solid( leds, NUM_LEDS, CRGB::Blue);
        FastLED.show();
        // Serial.print("blue\n");
      }
      if(luzes[i] == 3){
        fill_solid( leds, NUM_LEDS, CRGB::Green);
        FastLED.show();
        // Serial.print("green\n");
      }
      if(luzes[i] == 4){
        fill_solid( leds, NUM_LEDS, CRGB::Purple);
        FastLED.show();
        // Serial.print("purple\n");
      }
      delay(1000);
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      delay(1000);
    }

    int aux = 0; // Variável para apontar qual luz deve acender dentro do vetor
    int terminou = 0; // Variável para verificar se o jogador já terminou a sequência de luzes
    rodada++;

    while(terminou == 0 && acertou == 1){ // Enquanto ainda faltarem luzes para serem pressionadas e o jogador ainda não tiver perdido..
      Serial.print(luzes[aux]);
      bool isRed = false;
      bool isBlue = false;
      bool isGreen = false;
      bool isPurple = false;
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
      while(true)
      {
        // Clears the trigPin;
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        // Sets the trigPin on HIGH state for 10 micro seconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        // Reads the echoPin, returns the sound wave travel time in microseconds
        duration = pulseIn(echoPin, HIGH);
        // Calculate the distance
        distanceCm = duration * SOUND_VELOCITY/2;
        Serial.print(distanceCm);
        Serial.print("\n");
        if(distanceCm <= 10.00 && distanceCm > 0.00){
          isRed = true;
          break;
        }
        else if(distanceCm > 10 && distanceCm <= 20){
          isBlue = true;
          break;
        }
        else if(distanceCm > 20 && distanceCm <= 30){
          isGreen = true;
          break;
        }
        else if(distanceCm > 30 && distanceCm <= 40){
          isPurple = true;
          break;
        }
        else{
          delay(1000);
        }
      }

      if(isRed){ // Se o botão dessa luz for pressionado...
        fill_solid( leds, NUM_LEDS, CRGB::Red);
        FastLED.show();
        // Serial.print("red\n");
        if(luzes[aux] == 1){ // Verifica se o jogador acertou a luz correta
          aux++; // Se sim, a próxima luz é apontada
        }
        else{
          acertou = 0; // Se não, o jogador errou
          break;
        }
      }
       // A mesma lógica é implementada para todas as luzes
      if(isBlue){
        fill_solid( leds, NUM_LEDS, CRGB::Blue);
        FastLED.show();
        // Serial.print("Blue");
        if(luzes[aux] == 2){
          aux++;
        }
        else{
          acertou = 0;
          break;
        }
      }
      if(isGreen){
        fill_solid( leds, NUM_LEDS, CRGB::Green);
        FastLED.show();
        // Serial.print("green\n");
        if(luzes[aux] == 3){
          aux++;
        }
        else{
          acertou = 0;
          break;
        }
      }
      if(isPurple){
        fill_solid( leds, NUM_LEDS, CRGB::Purple);
        FastLED.show();
        // Serial.print("purple\n");
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
      delay(2000);
    }

  }
  
  delay(2000);
  fill_solid( leds, NUM_LEDS, CRGB::Black);
  FastLED.show();
  Serial.print("Perdeu\n");
}