/*$************** SCADE FMU wrapper *******************
** Begin of file CruiseControl__CruiseControl_FMU.c
*****************************************************$*/

#include "CruiseControl_macro.h"

/* declare sensors as extern variables */
DECLARE_EXT_SENSORS_CruiseControl(extern )

/*$************ MODEL DEFINITIONS *************$*/

/* define class name and unique id */
#define MODEL_IDENTIFIER CruiseControl__CruiseControl_FMU
#define MODEL_GUID "{8c4e810f-3df3-4a00-8276-176fa3c9f004}"
#define WHITEBOX "0"

#define TASK_PERIOD 0.1 /* Task period setting */

/* define model size */
#define NUMBER_OF_REALS 5
#define NUMBER_OF_INTEGERS 1
#define NUMBER_OF_BOOLEANS 6
#define NUMBER_OF_CHARS 0
#define NUMBER_OF_STATES 0
#define STATES {0}
#define NUMBER_OF_EVENT_INDICATORS 0

#ifndef WU_CTX_TYPE_CruiseControl_SIZE
#define CONTEXT_SIZE 1
#else
#define CONTEXT_SIZE WU_CTX_TYPE_CruiseControl_SIZE
#endif
/* include fmu header files, typedefs and macros */
#include "fmuTemplate.h"

/* define all model variables and their value references
   conventions used here:
   - if x is a variable, then macro x_ is its variable reference
   - the vr of a variable is its index in array  r, i, b or s
   - if k is the vr of a real state, then k+1 is the vr of its derivative */
#define On_ 0
#define Off_ 1
#define Resume_ 2
#define Set_ 3
#define QuickAccel_ 4
#define QuickDecel_ 5
#define Accel_ 0
#define Brake_ 1
#define Speed_ 2
#define CruiseSpeed_ 3
#define ThrottleCmd_ 4
#define CruiseState_ 0



/*$************* FMU TASK ***************************$*/
/* called by fmiInstantiateModel
   Set values for all variables that define a start value
   Settings used unless changed by fmiSetX before fmiInitialize */
void setStartValues(ModelInstance *comp) {
}

/* called by fmiInitialize() after setting eventInfo to defaults
   Used to set the first time event, if any. */
void initialize(ModelInstance* comp, fmiEventInfo* eventInfo) {



  /* Initialize SCADE contexts */
  WU_CTX_TYPE_CruiseControl* ctx = (WU_CTX_TYPE_CruiseControl*)comp->context;

  /* Sensors */

  /* Inputs */
  b(On_) = &(VARC_CruiseControl_On((*ctx)));
  b(Off_) = &(VARC_CruiseControl_Off((*ctx)));
  b(Resume_) = &(VARC_CruiseControl_Resume((*ctx)));
  b(Set_) = &(VARC_CruiseControl_Set((*ctx)));
  b(QuickAccel_) = &(VARC_CruiseControl_QuickAccel((*ctx)));
  b(QuickDecel_) = &(VARC_CruiseControl_QuickDecel((*ctx)));
  r(Accel_) = &(VARC_CruiseControl_Accel((*ctx)));
  r(Brake_) = &(VARC_CruiseControl_Brake((*ctx)));
  r(Speed_) = &(VARC_CruiseControl_Speed((*ctx)));

  /* Outputs */
  r(CruiseSpeed_) = &(VARC_CruiseControl_CruiseSpeed((*ctx)));
  r(ThrottleCmd_) = &(VARC_CruiseControl_ThrottleCmd((*ctx)));
  i(CruiseState_) = &(VARC_CruiseControl_CruiseState((*ctx)));

  INITC_CruiseControl((*ctx));

  /* Set delay for next event */
  eventInfo->upcomingTimeEvent   = fmiTrue;
  eventInfo->nextEventTime       = TASK_PERIOD + comp->time;
}

/* called by fmiGetReal, fmiGetContinuousStates and fmiGetDerivatives */
fmiReal getReal(ModelInstance* comp, fmiValueReference vr) {
  return *(kcg_real*)r(vr);
}

/* called by fmiEventUpdate() after setting eventInfo to defaults
   Used to set the next time event, if any. */
void eventUpdate(ModelInstance* comp, fmiEventInfo* eventInfo) {

  /* Executing SCADE cycle */
  PERFORMC_CruiseControl((*(WU_CTX_TYPE_CruiseControl*)comp->context));

  /* Set delay for next cycle... */
  eventInfo->upcomingTimeEvent   = fmiTrue;
  eventInfo->nextEventTime       = TASK_PERIOD + comp->time;

}

/* include code that implements the FMI based on the above definitions */
#include "fmuTemplate.ci"



/*$************** SCADE FMU wrapper *******************
** End of file CruiseControl__CruiseControl_FMU.c
*******************************************************$*/
