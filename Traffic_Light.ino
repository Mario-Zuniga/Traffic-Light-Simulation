// Definiciones para posiciónes de coches
#define NO_COCHE 0
#define COCHE 1

int sensor_1 = A0;       // Sensor 1
int sensor_2 = A1;       // Sensor 2
int sensor_3 = A2;       // Sensor 3


int ledPin_1 = 11;       // Led carril 1
int ledPin_2 = 12;       // Led carril 2
int ledPin_3 = 13;       // Led carril 3

short Vel_A0;            // Contador de lecturas donde hay coche por segundo 
short VelF_A0;           // Lecturas donde hay coche por segundo 
short ESTADO_A0;         // Estado si hay COCHE_NOCOCHE 

short Vel_A1;            // Contador de lecturas donde hay coche por segundo 
short VelF_A1;           // Lecturas donde hay coche por segundo 
short ESTADO_A1;         // Estado si hay COCHE_NOCOCHE 

short Vel_A2;            // Contador de lecturas donde hay coche por segundo 
short VelF_A2;           // Lecturas donde hay coche por segundo 
short ESTADO_A2;         // Estado si hay COCHE_NOCOCHE 



float Velocidad_A0;      // Velocidad del auto 1
float Velocidad_A1;      // Velocidad del auto 2
float Velocidad_A2;      // Velocidad del auto 3
    
long time;               // Variable tiempo

int carril_1 = 1;
int carril_2 = 2;
int carril_3 = 3;

long timeLed_1;
long timeLed_2;
long timeLed_3;

//////////////////////////////////////////////////////
/* Variables */
const int greenLedPin =  8;            /* Pin LED verde */
const int yellowLedPin =  9;           /* Pin LED amarillo */
const int redLedPin =  10;             /* Pin LED rojo */
 
int ledState = LOW;                   /* Estado del LED */

long previousMillisGreen = 0;         /* Variable que indica la última vez que se activo el LED verde */
long previousMillisYellow = 0;        /* Variable que indica la última vez que se activo el LED amarillo */
long previousMillisRed = 0;           /* Variable que indica la última vez que se activo el LED rojo */

/* Valor para el estado del semaforo */
int semaphore = 1;
 
/* Variables que indican el tiempo que le tomara a los LEDs parpadear */
long interval1 = 6000;           /* 6 segundos */
long interval2 = 2000;           /* 2 segundos */
long interval3 = 7000;           /* 7 segundos */
//////////////////////////////////////////////////////

void setup() {
 pinMode(sensor_1,INPUT); //Activamos el ADC
 pinMode(ledPin_1, OUTPUT);      // sets the digital pin as output
 pinMode(ledPin_2, OUTPUT);      // sets the digital pin as output
 pinMode(ledPin_3, OUTPUT);      // sets the digital pin as output
   
 Serial.begin(115200);    //Configuramos Serial

 //Inicialización de Variables
 Vel_A0=0;
 VelF_A0=0;
 ESTADO_A0=0;
 Velocidad_A0=0;
 
 Vel_A1=0;
 VelF_A1=0;
 ESTADO_A1=0;
 Velocidad_A1=0;

 Vel_A2=0;
 VelF_A2=0;
 ESTADO_A2=0;
 Velocidad_A2=0;

 timeLed_1=0;
 timeLed_2=0;
 timeLed_3=0;

//////////////////////////////////////////////////////////////////
   /* Declaramos los pines de LEDs como Outputs */
  pinMode(greenLedPin, OUTPUT);
  pinMode(yellowLedPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);  

  /* El LED verde comenzará ensendido automáticamente */
  digitalWrite(greenLedPin, HIGH);
 //////////////////////////////////////////////////////////////////
 
}

void loop() {
  time=millis(); //Iniciamos nuestro contador en Millis
  //Serial.println(time);

 Sem_func();
 ADC_func(sensor_1, 330 , &(ESTADO_A0), &(VelF_A0), &(Vel_A0), &(Velocidad_A0), carril_1,&(timeLed_1));
 ADC_func(sensor_2, 355 , &(ESTADO_A1), &(VelF_A1), &(Vel_A1), &(Velocidad_A1), carril_2,&(timeLed_2));
 ADC_func(sensor_3, 350 , &(ESTADO_A2), &(VelF_A2), &(Vel_A2), &(Velocidad_A2), carril_3,&(timeLed_3));


}


