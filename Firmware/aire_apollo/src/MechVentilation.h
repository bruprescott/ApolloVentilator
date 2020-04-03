/**
 * Mechanical ventilation.
 *
 * @file MechVentilation.h
 *
 * This is the mechanical ventilation software module.
 * It handles the mechanical ventilation control loop.
 */
#ifndef INC_MECHANICAL_VENTILATION_H
#define INC_MECHANICAL_VENTILATION_H

#include <PID_v1.h>
#include <PID_AutoTune_v0.h>

#include <inttypes.h>
#include "defaults.h"
#include "ApolloHal.h"
#include "ApolloConfiguration.h"

/** States of the mechanical ventilation. */
enum State
{
    Wait = 0, //Espera a que lance el trigger o al tiempo

    InsuflationBefore = 1,
    InsufaltionProcess = 2, //Proceso mientras insufla aire
    InsuflationAfter = 3,

    ExsufflationBefore = 4,
    ExsufflationProcess = 5, //Proceso de exsuflación aire
    ExsufflationAfter = 6,

};
/** Trigger of the proccess mechanical ventilation. */
enum Trigger
{
    Timer = 0,              //Lanzado por tiempo definido
    PattientInpiration = 1, //Lanzado por el paciente al inspirar
};

enum Mode
{
    Presion = 0,
    Flow = 1
};

/**
 * This is the mechanical ventilation class.
 */
class MechVentilation
{
public:
    /**
	 * Constructor
	 *
     * @param[ApolloHal]            hal                       Hal object.
     * @param[ApolloConfiguration]  config                    Configuration Object
     *
	 */
    MechVentilation(
        ApolloHal *hal,
        ApolloConfiguration *configuration);

    /**,
        float secTimeoutInsufflation,
        float secTimeoutExsufflation,
        int ventilationCyle_WaitTime);*/

    /**
     * Update mechanical ventilation.
     *
     * If any control variable were to change, new value
     * would be applied at the beginning of the next ventilation
     * cycle.
     *
     * @note This method must be called on the main loop.
     */
    void update(void);

/*
///Autotune
double kpmodel=1.5, taup=100, theta[50];
double outputStart=5;
double aTuneStep=50, aTuneNoise=1, aTuneStartValue=100;
unsigned int aTuneLookBack=20;

boolean tuning = false;
unsigned long  modelTime, serialTime

    void autoTunePressurePID()
    {

    }

    void DoModel()
    {
      //cycle the dead time
      for(byte i=0 ; i<49 ; i++)
      {
        theta[i] = theta[i+1];
      }
      //compute the input
      input = (kpmodel / taup) *(theta[0]-outputStart) + input*(1-1/taup) + ((float)random(-10,10))/100;

    }

    void DoModel()
    {
      //cycle the dead time
      for(byte i=0;i<49;i++)
      {
        theta[i] = theta[i+1];
      }
      //compute the input
      input = (kpmodel / taup) *(theta[0]-outputStart) + input*(1-1/taup) + ((float)random(-10,10))/100;

    }

    void changeAutoTune()
    {
     if(!tuning)
      {
        //Set the output to the desired starting frequency.
        output=aTuneStartValue;
        aTune.SetNoiseBand(aTuneNoise);
        aTune.SetOutputStep(aTuneStep);
        aTune.SetLookbackSec((int)aTuneLookBack);
        AutoTuneHelper(true);
        tuning = true;
      }
      else
      { //cancel autotune
        aTune.Cancel();
        tuning = false;
        AutoTuneHelper(false);
      }
    }
*/
    void setTargetPressure(float p)
    {
      _targetPressure = p;
    }

////////////////////////////
    void    pidCompute();

    void    setP(float p)
    {
      _consKp = p;
    }

    void    setI(float i)
    {
      _consKi = i;
    }

    void    setD(float d)
    {
      _consKd = d;
    }

private:
    /** Initialization. */
    void _init(
        ApolloHal *hal,
        int mlTidalVolume,
        int rpm,
        int porcentajeInspiratorio);

    /** Set state. */
    void _setState(State state);

    ApolloHal *hal;

    ApolloConfiguration *configuration;


//Javi - testing

    double _targetPressure, _currentPressure, _inputValvePercent;
    //Define the aggressive and conservative Tuning Parameters
    double _aggKp,_aggKi,_aggKd;
    double _consKp,_consKi,_consKd;
    //Specify the links and initial tuning parameters
    PID _pidPressure;

/////


    /** Tidal volume in millilitres. */
    float _cfgmlTidalVolume;
    /** Respiraciones por minuto */
    int _cfgRpm;
    /** Porcentaje tiempo inspiración */
    int _cfgPorcentajeInspiratorio;

    /** Pression trigger value in litres per minute. */
    float _cfgCmh2oTriggerValue;
    /**  Ciclo time in seconds. */
    float _cfgSecCiclo;
    /**  Insufflation time in seconds. */
    float _cfgSecTimeInsufflation;
    /* Exsufflation time in seconds. */
    float _cfgSecTimeExsufflation;
    /* Presion peep (presión mínima en pulmones a la salida). CMH2O */
    float _cfgPresionPeep;
    /* Presión pico de insuflación CMH2O */
    float _cfgPresionPico;
    /* Presión meseta de insuflación CMH2O */
    float _cfgPresionMeseta;
    /* Presión Máxima de seguridad de insuflación CMH2O */
    float _cfgPresionMax;

    /** Current state. */
    State _currentState;
    /** Timer counter in seconds. */
    uint64_t _secTimerCnt;
    /**  Insufflation timeout in seconds. */
    float _secTimeoutInsufflation;
    /** Exsufflation timeout in seconds. */
    float _secTimeoutExsufflation;

    Mode mode = Mode::Presion;

    unsigned long lastExecution = 0;



    void wait();
    void insuflationBefore();
    void insufaltionProcess();
    void insuflationAfter();
    void exsufflationBefore();
    void exsufflationProcess();
    void exsufflationAfter();

    void stateNext();

    void calcularCiclo();

    void configurationUpdate();

};

#endif /* INC_MECHANICAL_VENTILATION_H */
