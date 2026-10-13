#ifndef __FMU_WRAPPER_FMI1_ME__
#define __FMU_WRAPPER_FMI1_ME__

/* Inquire version numbers of header files */
const char *fmiGetModelTypesPlatform();
const char *fmiGetVersion();

fmiStatus fmiSetDebugLogging(fmiComponent c, fmiBoolean loggingOn);

/***************************************************
 Model Exchange
****************************************************/
fmiComponent fmiInstantiateModel(fmiString instanceName,
                                 fmiString GUID,
                                 fmiCallbackFunctions functions,
                                 fmiBoolean loggingOn);
void fmiFreeModelInstance(fmiComponent c);
fmiStatus fmiSetDebugLogging(fmiComponent c, fmiBoolean loggingOn);

/* Providing independent variables and re-initialization of caching */
fmiStatus fmiSetTime(fmiComponent c, fmiReal time);
fmiStatus fmiSetContinuousStates(fmiComponent c, const fmiReal x[], size_t nx);
fmiStatus fmiCompletedIntegratorStep(fmiComponent c, fmiBoolean *OUTPUT);
fmiStatus fmiSetReal(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiReal value[]);
fmiStatus fmiSetInteger(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiInteger value[]);
fmiStatus fmiSetBoolean(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiBoolean value[]);
fmiStatus fmiSetString(fmiComponent c, const fmiValueReference vr[], size_t nvr, const fmiString value[]);

/* Evaluation of the model equations */
fmiStatus fmiInitialize(fmiComponent c, fmiBoolean toleranceControlled,
                        fmiReal relativeTolerance, fmiEventInfo *eventInfo);

fmiStatus fmiGetDerivatives(fmiComponent c, fmiReal OUT[], int OUT_SIZE);
fmiStatus fmiGetEventIndicators(fmiComponent c, fmiReal OUT[], int OUT_SIZE);

fmiStatus fmiGetReal(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiReal OUTPUT[]);
fmiStatus fmiGetInteger(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiInteger OUTPUT[]);
fmiStatus fmiGetBoolean(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiBoolean OUTPUT[]);
fmiStatus fmiGetString(fmiComponent c, const fmiValueReference vr[], size_t nvr, fmiString OUTPUT[]);

fmiStatus fmiEventUpdate(fmiComponent c, fmiBoolean intermediateResults, fmiEventInfo *eventInfo);
fmiStatus fmiGetContinuousStates(fmiComponent c, fmiReal OUT[], int OUT_SIZE);
fmiStatus fmiGetNominalContinuousStates(fmiComponent c, fmiReal OUT[], int OUT_SIZE);
fmiStatus fmiGetStateValueReferences(fmiComponent c, fmiValueReference OUT[], int OUT_SIZE);
fmiStatus fmiTerminate(fmiComponent c);

#endif /* __FMU_WRAPPER_FMI1_ME__ */
