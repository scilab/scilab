//
// SWIG (https://www.swig.org) mapping of fmi interface into Scilab
//
// to update run:
// $ swig -c++ -scilab -gatewayxml6 -I../c -I../../includes fmuswig.i
//
%module fmuswig
%include "typemaps.i"
%include "matrix.i"
%include "constraints.i"
%include "fmu_typemaps.i"

// constants are wrapped as Scilab variables
%scilabconst(1);
%include "fmi_constants.i"

/*
 * Fake SWIG classes used to map fmu_wrapper.hxx implementation
 *
 * These classes are implemented in C-like indirection with function pointers
 * loaded with dlopen/dlsym. A SWIG generated C++ class use "->" to call these
 * function pointers and effectivly call the mapped function ! 
 */

%{
extern "C" {
#include "fmiPlatformTypes.h"
#include "fmiFunctions.h"
#include "fmiFunctionTypes.h"

#include "fmi2TypesPlatform.h"
#include "fmi2FunctionTypes.h"
#include "fmi2Functions.h"

#include "fmi3PlatformTypes.h"
#include "fmi3Functions.h"
#include "fmi3FunctionTypes.h"
}

#include "fmu_wrapper.hxx"
%}

// factory method
FmuWrapper* allocate_fmu(const std::string& path, const std::string& prefix);

// do not generate constructor nor destructor for these classes
%nodefaultctor;
%nodefaultdtor;
#pragma SWIG nowarn=302

class FmuModelExchange
{
public:
    %include "fmi1_me.i"
};

class FmuCoSimulation
{
public:
    %include "fmi1_cs.i"
};

class Fmu2ModelExchange
{
public:
    %include "fmi2_me.i"
};

class Fmu2CoSimulation
{
public:
    %include "fmi2_cs.i"
};

class Fmu3ModelExchange
{
public:
    %include "fmi3_me.i"
};

class Fmu3CoSimulation
{
public:
    %include "fmi3_cs.i"
};

class Fmu3ScheduledExecution
{
public:
    %include "fmi3_se.i"
};
