#include <SoftwareSerial.h> //importiamo la libreria che fa accedere alla classe del softwareSerial

#define rxPin 10 // pin ricezione
#define txPin 8  // pin trasmettitore
// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin); //istanziamo un oggetto di classe SoftwareSerial con i pin di ricetrasmissione

String check4answer(){ //creiamo una funzione che controlla se ci sono richieste
    String str = "";
    while (mySerial.available() > 0) { //finchè i dati da leggere sono maggior di 0
        char c = mySerial.read(); //leggi l'input dell'utente (carattere per carattere)
        Serial.print(c); //stampa l'input dell'utente
        str += String(c); //salviamo il carattere in una variabile str 
    }
    return str; //la funzione restituisce la stringa collezionata 
}


String esp01cmd(String cmd) { // usiamo il monitor seriale dell'arduino collegato all'interfaccia wifi
  Serial.println("sending: " + cmd); 
  mySerial.println(cmd);
  delay(10);//aspetta 10 millisecondi
  String risposta = check4answer(); //salviamo la risposta nella variabile risposta
  return risposta; // la funzione restituisce la risposta
}

void setup()  {
    // Define pin modes for TX and RX
    // pinMode(rxPin, INPUT);
    // pinMode(txPin, OUTPUT);
    
    // Set the baud rate for the SoftwareSerial object
    mySerial.begin(115200); //impostiamo la velocità di trasmissione dell mySerial
    Serial.begin(9600); //impostiamo la velocità di trasmissione della porta seriale dell'arduino
    delay(1000); //aspetta 1 secondo
    esp01cmd("AT"); //chiedi se il dispositivo wifi è attivo
    delay(1000); //aspetta 1 secondo 
    esp01cmd("AT+CWMODE=2"); //impostiamo il dispositivo come station(o client o access point), quindi sarà possibile un collegamento da un terzo
    delay(1000); //aspettiamo un secondo
    esp01cmd("AT+CWSAP=\"robottino\",\"robottino\",1,4"); //impostiamo i dati del esp01 (SSID, PASSWORD)
    delay(1000); // aspetta un secondo
    esp01cmd("AT+CIFSR"); //mostra indirizzo ip dell acces point (AP IP)
    esp01cmd("AT+CIPMUX=1"); //allow up to 1 connections at the time
    
    
    Serial.println("ESP-01 Configuration Completed");
}

void loop() {
    Serial.println("loop..."); //print loop sul monitor seriale
    while(esp01cmd("AT+CWLIF").substring(11,18) != "192.168") { // prendiamo l'ip del client con CWLIF(telefono) e controlliamo se è diverso da 192.168
      Serial.println("no connections so far... still waiting"); //informa che non ci sono collegate
      delay(1000); //aspetta 1 secondo
    }

    Serial.println("Connection from remote device was Established!!!"); //il loop si conclude se e solo se c'è un collegamento
    //Socket ID: 3
    //accept packets from any IP address/devices
    //Listen to local port 4567
    //outgoing packets could go to any remote host without restrictions...
    esp01cmd("AT+CIPSTART=3,\"UDP\",\"0.0.0.0\",0,4567,2"); //starting UDP Socket Server (il socket è il programma che si mette in ascolto su ip e porta del telefono)
    
//    String str = ;    
//    Serial.println("received: "+esp01cmd("AT+CWLIF").substring(11,18));
    delay(3000); //aspetta 3 secondi
    while(true) { //loop infinito
      String str = mySerial.readString(); //leggi input utente collegato all esp01
      if(str != "") { //se la stringa non è vuota
        int startOfSTR = str.indexOf(":",10)+1; //prendi il punto di inizio della stringa
        Serial.println("Received: "+str.substring(startOfSTR)); //stampa la substing con i dati contenuti
        //Serial.println("Received: "+str);
        //Serial.println(startOfSTR);
      }
    }

}
