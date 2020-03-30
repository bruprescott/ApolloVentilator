#include "MksmValve.h"
#include <Arduino.h>
#include "trace.h"

MksmValve::MksmValve(uint8_t pin, uint16_t openDelay, uint16_t closeDelay, bool invertedLogic) :
_pin(pin),_trueState(!invertedLogic),
_openDelayMS(openDelay/2),_closeDelayMS(closeDelay/2) {;}

bool MksmValve::begin()
{
  pinMode(_pin, OUTPUT);
  return true;
}

void MksmValve::open(uint8_t percent)
{
  if(percent > 100) percent = 100;
  if(percent == 0)  percent = 1; //Hack!
  if(_openPercent == 0)//Si estabamos cerrados primero cargamos la bobina de la valvula
  {
      magnetize();
      delay(_openDelayMS);
  }

  _openPercent = percent;
  float factor = 100.0/float(101-percent);
  _deMagnetizedTimeMS   = _closeDelayMS       * factor;
  _magnetizedTimeMS     = _deMagnetizedTimeMS * (_openDelayMS/float(_closeDelayMS));
  _waitTimeMS           = (_closeDelayMS + _openDelayMS) / (101.0-percent/100.0);// / (factor/4.0);

  if(percent == 100)
  {
    _deMagnetizedTimeMS =   0;
    _magnetizedTimeMS   = 20;
  }
  if(percent == 0)
  {
    _deMagnetizedTimeMS = 20;
    _magnetizedTimeMS   =   0;
  }

  _cycleTimeMS = _magnetizedTimeMS + _deMagnetizedTimeMS + _waitTimeMS;
  //Serial.println("factor: " + String(factor) + " oTime: " + String(_magnetizedTimeMS) + " cTime:" + String(_deMagnetizedTimeMS) + " wTime: " + " cyclems:" + String(_cycleTimeMS) );
}

void MksmValve::close()
{
  _openPercent = 0;
  demagnetize();
  delay(_closeDelayMS);
}

void MksmValve::update()
{
  if(!isOpen()) return;

  uint16_t timeReference = millis()%_cycleTimeMS; //Hallamos en que momento del ciclo de la valvula estamos

  if      (isMagnetized() && ( timeReference < _deMagnetizedTimeMS ) ) //Si la valvula esta abierta y ya ha pasado el tiempo de estar abierta la cerramos
          demagnetize();
  else if (!isMagnetized() && (timeReference > _deMagnetizedTimeMS) ) // Si no está abierta y estamos dentro del tiempo de estar abierta la abrimos
          magnetize();
}
