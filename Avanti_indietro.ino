// C++ code
//
#define In1 2 //Pin D2 dell'Arduino e' collegato al Input 1 del ponte H
#define In2 4 //Pin D4 dell'Arduino e' collegato al Input 2 del ponte H
#define In3 6 //Pin D6 dell'Arduino e' collegato al Input 3 del ponte H
#define In4 7 //Pin D7 dell'Arduino e' collegato al Input 4 del ponte H
#define EnM1 5 //M1 motore di sinistra
#define EnM2 9 //M2 motore destro
#define trig 11
#define echo 12



void Msinistra(String direzione){
  if(direzione == "avanti"){
    digitalWrite(In1, HIGH); //M1 avanti
    digitalWrite(In2, LOW);
  }else if(direzione == "indietro") {
    digitalWrite(In1, LOW);  //M1 indietro
     digitalWrite(In2, HIGH);}
   else {
    digitalWrite(In1, LOW);
    digitalWrite(In2, LOW);}
}

void Mdestra(String direzione){
  if(direzione == "avanti"){
    digitalWrite(In3, HIGH); //M2 avanti
    digitalWrite(In4, LOW);
  } else if(direzione == "indietro") {
      digitalWrite(In3, LOW); //M2 indietro
      digitalWrite(In4, HIGH);

  }else{
    digitalWrite(In3, LOW);
    digitalWrite(In4, LOW);
  }
}

void robot(String azione){
  if(azione == "gira in senso orario"){
   Msinistra("avanti");
   Mdestra("indietro");
  }else if (azione == "gira in senso antiorario"){
   Msinistra("indietro");
      Mdestra("avanti");
  }else if (azione == "vai avanti"){
   Msinistra("avanti");
      Mdestra("avanti");
  }else if (azione == "vai indietro"){
   Msinistra("indietro");
      Mdestra("indietro");
  }else if (azione == "fermati"){
   Msinistra("stop");
      Mdestra("stop");
  }
}


void setup(){
 
  pinMode(2, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(EnM1, HIGH);
  digitalWrite(EnM2, HIGH);
 
 
  Serial.begin(9600);
  pinMode(trig,OUTPUT);
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  pinMode(echo,INPUT);

  Serial.println("Distanza:");
 
}

int terra_sensore = usaSensore();

bool riconosciSePrecipizio(int a){
  if (terra_sensore-a>2 || terra_sensore-a>-2) {
  	return true;
  }else{
  	return false;
  }
}


int usaSensore(){
  int duration;
  float distance;
  float meter;
  digitalWrite(trig, HIGH);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
   distance = duration/58;
   meter=distance/100;
   return distance;
 }




void loop(){

  robot("vai avanti");
  
  if(riconosciSePrecipizio(usaSensore())){
    robot("vai indietro");
    delay(5000);
    robot("gira in senso orario");
    delay(500);
  };
 
}
