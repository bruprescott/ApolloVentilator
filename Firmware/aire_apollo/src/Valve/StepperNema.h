#ifndef StepperNema_H
#define  StepperNema_H

#include <Arduino.h>
#include "../ApolloValve.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 900
// Microstepping mode. If you hardwired it to save pins, set to the same value here.
#define MICROSTEPS 16

//#include "A4988.h"
// #include "DRV8825.h"
// #include "DRV8880.h"
#include "BasicStepperDriver.h" // generic
// BasicStepperDriver stepper(DIR, STEP);

class StepperNema : public ApolloValve
{

public:

  StepperNema(uint8_t pinEna_,uint8_t pinDir_,uint8_t pinPul_,uint8_t pinFcIni_ = 0,uint8_t pinFcEnd_ = 0);
  bool    begin();
  void    open(double percent = 100);
  void    close();
  double  status() {return this->percent;}
  void    update(); //Required to move de steps
  BasicStepperDriver stepper;


protected:

  // Numero de pasos de apertura máxima del motor
  // A mayor apertura necesaria, hay que aumentar la velocidad bajando los microsteps
  // Configurar con los pasos necesarios para llegar al final del elemento que se quiera presionar, pulsar o rotar
  int stepsMax = MOTOR_STEPS*MICROSTEPS;

  uint8_t pinEna;
  uint8_t pinDir;
  uint8_t pinPul;
  uint8_t stepEnd;
  uint8_t pinFcIni = 0;
  uint8_t pinFcEnd = 0;
  double percent = 0;

  bool lastDir;
  int lastStep;
  int stepDestination;

};

#endif