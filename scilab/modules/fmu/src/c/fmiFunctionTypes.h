#ifndef fmiFunctionTypes_h
#define fmiFunctionTypes_h

#include "fmiPlatformTypes.h"

/*
This header file defines the data and function types of FMI 1.0.
It must be used when compiling an FMU or an FMI importer.

Copyright (C) 2011 MODELISAR consortium,
              2012-2022 Modelica Association Project "FMI"
              All rights reserved.

This file is licensed by the copyright holders under the 2-Clause BSD License
(https://opensource.org/licenses/BSD-2-Clause):

----------------------------------------------------------------------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
----------------------------------------------------------------------------
*/

#ifdef __cplusplus
extern "C" {
#endif

/* Include stddef.h, in order that size_t etc. is defined */
#include <stddef.h>


/* Type definitions */

/* tag::Status[] */
typedef enum {
    fmiOK,
    fmiWarning,
    fmiDiscard,
    fmiError,
    fmiFatal,
    fmiPending,
} fmiStatus;
/* end::Status[] */

typedef enum {fmiDoStepStatus,
              fmiPendingStatus,
              fmiLastSuccessfulTime
             } fmiStatusKind;

typedef void  (*fmiCallbackLogger) (fmiComponent c, fmiString instanceName, fmiStatus status,
                                    fmiString category, fmiString message, ...);
typedef void* (*fmiCallbackAllocateMemory)(size_t nobj, size_t size);
typedef void  (*fmiCallbackFreeMemory)    (void* obj);
typedef void  (*fmiStepFinished)          (fmiComponent c, fmiStatus status);

typedef struct
{
    fmiCallbackLogger         logger;
    fmiCallbackAllocateMemory allocateMemory;
    fmiCallbackFreeMemory     freeMemory;
    fmiStepFinished           stepFinished;
} fmiCallbackFunctions;

typedef struct
{
    fmiBoolean iterationConverged;
    fmiBoolean stateValueReferencesChanged;
    fmiBoolean stateValuesChanged;
    fmiBoolean terminateSimulation;
    fmiBoolean upcomingTimeEvent;
    fmiReal    nextEventTime;
} fmiEventInfo;


/* Define fmi function pointer types to simplify dynamic loading */

/***************************************************
Types for Common Functions
****************************************************/

/* Inquire version numbers of header files and setting logging status */
typedef const char* fmiGetModelTypesPlatformTYPE(void);
typedef const char* fmiGetTypesPlatformTYPE(void);
typedef const char* fmiGetVersionTYPE(void);
typedef fmiStatus  fmiSetDebugLoggingTYPE(fmiComponent, fmiBoolean);

/* Getting and setting variable values */
typedef fmiStatus fmiGetRealTYPE   (fmiComponent, const fmiValueReference[], size_t, fmiReal   []);
typedef fmiStatus fmiGetIntegerTYPE(fmiComponent, const fmiValueReference[], size_t, fmiInteger[]);
typedef fmiStatus fmiGetBooleanTYPE(fmiComponent, const fmiValueReference[], size_t, fmiBoolean[]);
typedef fmiStatus fmiGetStringTYPE (fmiComponent, const fmiValueReference[], size_t, fmiString []);

typedef fmiStatus fmiSetRealTYPE   (fmiComponent, const fmiValueReference[], size_t, const fmiReal   []);
typedef fmiStatus fmiSetIntegerTYPE(fmiComponent, const fmiValueReference[], size_t, const fmiInteger[]);
typedef fmiStatus fmiSetBooleanTYPE(fmiComponent, const fmiValueReference[], size_t, const fmiBoolean[]);
typedef fmiStatus fmiSetStringTYPE (fmiComponent, const fmiValueReference[], size_t, const fmiString []);


/***************************************************
Types for Functions for FMI for Model Exchange
****************************************************/

/* Creation and destruction of FMU instances and setting debug status */
typedef fmiComponent fmiInstantiateModelTYPE (fmiString, fmiString, fmiCallbackFunctions, fmiBoolean);
typedef void         fmiFreeModelInstanceTYPE(fmiComponent);

/* Enter and exit the different modes */
typedef fmiStatus fmiInitializeTYPE(fmiComponent c, fmiBoolean toleranceControlled,
                                     fmiReal relativeTolerance, fmiEventInfo* eventInfo);
typedef fmiStatus fmiCompletedIntegratorStepTYPE(fmiComponent, fmiBoolean*);
typedef fmiStatus fmiTerminateTYPE(fmiComponent);

/* Providing independent variables and re-initialization of caching */
typedef fmiStatus fmiSetTimeTYPE            (fmiComponent, fmiReal);
typedef fmiStatus fmiSetContinuousStatesTYPE(fmiComponent, const fmiReal[], size_t);

/* Evaluation of the model equations */
typedef fmiStatus fmiGetDerivativesTYPE            (fmiComponent, fmiReal[], size_t);
typedef fmiStatus fmiGetEventIndicatorsTYPE        (fmiComponent, fmiReal[], size_t);
typedef fmiStatus fmiEventUpdateTYPE               (fmiComponent c, fmiBoolean intermediateResults, fmiEventInfo* eventInfo);
typedef fmiStatus fmiGetContinuousStatesTYPE       (fmiComponent, fmiReal[], size_t);
typedef fmiStatus fmiGetNominalContinuousStatesTYPE(fmiComponent, fmiReal[], size_t);
typedef fmiStatus fmiGetStateValueReferencesTYPE   (fmiComponent, fmiValueReference[], size_t);


/***************************************************
Types for Functions for FMI for Co-Simulation
****************************************************/

/* Creation and destruction of FMU instances and setting debug status */
typedef fmiComponent fmiInstantiateSlaveTYPE (fmiString, fmiString, fmiString, fmiString, fmiReal, fmiBoolean, fmiBoolean, fmiCallbackFunctions, fmiBoolean);
typedef void         fmiFreeSlaveInstanceTYPE(fmiComponent);

/* Enter and exit initialization mode, terminate and reset */
typedef fmiStatus fmiInitializeSlaveTYPE     (fmiComponent, fmiReal, fmiBoolean, fmiReal);
typedef fmiStatus fmiTerminateSlaveTYPE      (fmiComponent);
typedef fmiStatus fmiResetSlaveTYPE          (fmiComponent);

/* Simulating the slave */
typedef fmiStatus fmiSetRealInputDerivativesTYPE (fmiComponent, const fmiValueReference [], size_t, const fmiInteger [], const fmiReal []);
typedef fmiStatus fmiGetRealOutputDerivativesTYPE(fmiComponent, const fmiValueReference [], size_t, const fmiInteger [], fmiReal []);

typedef fmiStatus fmiDoStepTYPE     (fmiComponent, fmiReal, fmiReal, fmiBoolean);
typedef fmiStatus fmiCancelStepTYPE (fmiComponent);

/* Inquire slave status */
typedef fmiStatus fmiGetStatusTYPE       (fmiComponent, const fmiStatusKind, fmiStatus* );
typedef fmiStatus fmiGetRealStatusTYPE   (fmiComponent, const fmiStatusKind, fmiReal*   );
typedef fmiStatus fmiGetIntegerStatusTYPE(fmiComponent, const fmiStatusKind, fmiInteger*);
typedef fmiStatus fmiGetBooleanStatusTYPE(fmiComponent, const fmiStatusKind, fmiBoolean*);
typedef fmiStatus fmiGetStringStatusTYPE (fmiComponent, const fmiStatusKind, fmiString* );


#ifdef __cplusplus
}  /* end of extern "C" { */
#endif

#endif /* fmiFunctionTypes_h */
