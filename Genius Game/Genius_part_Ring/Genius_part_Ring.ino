#include <string.h>
#include <FastLED.h>

#define LED_PIN     12
#define NUM_LEDS    16
#define BRIGHTNESS  255
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGBArray<NUM_LEDS>leds;

int rodada = 0;

void setup() {
  // Declaração dos pinos usados
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  Serial.begin(115200);
  Serial.setTimeout(1);
  randomSeed(analogRead(0)); // Garante que os números serão aleatórios quando o jogo é reiniciado
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
      
        leds[0] = leds[1] = leds[2] = leds[3] = CRGB::Red;
        FastLED.show();
        Serial.print("red");
      } // A mesma lógica é implementada para as demais luzes
      if(luzes[i] == 2){
        leds[4] = leds[5] = leds[6] = leds[7] = CRGB::Blue;
        FastLED.show();
        Serial.print("blue");
      }
      if(luzes[i] == 3){
        leds[8] = leds[9] = leds[10] = leds[11] = CRGB::Green;
        FastLED.show();
        Serial.print("green");
      }
      if(luzes[i] == 4){
        leds[12] = leds[13] = leds[14] = leds[15] = CRGB::Purple;
        FastLED.show();
        Serial.print("purple");
      }
      delay(1000);
      fill_solid( leds, NUM_LEDS, CRGB::Black);
      FastLED.show();
    }

    int aux = 0; // Variável para apontar qual luz deve acender dentro do vetor
    int terminou = 0; // Variável para verificar se o jogador já terminou a sequência de luzes
    rodada++;
    while(terminou == 0 && acertou == 1){ // Enquanto ainda faltarem luzes para serem pressionadas e o jogador ainda não tiver perdido..

      while (!Serial.available()); 
      String x = Serial.readString();
      x.trim();  


      if(x == "r"){ // Se o botão dessa luz for pressionado...
        leds[0] = leds[1] = leds[2] = leds[3] = CRGB::Red;
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
      if(x == "b"){
        leds[4] = leds[5] = leds[6] = leds[7] = CRGB::Blue;
        FastLED.show();
        if(luzes[aux] == 2){
          aux++;
        }
        else{
          acertou = 0;
          break;
        }
      }
      if(x == "g"){
        leds[8] = leds[9] = leds[10] = leds[11] = CRGB::Green;
        FastLED.show();
        if(luzes[aux] == 3){
          aux++;
        }
        else{
          acertou = 0;
          break;
        }
      }
      if(x == "p"){
        leds[12] = leds[13] = leds[14] = leds[15] = CRGB::Purple;
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
  
      delay(2000);
    }

  }
  delay(2000);
  Serial.print("Perdeu");
}