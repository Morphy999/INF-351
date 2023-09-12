#include <LiquidCrystal.h>
#include <string.h>

// Definição de variáveis
#define ledG 10
#define ledR 11
#define ledB 9

int rodada = 0;

void setup() {
  // Declaração dos pinos usados
  Serial.begin(9600);
  Serial.setTimeout(1);
  pinMode(ledG, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledB, OUTPUT);
  randomSeed(analogRead(0)); // Garante que os números serão aleatórios quando o jogo é reiniciado
}

void loop() {
  analogWrite(ledG, 255);
  analogWrite(ledR, 255);
  analogWrite(ledB, 255);
  delay(2000); 

  int acertou = 1; // Variável usada para determinar se o jogador acertou a sequência de cores ou não
  int luzes[200]; // Variável para guardar a sequência de cores
  int quant = 0; // Quantidade de cores na sequência
  int pont = 0; // Pontuação do jogador
  
  while(acertou == 1){ // Enquanto o jogador continuar acertando...
    Serial.print("rodada ");
    Serial.print(rodada+1);
    int valor = random(1,5);
    luzes[quant] = valor; // Escolhe aleatoriamente qual luz será a próxima a acender
    quant = quant + 1; // Adiciona 1 a quantidade de cores na sequência
    
    for(int i = 0; i < quant; i++){ // Loop para acender a sequência de luzes uma a uma
      if(luzes[i] == 1){ // Verifica se é esta a luz que deve acender
        analogWrite(ledR, 0); // Se sim, liga o led correspondente
        delay(500);
        analogWrite(ledR, 255); // Depois de um tempo, apaga a luz
        delay(2000);
      } // A mesma lógica é implementada para as demais luzes
      if(luzes[i] == 2){
        analogWrite(ledG, 0);
        delay(500);
        analogWrite(ledG, 255);
        delay(2000);
      }
      if(luzes[i] == 3){
        analogWrite(ledB, 0);
        delay(500);
        analogWrite(ledB, 255);
        delay(2000);
      }
      if(luzes[i] == 4){
        analogWrite(ledR,100);
        analogWrite(ledB,20);
        delay(500);
        analogWrite(ledR,255);
        analogWrite(ledB,255);
        delay(2000);
      }
    }

    int aux = 0; // Variável para apontar qual luz deve acender dentro do vetor
    int terminou = 0; // Variável para verificar se o jogador já terminou a sequência de luzes
    rodada++;
    while(terminou == 0 && acertou == 1){ // Enquanto ainda faltarem luzes para serem pressionadas e o jogador ainda não tiver perdido..
      while (!Serial.available()); 
      String x = Serial.readString();
      x.trim();  


      if(x == "r"){ // Se o botão dessa luz for pressionado...
        analogWrite(ledR, 0); // O led acende
        if(luzes[aux] == 1){ // Verifica se o jogador acertou a luz correta
          aux++; // Se sim, a próxima luz é apontada
        }
        else{
          acertou = 0; // Se não, o jogador errou
        }
        delay(500);
        analogWrite(ledR, 255); // A luz se apaga
        delay(1000);
      }
       // A mesma lógica é implementada para todas as luzes
      if(x == "g"){
        analogWrite(ledG, 0);
        if(luzes[aux] == 2){
          aux++;
        }
        else{
          acertou = 0;
        }
        delay(500);
        analogWrite(ledG,255);
        delay(1000);
      }
      if(x == "b"){
        analogWrite(ledB, 0);
        if(luzes[aux] == 3){
          aux++;
        }
        else{
          acertou = 0;
        }
        delay(500);
        analogWrite(ledB, 255);
        delay(1000);
      }
      if(x == "p"){
        analogWrite(ledR,100);
        analogWrite(ledB,20);
        if(luzes[aux] == 4){
          aux++;
        }
        else{
          acertou = 0;
        }
        delay(500);
        analogWrite(ledR,255);
        analogWrite(ledB,255);
        delay(1000);
      }
      if(aux >= quant){ // Se o jogador já terminou a sequência...
        terminou = 1;
      } 
      if(acertou == 0){
        rodada = 0;
        break;
      }
  
    }

  }
  delay(3000);
  Serial.print("Perdeu");
  exit(0);
}