void ADC_func(int sensor_x, int flag_sense, short *(ESTADO), short *(VelF), short *(Vel), float *(Velocidad), int carril, long *timeLed){

  int read_Value=0;  //Valor de la lectura 
  long read_ValueProm=0;  //Contador de lectura sumada 
  long read_ValuePromF=0; //Variable de lectura promedio 
  float T=0;              //Tiempo de auto en el sensor 
  short CAR=0;            //Variable que defina si hay un auto en la lectura promedio 
 
  

    //Hacemos una lectura cada 10uS y hacemos un promedio de 100 para tener una muestra cada 1mS
    for (int i=0; i <33; i++){
        read_Value=analogRead(sensor_x);
        read_ValueProm=read_ValueProm+read_Value;
        delayMicroseconds(10);
     } 
    
     read_ValuePromF=read_ValueProm/33;  //Valor de lectura promediada para mayor presición

    //Definimos si hay coche o no dependiendo de nuestra bandera
     if(read_ValuePromF>=flag_sense){
        CAR=1;
     }
     else
     {
        CAR=0;
     }

   
   // Serial.println(read_ValuePromF);

   //Maquina de estados
   switch(*ESTADO){
    case NO_COCHE:  //Si no hay coche checamos cuando se encuentra un 1 en el sensor y pasamos al estado Coche 
    {
      if(CAR==1){

        if(semaphore==3){
          *Velocidad=40;
          *ESTADO=COCHE; 
          *VelF=0; 
          
          
        }
        else{
        *ESTADO=COCHE; 
        *VelF=0;   
        }
      }
      else{
        *Vel=0;
        *Velocidad=0;
      }
      break;
     
    }
    case COCHE:{  //Si hay coche checamos cuando se encuentre un 0 en el sensor y pasamos al estado NO_COCHE

        
        
      if (CAR==0){

          if (semaphore==3){
        *ESTADO=NO_COCHE;
        Serial.print("Multa por Semáforo en carril ");
        Serial.println(carril); 
        
        }

        else{
          
        
        *ESTADO=NO_COCHE;
        *VelF=*Vel;   //Capturamos las muestras sumadas
        T=float(*VelF)/(83); //Las dividimos entre 83 que es nuestro periodo de muestreo
        *Velocidad=float((0.15)*(3.6))/T;  //Convertimos a km/h los 0.15m que mide nuestro auto
        *Vel=0;

    

   Serial.print("VelF: ");
   Serial.println(*VelF);
   Serial.print("Velocidad: ");
   Serial.println(*Velocidad);

   if(*Velocidad>7){
   Serial.print("Multa por exceso de Vel. en carril ");
   Serial.println(carril); 
   }
   } 
      }
      else{ //Si hay coche contamos cuantas veces se encuentra el coche en el sensor
        *Vel=*Vel+1;
      }
    
      break;     
    }
      default: 
      break;
   }

  //Imprimimos nuestros valores Finales
  
  
  if(*Velocidad>7){

    switch(carril){
    case 1:  //prende led 1
     {
     digitalWrite(ledPin_1, HIGH);   // sets the LED on
     if(*timeLed==0){
     *timeLed=time;
     *Velocidad=0;
     }
     else{
      
     }
     break;
      }
     case 2:  //prende led 2
     {
     digitalWrite(ledPin_2, HIGH);   // sets the LED on
     if(*timeLed==0){
     *timeLed=time;
     *Velocidad=0;
     }
     else{
      
     }
     break;
      }

      case 3:  //prende led 3
     {
     digitalWrite(ledPin_3, HIGH);   // sets the LED on
     if(*timeLed==0){
     *timeLed=time;
     *Velocidad=0;
     }
     else{
      
     }
     break;
      }
      default: 
      break;
      
    }    
  }

  else{

    switch(carril){
    case 1:  //prende led 1
     {
     if(time-*timeLed>1000){
     digitalWrite(ledPin_1, LOW);   // sets the LED on
     *timeLed=0;
     }
     break;
      }
      case 2:  //prende led 2
     {
     if(time-*timeLed>1000){
     digitalWrite(ledPin_2, LOW);   // sets the LED on
     *timeLed=0;
     }
     break;
      }
      case 3:  //prende led 3
     {
      if(time-*timeLed>1000){
     digitalWrite(ledPin_3, LOW);   // sets the LED on
     *timeLed=0;
      }
     break;
      }
      default: 
      break;
      
    }   
  }
}

//////////////////////////////////////////////////////////////////
void Sem_func(){

  /* Obtenemos el valor en tiempo desde que el código comenzo
     a correr */
  unsigned long currentMillis1 = millis();
  unsigned long currentMillis2 = millis();
  unsigned long currentMillis3 = millis();

  /* Revisamos si el semáforo esta en estado 1 (verde) */
  if(semaphore == 1) {

    /* Verificamos si el código a corrido 6 segundos */
    if(currentMillis1 - previousMillisGreen > interval1) {
      /* Guarda el tiempo donde parpadeo por última vez */
      previousMillisGreen = currentMillis1;   
   
      /* Verificamos el estado del LED */
      if (ledState == LOW)
        ledState = HIGH;
      else {
        ledState = LOW;
        /* Cambiamos a la segunda etapa del semaforo (amarillo) */
        semaphore = 2;
      }
      /* Ajustamos el pin del LED con el estado */
      digitalWrite(greenLedPin, ledState);
    }
  }
    /* Revisamos si el semáforo esta en estado 2 (amarillo) */
    if(semaphore == 2) {

      /* Verificamos si el código a corrido 2 segundos */
      if(currentMillis2 - previousMillisYellow > interval2) {
        /* Guarda el tiempo donde parpadeo por última vez */
        previousMillisYellow = currentMillis2;   
     
        /* Verificamos el estado del LED */
        if (ledState == LOW)
          ledState = HIGH;
        else {
          ledState = LOW;
          /* Cambiamos a la segunda etapa del semaforo (rojo) */
          semaphore = 3;
        }
        /* Ajustamos el pin del LED con el estado */
        digitalWrite(yellowLedPin, ledState);
      }
  }
    /* Revisamos si el semáforo esta en estado 3 (rojo) */
    if(semaphore == 3) {

      /* Verificamos si el código a corrido 7 segundos */
      if(currentMillis3 - previousMillisRed > interval3) {
         /* Guarda el tiempo donde parpadeo por última vez */
        previousMillisRed = currentMillis3;   
     
        /* Verificamos el estado del LED */
        if (ledState == LOW)
          ledState = HIGH;
        else {
          ledState = LOW;
          /* Cambiamos a la segunda etapa del semaforo (verde) */
          semaphore = 1;
        }
        /* Ajustamos el pin del LED con el estado */
        digitalWrite(redLedPin, ledState);
      }
  }
  
}
