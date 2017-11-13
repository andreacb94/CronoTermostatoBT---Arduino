/** CREATED BY ANDREA PETRELLA.
 *  DATE: 12/11/2017
 */

/*** Progetto per usare Arduino come crono-termostato per caldaia con interfaccia Bluetooth per dispositivi mobile ***/

//Definisco le costanti
#define pin_boiler 3
#define pin_sensorTemperature A0

//Variabili temporali
unsigned long now = 0;
unsigned long old_time = 0;
// Frangente di tempo da attendere prima di una nuovo controllo
unsigned long delta_time = (30 * 60 * 1000); //30 minuti

//Calendario base
int YY = 2017;
int MM = 11;
int DD = 14;
int hh = 00;
int mm = 00;
int ss = 00;

//Variabili per la temperatura
int temp_start = 18;
int temp_limit = 10;
int temp_stop = 21;

//Variabile di comunicazione Bluetooth
char data_bt = '0';

//Variabile di stato
// false = non configurato
// true = configurato
boolean state = false;

String convert_data = "";

int buffer_cont = 38;

void setup() {

  //Inizializzo la porta seriale per la comunicazione Bluetooth
  Serial.begin(9600);

  //Inizializzo i pin
  pinMode(pin_boiler, OUTPUT);
  pinMode(pin_sensorTemperature, INPUT);

  //Setto a 0 lo stato di partenza delle uscite
  digitalWrite(pin_boiler, 0);

  //Attendo 2 secondi dall'accensione prima di iniziare il programma
  delay(2000);

}

void loop() {

  //Controllo se lo stato non è configurato
  if(!state){

    //Controllo le è presente ancora qualcosa nel buffer seriale
    if(Serial.available() > 0){

      //Decremento il buffer
      buffer_cont--;

      //Leggo il dato in arrivo dal bt e lo salvo in data
      data_bt = Serial.read();
      
      //Serial.println("data_bt: " + data_bt);
      
      convert_data += data_bt;

    }

    //Una volta esaurito il buffer salvo i dati
    if(cont<= 0){
      
      //Codifico l'input seriale
      //00000000001111111111222222222233333333
      //01234567890123456789012345678901234567
      //{temp:18;10;21/DD:2017;11;14;00;00;00}

      //Salvo i dati acquisendo le substrings
      temp_start = convert_data.substring(6,8).toInt();
      temp_limit = convert_data.substring(9,12).toInt();
      temp_stop = convert_data.substring(12,14).toInt();
      YY = convert_data.substring(18,22).toInt();
      MM = convert_data.substring(23,25).toInt();
      DD = convert_data.substring(26,28).toInt();
      hh = convert_data.substring(29,31).toInt();
      mm = convert_data.substring(32,34).toInt();
      ss = convert_data.substring(35,37).toInt();

      //Setto lo stato nella condizione di configurato
      state = true;

    }
    
  }

  //Se lo stato è configurato, avvio il programma principale
  else{
    
    now = millis();
  
    if( now >= (old_time + delta_time)){
  
      crono();
  
      old_time = now;
      
    }
    
  }

}

//Funzione per gestire il cronometro
void crono(){

  //Orario massimo e orario minimo
  unsigned long min_time = 0;
  unsigned long max_time = 1000 * 60 * 60 * 24;

  if(min_time <= now){

    if(now <= max_time){

      temperature(temp_start, temp_stop);
      
    }

    else{

      temperature(temp_limit, temp_start);
      
    }
    
  }

  else{

    temperature(temp_limit, temp_start);

  }
  
}

//Funzione per gestire il termostato
void temperature(int temp_min, int temp_max){

  //Acquisisco la temperatura dal sensore
  int temp = analogRead(pin_sensorTemperature);

  //Se è minure del minimo accendo il riscaldamento
  if(temp < temp_min){

    digitalWrite(pin_boiler, 1);
    
  }
  
  else{

    //Se è maggiore o uguale al massimo spengo il riscaldamento
    if(temp >= temp_max){

      digitalWrite(pin_boiler, 0);
    }
    
  }

}

