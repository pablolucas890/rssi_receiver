#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <RCSwitch.h>
#include <stdio.h>
#include <EEPROM.h>

// Dados de RF dos enlaces
long enlace_1 = 0;
long enlace_2 = 0;

// Verificação de enlaces configurados
boolean todos_configurados = false;

// Endereços na EEPROM para armazenar os valores dos enlaces
int enderecoEnlace1 = 0;
int enderecoEnlace2 = sizeof(long);

// Reset da EEPROM
const int pinBotao = 4;  // Pino digital 4

RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(9600);
  
  if (ELECHOUSE_cc1101.getCC1101()) {  // Check the CC1101 Spi connection.
    Serial.println("Connection OK");
  } else {
    Serial.println("Connection Error");
  }

  ELECHOUSE_cc1101.Init();          // must be set to initialize the cc1101!
  ELECHOUSE_cc1101.setMHZ(433.92);  // Here you can set your basic frequency. The lib calculates the frequency automatically (default = 433.92).The cc1101 can: 300-348 MHZ, 387-464MHZ and 779-928MHZ. Read More info from datasheet.
  mySwitch.enableReceive(0);        // Receiver on
  ELECHOUSE_cc1101.SetRx();         // set Receive on

  // Lendo os valores dos enlaces da EEPROM
  enlace_1 = ((long)EEPROM.read(enderecoEnlace1) << 24) + ((long)EEPROM.read(enderecoEnlace1 + 1) << 16) + ((long)EEPROM.read(enderecoEnlace1 + 2) << 8) + (long)EEPROM.read(enderecoEnlace1 + 3);
  enlace_2 = ((long)EEPROM.read(enderecoEnlace2) << 24) + ((long)EEPROM.read(enderecoEnlace2 + 1) << 16) + ((long)EEPROM.read(enderecoEnlace2 + 2) << 8) + (long)EEPROM.read(enderecoEnlace2 + 3);


  if (enlace_1 == 0) {
    Serial.println("Enlace 1  não configurado, precione o controle transmissor do primeiro enlace: ");
  } else {
    Serial.println("Enlaces configurados na EEPROM!");
    Serial.print("Enlace 1: ");
    Serial.println(enlace_1);
    Serial.print("Enlace 2: ");
    Serial.println(enlace_2);
    Serial.println("Recebendo dados...");
    todos_configurados = true;
  }
}
void loop() {
  delay(1000);
  if (todos_configurados) {
    if (mySwitch.available()) {
      if (mySwitch.getReceivedValue() == enlace_1) {
        Serial.print("RSSI ENLACE 1: ");
        Serial.println(ELECHOUSE_cc1101.getRssi());
      }
      if (mySwitch.getReceivedValue() == enlace_2) {
        Serial.print("RSSI ENLACE 2: ");
        Serial.println(ELECHOUSE_cc1101.getRssi());
      }
      mySwitch.resetAvailable();
    }
  } else if (enlace_1 == 0) {
    if (mySwitch.available()) {
      enlace_1 = mySwitch.getReceivedValue();


      EEPROM.write(enderecoEnlace1, (enlace_1 >> 24) & 0xFF);
      EEPROM.write(enderecoEnlace1 + 1, (enlace_1 >> 16) & 0xFF);
      EEPROM.write(enderecoEnlace1 + 2, (enlace_1 >> 8) & 0xFF);
      EEPROM.write(enderecoEnlace1 + 3, enlace_1 & 0xFF);

      Serial.print("Enlace 1 Configurado Com: ");
      Serial.println(enlace_1);
      mySwitch.resetAvailable();
      Serial.println("Precione outro controle para configurar o enlace 2:");
    }
  } else if (enlace_2 == 0) {
    if (mySwitch.available()) {
      enlace_2 = mySwitch.getReceivedValue();

      EEPROM.write(enderecoEnlace2, (enlace_2 >> 24) & 0xFF);
      EEPROM.write(enderecoEnlace2 + 1, (enlace_2 >> 16) & 0xFF);
      EEPROM.write(enderecoEnlace2 + 2, (enlace_2 >> 8) & 0xFF);
      EEPROM.write(enderecoEnlace2 + 3, enlace_2 & 0xFF);

      Serial.print("Enlace 2 Configurado Com: ");
      Serial.println(enlace_2);
      mySwitch.resetAvailable();
      Serial.println("Sistema pronto para recebimento de dados..");
      todos_configurados = true;
    }
  }
  if (digitalRead(pinBotao) == HIGH && todos_configurados) {
    // Executa o código para limpar a memória EEPROM
    Serial.println("Limpando dados da EEPROM...");
    EEPROM.write(enderecoEnlace1, 0);
    EEPROM.write(enderecoEnlace1, 0);
    enlace_1 = 0;
    enlace_2 = 0;
    todos_configurados = false;
    Serial.println("Enlace 1  não configurado, precione o controle transmissor do primeiro enlace: ");
  }  
}
