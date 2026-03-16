/*

Jogo Simon para Arduino

Para iniciar, pressione e matenha pressionada qualquer tecla. 

Para recomeçar pressione RESET

Para Limpar a memoria, pressione e mantenha pressionada as teclas
1 e 4, ate ouvir o beep.


*/


//Adiciona biblioteca para Cristal Liquido
#include <LiquidCrystal.h>

//Adiciona biblioteca para EEPROM
#include <EEPROM.h>


//Configura pinos ligados ao display LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//Quantidade de jogadas maxima
const int JOGADAS_SIZE = 100;


//Velocidade dos leds durante exibicao inicial
const int DELAY_APRESENTACAO = 150;
//
//Teclas
const int key1 = 6; 
const int key2 = 7;
const int key3 = 8;
const int key4 = 9; 
//
//Leds
const int led1 = A0; //14
const int led2 = A1; //15
const int led3 = A2; //16
const int led4 = A3; //17
//
//Pino ligado ao Buzzer
const int pin10 = 10;
//
//Tons emitidos pelo buzzer
const int som1 = 50;
const int som2 = 100;
const int som3 = 150;
const int som4 = 200;
//
//Vetor de jogadas do computador
int cpu[JOGADAS_SIZE]; //quantidade de jogadas
int tempoNivel = 250;  //velocidade do jogo
//Variave de controle da jogada atual
int jogadaAtual = 0;
//Recorde
int recorde = 0; 
//
boolean resultado = true; 
//  -led in 17 key 4  //ligacao das teclas e saidas (led e teclas)
//  -led in 16 key 3
//  -led in 15 key 1
//  -led in 14 key 1

//

void setup(){


  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(key1, INPUT);
  pinMode(key2, INPUT);
  pinMode(key3, INPUT);
  pinMode(key4, INPUT);
  //
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  //
  pinMode(10, OUTPUT);

  limpaMemoria();

  //
  // como a entrada analogica 5 nao esta conectada
  // o ruido vai fazer com que o metodo
  // randomSeed(), que inicializa o gerador
  // de numeros pseudo-aleatorios da funcao random()
  randomSeed(analogRead(5)); 


  carregaRecorde();

  preencheVetor();
  escreveVetor();

  apresenta();

}

void loop(){

  escreveLcd();
  jogadaCpu(); 

  resultado= lerTeclado(); 

  if(resultado == false){

    lcd.setCursor(0, 1);
    lcd.print("Voce Falhou"); 

    checaRecorde();

    errou();

  }
  else{
    
    delay(1000);
    
  };

}

void limpaMemoria(){

  if(digitalRead(key1)
    && digitalRead(key4)){

    digitalWrite(pin10, HIGH);
    delay(50);
    digitalWrite(pin10,LOW);
    lcd.clear();
    lcd.print("Limpando Memor..");

    for (int i = 0; i < 512; i++)
      EEPROM.write(i, 0);

    digitalWrite(pin10, HIGH);
    lcd.clear();
    lcd.print("Memoria Limpa!");
    delay(2000);
    digitalWrite(pin10,LOW);
  }


}

void carregaRecorde(){

  recorde = EEPROM.read(0);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Recorde: "); 
  lcd.setCursor(11, 1);
  lcd.print(recorde); 
}

void checaRecorde(){

  if(jogadaAtual > recorde){

    recorde = jogadaAtual; 
    EEPROM.write(0, recorde);

    lcd.clear(); 
    lcd.setCursor(0,0);
    lcd.print("Novo Recorde");

    lcd.setCursor(14,0);
    lcd.print(recorde);

  }

}

void somOn(int s){

  switch(s){

  case 14:
    analogWrite(pin10, som1);
    break;


  case 15:
    analogWrite(pin10, som2);
    break;


  case 16:
    analogWrite(pin10, som3);
    break;



  case 17:
    analogWrite(pin10, som4);
    break;
  }

}

void somOff(){

  analogWrite(pin10, 0); 

}

void jogadaCpu(){

  for(int j = 0; j<=jogadaAtual; j++){
    somOff();
    digitalWrite(cpu[j], LOW);
    delay(tempoNivel);     
    digitalWrite(cpu[j], HIGH);
    somOn(cpu[j]);
    delay(tempoNivel); 
    digitalWrite(cpu[j], LOW);
    somOff();
  }

}

