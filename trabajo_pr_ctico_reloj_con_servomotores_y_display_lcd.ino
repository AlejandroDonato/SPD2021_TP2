// C++ code

#include <LiquidCrystal.h>
#include <Servo.h>
#define BSEG 11
#define BMIN 12
#define BHOR 13
Servo servoSeg;
Servo servoMin;
Servo servoHor;

LiquidCrystal lcd (2,3,4,5,6,7);
unsigned long millisAntes = 0;
int horas =0;
int minutos =0;
int segundos =0;
int botonSegAntes = LOW;
int botonMinAntes = LOW;
int botonHorAntes = LOW;
int estado =0;

void setup(){
  Serial.begin(9600);
  lcd.begin(16,2);
  
  pinMode(BSEG,INPUT);
  pinMode(BMIN,INPUT);
  pinMode(BHOR,INPUT);
  
  servoSeg.attach(8);
  servoMin.attach(9);
  servoHor.attach(10);
  
  lcd.setCursor(5, 0);
  lcd.print("Horario");
  lcd.setCursor(5, 1);
  lcd.print("00:00:00");

}

void loop(){
  
   //funcion del reloj principar
   //determina horas/minutos/segundos
   unsigned long millisAhora = millis();
   if(millisAhora - millisAntes > 1000){
		segundos++;
		millisAntes = millisAhora;
 
    	if(segundos == 60){
    	segundos =0;
    	minutos++;
    	}
    	if(minutos == 60){
    	minutos =0;
    	horas++;
    	}   
    	if(horas == 24){
    	horas =0;
    	}
    	SetHorario(segundos,11);
       	SetHorario(minutos,8);
       	SetHorario(horas,5);
  }
  //funcion para poner y sacar los  : :
  PuntosDivisores(segundos);
  
  int botonSegAhora = digitalRead(BSEG);
  int botonMinAhora = digitalRead(BMIN);
  int botonHorAhora = digitalRead(BHOR);
  
  //para sumar +1 a los segundos
  if(botonSegAhora == HIGH && botonSegAntes == LOW){
    if(segundos == 59){
    segundos=0;
    }
    else{
    segundos++;
    }
    SetHorario(segundos,11);
  }
  botonSegAntes = botonSegAhora;
  
  //para sumar +1 a los minutos
  if(botonMinAhora == HIGH && botonMinAntes == LOW){
    if(minutos == 59){
    minutos=0;
    }
    else{
    minutos++;
    }
    SetHorario(minutos,8);
  }
  botonMinAntes = botonMinAhora;
  
  //para sumar +1 a las horas
  if(botonHorAhora == HIGH && botonHorAntes == LOW){
    if(horas == 23){
    horas=0;
    }
    else{
    horas++;
    }
    SetHorario(horas,5);
  }
  botonHorAntes = botonHorAhora;
  
  //posicionamos los servomotores segun el tiempo
  servoSeg.write(PosicionServo(segundos,60));
  servoMin.write(PosicionServo(minutos,60));
  servoHor.write(PosicionServo(horas,24));
}

//dado el trascurso de los segundo 
//activa y desactiva los 2 puntos que separan hora/min./seg.
//segun si es segundo par o impar
//para agregar un efecto de reloj digital
//contador - recive los segundos
void PuntosDivisores(int contador){
  
  int banderaSeg = DetermianrParidad(contador);
    
  if(banderaSeg == 0){
	lcd.setCursor(7, 1);
	lcd.print(":");
	lcd.setCursor(10, 1);
	lcd.print(":");
  }
  else{
	lcd.setCursor(7, 1);
	lcd.print(" ");
	lcd.setCursor(10, 1);
	lcd.print(" ");
  }
}

/// recive un numero y determina si es par o no
/// unNumero - valor a verificar paridad
/// flagPar - devuelve 0 para un numero par y 1 para impar
int DetermianrParidad(int unNumero){
	int flagPar;
	if(unNumero%2==0){
		flagPar=0;
	}
	else{
		flagPar=1;
	}
	return flagPar;
}

//imprime en la pantalla LCD un numero en una pocicion
//unNumero - valor que recive para mostrar en la pantalla
//pocicion - ubicacion en donde va colocado el numero dado
void SetHorario(int unNumero,int posicion){
  
  lcd.setCursor(posicion, 1);
  
  if (unNumero <10){
  lcd.print("0");
  }
  lcd.print(unNumero);
}

//calcula y devuelve la pocicion en grados para el servomoot
//cantidad - valor a asociar con un grado
//escala - el rango en que varian los numeros
//gradosServo - devuelve el grado para setear el servo
int PosicionServo(int cantidad, int escala){
	int gradosServo = map(cantidad,escala,0,0,180);
  	return gradosServo;
}