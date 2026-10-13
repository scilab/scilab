#ifndef __FMU_WRAPPER_FMI1_CS__
#define __FMU_WRAPPER_FMI1_CS__

/* Inquire version numbers of header files */
const char *fmiGetTypesPlatform();
const char *fmiGetVersion();

fmiStatus fmiSetDebugLogging(fmiComponent c, fmiBoolean loggingOn);

/* Data Exchange Functions*/
fmiStatus fmiGetReal(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiReal OUTPUT[]);
fmiStatus fmiGetInteger(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiInteger OUTPUT[]);
fmiStatus fmiGetBoolean(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiBoolean OUTPUT[]);
fmiStatus fmiGetString(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiString OUTPUT[]);

fmiStatus fmiSetReal(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiReal value[]);
fmiStatus fmiSetInteger(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiInteger value[]);
fmiStatus fmiSetBoolean(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiBoolean value[]);
fmiStatus fmiSetString(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiString value[]);

/***************************************************
Functions for FMI for Co-Simulation
****************************************************/

/* Creation and destruction of slave instances and setting debug status */
fmiComponent fmiInstantiateSlave(fmiString instanceName,
                                 fmiString fmuGUID,
                                 fmiString fmuLocation,
                                 fmiString mimeType,
                                 fmiReal timeout,
                                 fmiBoolean visible,
                                 fmiBoolean interactive,
                                 fmiCallbackFunctions functions,
                                 fmiBoolean loggingOn);

fmiStatus fmiInitializeSlave(fmiComponent c,
                             fmiReal tStart,
                             fmiBoolean StopTimeDefined,
                             fmiReal tStop);

fmiStatus fmiTerminateSlave(fmiComponent c);
fmiStatus fmiResetSlave(fmiComponent c);
void fmiFreeSlaveInstance(fmiComponent c);

fmiStatus fmiSetRealInputDerivatives(fmiComponent c,
                                     const fmiValueReference vr[],
                                     size_t nvr,
                                     const fmiInteger order[],
                                     const fmiReal value[]);

fmiStatus fmiGetRealOutputDerivatives(fmiComponent c,
                                      const fmiValueReference vr[],
                                      size_t nvr,
                                      const fmiInteger order[],
                                      fmiReal OUTPUT[]);

fmiStatus fmiCancelStep(fmiComponent c);
fmiStatus fmiDoStep(fmiComponent c,
                    fmiReal currentCommunicationPoint,
                    fmiReal communicationStepSize,
                    fmiBoolean newStep);

fmiStatus fmiGetStatus(fmiComponent c, const fmiStatusKind s, fmiStatus *OUTPUT);
fmiStatus fmiGetRealStatus(fmiComponent c, const fmiStatusKind s, fmiReal *OUTPUT);
fmiStatus fmiGetIntegerStatus(fmiComponent c, const fmiStatusKind s, fmiInteger *OUTPUT);
fmiStatus fmiGetBooleanStatus(fmiComponent c, const fmiStatusKind s, fmiBoolean *OUTPUT);
fmiStatus fmiGetStringStatus(fmiComponent c, const fmiStatusKind s, fmiString *OUTPUT);

#endif /* __FMU_WRAPPER_FMI1_CS__ */
