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

//Variabili per la temperatura
int temp_start = 18;
int temp_limit = 10;
int temp_stop = 22;

void setup() {

  //Inizializzo i pin
  pinMode(pin_boiler, OUTPUT);
  pinMode(pin_sensorTemperature, INPUT);

  //Setto a 0 lo stato di partenza delle uscite
  digitalWrite(pin_boiler, 0);

  //Attendo 2 secondi dall'accensione prima di iniziare il programma
  delay(2000);

}

void loop() {

  now = millis();

  if( now >= (old_time + delta_time)){

    crono();

    old_time = now;
    
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

