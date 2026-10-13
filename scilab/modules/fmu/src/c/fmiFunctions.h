#ifndef fmiFunctions_h
#define fmiFunctions_h

/*
This header file declares the functions of FMI 1.0.
It must be used when compiling an FMU.

In order to have unique function names even if several FMUs
are compiled together (e.g. for embedded systems), every "real" function name
is constructed by prepending the function name by "FMI_FUNCTION_PREFIX".
Therefore, the typical usage is:

  #define FMI_FUNCTION_PREFIX MyModel_
  #include "fmiFunctions.h"

As a result, a function that is defined as "fmiGetContinuousStateDerivatives" in this header file,
is actually getting the name "MyModel_fmiGetContinuousStateDerivatives".

This only holds if the FMU is shipped in C source code, or is compiled in a
static link library. For FMUs compiled in a DLL/sharedObject, the "actual" function
names are used and "FMI_FUNCTION_PREFIX" must not be defined.

Copyright (C) 2008-2011 MODELISAR consortium,
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

#include "fmiPlatformTypes.h"
#include "fmiFunctionTypes.h"
#include <stdlib.h>

/*
Allow override of FMI_FUNCTION_PREFIX: If FMI_OVERRIDE_FUNCTION_PREFIX
is defined, then FMI_ACTUAL_FUNCTION_PREFIX will be used, if defined,
or no prefix if undefined. Otherwise FMI_FUNCTION_PREFIX will be used,
if defined.
*/
#if !defined(FMI_OVERRIDE_FUNCTION_PREFIX) && defined(FMI_FUNCTION_PREFIX)
  #define FMI_ACTUAL_FUNCTION_PREFIX FMI_FUNCTION_PREFIX
#endif

/*
Export FMI API functions on Windows and under GCC.
If custom linking is desired then the FMI_Export must be
defined before including this file. For instance,
it may be set to __declspec(dllimport).
*/
#if !defined(FMI_Export)
  #if !defined(FMI_ACTUAL_FUNCTION_PREFIX)
    #if defined _WIN32 || defined __CYGWIN__
     /* Note: both gcc & MSVC on Windows support this syntax. */
        #define FMI_Export __declspec(dllexport)
    #else
      #if __GNUC__ >= 4
        #define FMI_Export __attribute__ ((visibility ("default")))
      #else
        #define FMI_Export
      #endif
    #endif
  #else
    #define FMI_Export
  #endif
#endif

/* Macros to construct the real function name (prepend function name by FMI_FUNCTION_PREFIX) */
#if defined(FMI_ACTUAL_FUNCTION_PREFIX)
  #define fmiPaste(a,b)     a ## b
  #define fmiPasteB(a,b)    fmiPaste(a,b)
  #define fmiFullName(name) fmiPasteB(FMI_ACTUAL_FUNCTION_PREFIX, name)
#else
  #define fmiFullName(name) name
#endif

/* FMI version */
#define fmiVersion "1.0"

/***************************************************
Common Functions
****************************************************/

/* Inquire version numbers and set debug logging */
FMI_Export fmiGetModelTypesPlatformTYPE fmiGetModelTypesPlatform;
FMI_Export fmiGetTypesPlatformTYPE      fmiGetTypesPlatform;
FMI_Export fmiGetVersionTYPE            fmiGetVersion;
FMI_Export fmiSetDebugLoggingTYPE       fmiSetDebugLogging;

/* Creation and destruction of FMU instances */
FMI_Export fmiInstantiateModelTYPE      fmiInstantiateModel;
FMI_Export fmiFreeModelInstanceTYPE     fmiFreeModelInstance;
FMI_Export fmiInstantiateSlaveTYPE      fmiInstantiateSlave;
FMI_Export fmiFreeSlaveInstanceTYPE     fmiFreeSlaveInstance;

/* Enter and exit initialization mode, terminate and reset */
FMI_Export fmiInitializeTYPE              fmiInitialize;
FMI_Export fmiCompletedIntegratorStepTYPE fmiCompletedIntegratorStep;
FMI_Export fmiTerminateTYPE               fmiTerminate;
FMI_Export fmiInitializeSlaveTYPE         fmiInitializeSlave;
FMI_Export fmiTerminateSlaveTYPE          fmiTerminateSlave;
FMI_Export fmiResetSlaveTYPE              fmiResetSlave;

/* Getting and setting variables values */
FMI_Export fmiGetRealTYPE    fmiGetReal;
FMI_Export fmiGetIntegerTYPE fmiGetInteger;
FMI_Export fmiGetBooleanTYPE fmiGetBoolean;
FMI_Export fmiGetStringTYPE  fmiGetString;
FMI_Export fmiSetRealTYPE    fmiSetReal;
FMI_Export fmiSetIntegerTYPE fmiSetInteger;
FMI_Export fmiSetBooleanTYPE fmiSetBoolean;
FMI_Export fmiSetStringTYPE  fmiSetString;

/* Getting partial derivatives */
FMI_Export fmiSetRealInputDerivativesTYPE      fmiSetRealInputDerivatives;
FMI_Export fmiGetRealOutputDerivativesTYPE     fmiGetRealOutputDerivatives;


/***************************************************
Functions for Model Exchange
****************************************************/

FMI_Export fmiCompletedIntegratorStepTYPE fmiCompletedIntegratorStep;

/* Providing independent variables and re-initialization of caching */
/* tag::SetTimeTYPE[] */
FMI_Export fmiSetTimeTYPE             fmiSetTime;
/* end::SetTimeTYPE[] */
FMI_Export fmiSetContinuousStatesTYPE fmiSetContinuousStates;

/* Evaluation of the model equations */
FMI_Export fmiGetDerivativesTYPE                fmiGetDerivatives;
FMI_Export fmiGetEventIndicatorsTYPE            fmiGetEventIndicators;
FMI_Export fmiEventUpdateTYPE                   fmiEventUpdate;
FMI_Export fmiGetContinuousStatesTYPE           fmiGetContinuousStates;
FMI_Export fmiGetNominalContinuousStatesTYPE    fmiGetNominalContinuousStates;
FMI_Export fmiGetStateValueReferencesTYPE       fmiGetStateValueReferences;


/***************************************************
Functions for Co-Simulation
****************************************************/

/* Simulating the FMU */
FMI_Export fmiDoStepTYPE               fmiDoStep;
FMI_Export fmiCancelStepTYPE           fmiCancelStep;

/* Inquire slave status */
FMI_Export fmiGetStatusTYPE            fmiGetStatus;
FMI_Export fmiGetRealStatusTYPE        fmiGetRealStatus;
FMI_Export fmiGetIntegerStatusTYPE     fmiGetIntegerStatus;
FMI_Export fmiGetBooleanStatusTYPE     fmiGetBooleanStatus;
FMI_Export fmiGetStringStatusTYPE      fmiGetStringStatus;

#ifdef __cplusplus
}  /* end of extern "C" { */
#endif

#endif /* fmiFunctions_h */