void preencheVetor(){

  for(int x = 0; x < JOGADAS_SIZE; x++){

    cpu[x] = random(14,18);

  }

}

void escreveVetor(){

  for(int x = 0; x < JOGADAS_SIZE; x++){

    Serial.print(cpu[x]);
    Serial.print(" - ");
  }
  Serial.print("\n");

}

void errou(){

  analogWrite(pin10, 50);
  delay(500);
  analogWrite(pin10, 40);

  delay(500);            
  analogWrite(pin10, 30);
  delay(500);
  analogWrite(pin10, 20);
  delay(1000);
  somOff();

  while(true){
    delay(DELAY_APRESENTACAO);
    digitalWrite(14, HIGH);
    digitalWrite(15, HIGH);
    digitalWrite(16, HIGH);
    digitalWrite(17, HIGH);
    delay(DELAY_APRESENTACAO);  

    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    digitalWrite(16, LOW);
    digitalWrite(17, LOW);
    delay(DELAY_APRESENTACAO);
  }

}

void apresenta(){


  while(true){

    if(digitalRead(key1)
      ||digitalRead(key2)
      ||digitalRead(key3)
      ||digitalRead(key4)){
      delay(150);
      if(!digitalRead(key1)
        ||!digitalRead(key2)
        ||!digitalRead(key3)
        ||!digitalRead(key4)){
        return;
      }
    }
    delay(DELAY_APRESENTACAO * 2);
    for(int i = 14; i<=17; i++){
      digitalWrite(i, HIGH);
      delay(DELAY_APRESENTACAO);
    } 

    for(int i = 14; i<=17; i++){
      digitalWrite(i, LOW);
      delay(DELAY_APRESENTACAO);
    } 

    delay(DELAY_APRESENTACAO * 2);

    for(int i = 17; i>=14; i--){
      digitalWrite(i, HIGH);
      delay(DELAY_APRESENTACAO);
    } 

    for(int i = 17; i>=14; i--){
      digitalWrite(i, LOW);
      delay(DELAY_APRESENTACAO);
    } 
    delay(DELAY_APRESENTACAO * 2);
  }
}

boolean lerTeclado(){

  int index = 0;
  int jogadaPlayer = 0; 
  boolean jogou = false;

  do{
    do{
      jogou = false;

      while(digitalRead(key1)){
        delay(150);
        digitalWrite(14,HIGH); 
        somOn(14);
        if(!digitalRead(key1)){

          delay(150); 
          digitalWrite(14,LOW); 
          somOff();
          jogadaPlayer = 14;
          jogou = true;

          Serial.print(jogadaPlayer) ; 
          Serial.print("-"); 

        }
      }


      while(digitalRead(key2)){
        delay(150);
        digitalWrite(15,HIGH); 
        somOn(15);
        if(!digitalRead(key2)){

          delay(150); 
          digitalWrite(15,LOW);
          somOff();
          jogadaPlayer = 15;
          jogou = true;

          Serial.print(jogadaPlayer) ; 
          Serial.print("-"); 

        }
      }
      while(digitalRead(key3)){ 
        delay(150);
        digitalWrite(16,HIGH); 
        somOn(16);
        if(!digitalRead(key3)){

          delay(150); 
          digitalWrite(16,LOW);
          somOff();
          jogadaPlayer = 16;
          jogou = true;

          Serial.print(jogadaPlayer) ; 
          Serial.print("-"); 

        }
      }
      while(digitalRead(key4)){
        delay(150);
        digitalWrite(17,HIGH); 
        somOn(17);
        if(!digitalRead(key4)){

          delay(150); 
          digitalWrite(17,LOW);
          somOff();
          jogadaPlayer = 17;
          jogou = true;

          Serial.print(jogadaPlayer) ; 
          Serial.print("-"); 

        }
      }
    }
    while(jogadaPlayer == 0); 
    if(cpu[index] != jogadaPlayer){
      Serial.print(cpu[index]);
      Serial.print(jogadaPlayer);
      return false; 

    }

    jogadaPlayer = 0; 
    index++;
  }
  while(index<=jogadaAtual); 
  jogadaAtual++;
  Serial.print("\n");

  tempoNivel = tempoNivel-10; //torna o jogo mais rapido

  return true; 
}

void escreveLcd(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Pontos: ");
  lcd.setCursor(13,0);
  lcd.print(jogadaAtual);

}
