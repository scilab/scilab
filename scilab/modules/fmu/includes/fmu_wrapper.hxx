#ifndef __FMU_WRAPPER_HXX_
#define __FMU_WRAPPER_HXX_

#include <string>

extern "C" {
#include "dynlib_fmucpp_wrapper.h"

#include "fmiPlatformTypes.h"
#include "fmiFunctionTypes.h"
#include "fmiFunctions.h"

#include "fmi2TypesPlatform.h"
#include "fmi2FunctionTypes.h"
#include "fmi2Functions.h"

#include "fmi3PlatformTypes.h"
#include "fmi3FunctionTypes.h"
#include "fmi3Functions.h"
}

struct FMUCPP_IMPEXP FmuWrapper {
    FmuWrapper(std::string _version, std::string _type);
    virtual ~FmuWrapper();

    bool create(const std::string& _path, const std::string& _name);

protected:
    virtual bool loadCommonSymbols() = 0;
    virtual bool loadSymbols() = 0;

    void* lib;
    std::string name;

public:
    const std::string version;
    const std::string type;
};

extern "C" FMUCPP_IMPEXP
FmuWrapper* fmucpp_wrapper(FmuWrapper* w);


struct FMUCPP_IMPEXP FmuV1Common : public FmuWrapper {
    FmuV1Common(std::string _type);
    virtual ~FmuV1Common();

#define FMI_KLASS_METHOD(fun) fun##TYPE* fun;
    FMI_KLASS_METHOD(fmiGetVersion)
    FMI_KLASS_METHOD(fmiSetDebugLogging)
    FMI_KLASS_METHOD(fmiSetReal)
    FMI_KLASS_METHOD(fmiSetInteger)
    FMI_KLASS_METHOD(fmiSetBoolean)
    FMI_KLASS_METHOD(fmiSetString)
    FMI_KLASS_METHOD(fmiGetReal)
    FMI_KLASS_METHOD(fmiGetInteger)
    FMI_KLASS_METHOD(fmiGetBoolean)
    FMI_KLASS_METHOD(fmiGetString)
#undef FMI_KLASS_METHOD

protected:
    bool loadCommonSymbols() override;

};

struct FMUCPP_IMPEXP FmuModelExchange final : public FmuV1Common {
    FmuModelExchange();
    virtual ~FmuModelExchange();

#define FMI_KLASS_METHOD(fun) fun##TYPE* fun;
    FMI_KLASS_METHOD(fmiGetModelTypesPlatform)
    FMI_KLASS_METHOD(fmiInstantiateModel)
    FMI_KLASS_METHOD(fmiFreeModelInstance)
    FMI_KLASS_METHOD(fmiSetTime)
    FMI_KLASS_METHOD(fmiSetContinuousStates)
    FMI_KLASS_METHOD(fmiCompletedIntegratorStep)
    FMI_KLASS_METHOD(fmiInitialize)
    FMI_KLASS_METHOD(fmiGetDerivatives)
    FMI_KLASS_METHOD(fmiGetEventIndicators)
    FMI_KLASS_METHOD(fmiEventUpdate)
    FMI_KLASS_METHOD(fmiGetContinuousStates)
    FMI_KLASS_METHOD(fmiGetNominalContinuousStates)
    FMI_KLASS_METHOD(fmiGetStateValueReferences)
    FMI_KLASS_METHOD(fmiTerminate)
#undef FMI_KLASS_METHOD

private:
    virtual bool loadSymbols() override;
};

struct FMUCPP_IMPEXP FmuCoSimulation final : public FmuV1Common {
    FmuCoSimulation();
    virtual ~FmuCoSimulation();

#define FMI_KLASS_METHOD(fun) fun##TYPE* fun;
    FMI_KLASS_METHOD(fmiGetTypesPlatform)
    FMI_KLASS_METHOD(fmiInstantiateSlave)
    FMI_KLASS_METHOD(fmiInitializeSlave)
    FMI_KLASS_METHOD(fmiTerminateSlave)
    FMI_KLASS_METHOD(fmiResetSlave)
    FMI_KLASS_METHOD(fmiFreeSlaveInstance)
    FMI_KLASS_METHOD(fmiSetRealInputDerivatives)
    FMI_KLASS_METHOD(fmiGetRealOutputDerivatives)
    FMI_KLASS_METHOD(fmiDoStep)
    FMI_KLASS_METHOD(fmiCancelStep)
    FMI_KLASS_METHOD(fmiGetStatus)
    FMI_KLASS_METHOD(fmiGetRealStatus)
    FMI_KLASS_METHOD(fmiGetIntegerStatus)
    FMI_KLASS_METHOD(fmiGetBooleanStatus)
    FMI_KLASS_METHOD(fmiGetStringStatus)
#undef FMI_KLASS_METHOD

private:
    virtual bool loadSymbols() override;
};

struct FMUCPP_IMPEXP FmuV2Common : public FmuWrapper {
    FmuV2Common(std::string _type);
    virtual ~FmuV2Common();

#define FMI_KLASS_METHOD(fun) fun##TYPE* fun;
    FMI_KLASS_METHOD(fmi2GetTypesPlatform)
    FMI_KLASS_METHOD(fmi2GetVersion)
    FMI_KLASS_METHOD(fmi2SetDebugLogging)
    FMI_KLASS_METHOD(fmi2Instantiate)
    FMI_KLASS_METHOD(fmi2FreeInstance)
    FMI_KLASS_METHOD(fmi2SetupExperiment)
    FMI_KLASS_METHOD(fmi2EnterInitializationMode)
    FMI_KLASS_METHOD(fmi2ExitInitializationMode)
    FMI_KLASS_METHOD(fmi2Terminate)
    FMI_KLASS_METHOD(fmi2Reset)
    FMI_KLASS_METHOD(fmi2SetReal)
    FMI_KLASS_METHOD(fmi2SetInteger)
    FMI_KLASS_METHOD(fmi2SetBoolean)
    FMI_KLASS_METHOD(fmi2SetString)
    FMI_KLASS_METHOD(fmi2GetReal)
    FMI_KLASS_METHOD(fmi2GetInteger)
    FMI_KLASS_METHOD(fmi2GetBoolean)
    FMI_KLASS_METHOD(fmi2GetString)
    FMI_KLASS_METHOD(fmi2GetFMUstate)
    FMI_KLASS_METHOD(fmi2SetFMUstate)
    FMI_KLASS_METHOD(fmi2FreeFMUstate)
    FMI_KLASS_METHOD(fmi2SerializedFMUstateSize)
    FMI_KLASS_METHOD(fmi2SerializeFMUstate)
    FMI_KLASS_METHOD(fmi2DeSerializeFMUstate)
    FMI_KLASS_METHOD(fmi2GetDirectionalDerivative)
#undef FMI_KLASS_METHOD

protected:
    bool loadCommonSymbols() override;

};

struct FMUCPP_IMPEXP Fmu2ModelExchange final : public FmuV2Common {
    Fmu2ModelExchange();
    virtual ~Fmu2ModelExchange();

#define FMI_KLASS_METHOD(fun) fun##TYPE* fun;
    FMI_KLASS_METHOD(fmi2EnterEventMode)
    FMI_KLASS_METHOD(fmi2NewDiscreteStates)
    FMI_KLASS_METHOD(fmi2EnterContinuousTimeMode)
    FMI_KLASS_METHOD(fmi2CompletedIntegratorStep)
    FMI_KLASS_METHOD(fmi2SetTime)
    FMI_KLASS_METHOD(fmi2SetContinuousStates)
    FMI_KLASS_METHOD(fmi2GetEventIndicators)
    FMI_KLASS_METHOD(fmi2GetContinuousStates)
    FMI_KLASS_METHOD(fmi2GetDerivatives)
    FMI_KLASS_METHOD(fmi2GetNominalsOfContinuousStates)
#undef FMI_KLASS_METHOD

private:
    virtual bool loadSymbols() override;
};

struct FMUCPP_IMPEXP Fmu2CoSimulation final : public FmuV2Common {
    Fmu2CoSimulation();
    virtual ~Fmu2CoSimulation();

#define FMI_KLASS_METHOD(fun) fun##TYPE* fun;
    FMI_KLASS_METHOD(fmi2SetRealInputDerivatives)
    FMI_KLASS_METHOD(fmi2GetRealOutputDerivatives)
    FMI_KLASS_METHOD(fmi2DoStep)
    FMI_KLASS_METHOD(fmi2CancelStep)
    FMI_KLASS_METHOD(fmi2GetStatus)
    FMI_KLASS_METHOD(fmi2GetRealStatus)
    FMI_KLASS_METHOD(fmi2GetIntegerStatus)
    FMI_KLASS_METHOD(fmi2GetBooleanStatus)
    FMI_KLASS_METHOD(fmi2GetStringStatus)
#undef FMI_KLASS_METHOD

private:
    virtual bool loadSymbols() override;
};

struct FMUCPP_IMPEXP FmuV3Common : public FmuWrapper {
    FmuV3Common(std::string _type);
    virtual ~FmuV3Common();

#define FMI_KLASS_METHOD(fun) fun##TYPE* fun;
    FMI_KLASS_METHOD(fmi3GetVersion)
    FMI_KLASS_METHOD(fmi3SetDebugLogging)
    FMI_KLASS_METHOD(fmi3InstantiateModelExchange)
    FMI_KLASS_METHOD(fmi3InstantiateCoSimulation)
    FMI_KLASS_METHOD(fmi3InstantiateScheduledExecution)
    FMI_KLASS_METHOD(fmi3FreeInstance)
    FMI_KLASS_METHOD(fmi3EnterInitializationMode)
    FMI_KLASS_METHOD(fmi3ExitInitializationMode)
    FMI_KLASS_METHOD(fmi3EnterEventMode)
    FMI_KLASS_METHOD(fmi3Terminate)
    FMI_KLASS_METHOD(fmi3Reset)
    FMI_KLASS_METHOD(fmi3GetFloat32)
    FMI_KLASS_METHOD(fmi3GetFloat64)
    FMI_KLASS_METHOD(fmi3GetInt8)
    FMI_KLASS_METHOD(fmi3GetUInt8)
    FMI_KLASS_METHOD(fmi3GetInt16)
    FMI_KLASS_METHOD(fmi3GetUInt16)
    FMI_KLASS_METHOD(fmi3GetInt32)
    FMI_KLASS_METHOD(fmi3GetUInt32)
    FMI_KLASS_METHOD(fmi3GetInt64)
    FMI_KLASS_METHOD(fmi3GetUInt64)
    FMI_KLASS_METHOD(fmi3GetBoolean)
    FMI_KLASS_METHOD(fmi3GetString)
    FMI_KLASS_METHOD(fmi3GetBinary)
    FMI_KLASS_METHOD(fmi3GetClock)
    FMI_KLASS_METHOD(fmi3SetFloat32)
    FMI_KLASS_METHOD(fmi3SetFloat64)
    FMI_KLASS_METHOD(fmi3SetInt8)
    FMI_KLASS_METHOD(fmi3SetUInt8)
    FMI_KLASS_METHOD(fmi3SetInt16)
    FMI_KLASS_METHOD(fmi3SetUInt16)
    FMI_KLASS_METHOD(fmi3SetInt32)
    FMI_KLASS_METHOD(fmi3SetUInt32)
    FMI_KLASS_METHOD(fmi3SetInt64)
    FMI_KLASS_METHOD(fmi3SetUInt64)
    FMI_KLASS_METHOD(fmi3SetBoolean)
    FMI_KLASS_METHOD(fmi3SetString)
    FMI_KLASS_METHOD(fmi3SetBinary)
    FMI_KLASS_METHOD(fmi3SetClock)
    FMI_KLASS_METHOD(fmi3GetNumberOfVariableDependencies)
    FMI_KLASS_METHOD(fmi3GetVariableDependencies)
    FMI_KLASS_METHOD(fmi3GetFMUState)
    FMI_KLASS_METHOD(fmi3SetFMUState)
    FMI_KLASS_METHOD(fmi3FreeFMUState)
    FMI_KLASS_METHOD(fmi3SerializedFMUStateSize)
    FMI_KLASS_METHOD(fmi3SerializeFMUState)
    FMI_KLASS_METHOD(fmi3DeserializeFMUState)
    FMI_KLASS_METHOD(fmi3GetDirectionalDerivative)
    FMI_KLASS_METHOD(fmi3GetAdjointDerivative)
    FMI_KLASS_METHOD(fmi3EnterConfigurationMode)
    FMI_KLASS_METHOD(fmi3ExitConfigurationMode)
    FMI_KLASS_METHOD(fmi3GetIntervalDecimal)
    FMI_KLASS_METHOD(fmi3GetIntervalFraction)
    FMI_KLASS_METHOD(fmi3GetShiftDecimal)
    FMI_KLASS_METHOD(fmi3GetShiftFraction)
    FMI_KLASS_METHOD(fmi3SetIntervalDecimal)
    FMI_KLASS_METHOD(fmi3SetIntervalFraction)
    FMI_KLASS_METHOD(fmi3SetShiftDecimal)
    FMI_KLASS_METHOD(fmi3SetShiftFraction)
    FMI_KLASS_METHOD(fmi3EvaluateDiscreteStates)
    FMI_KLASS_METHOD(fmi3UpdateDiscreteStates)
#undef FMI_KLASS_METHOD

protected:
    bool loadCommonSymbols() override;

};

struct FMUCPP_IMPEXP Fmu3ModelExchange final : public FmuV3Common  {
    Fmu3ModelExchange();
    virtual ~Fmu3ModelExchange();

#define FMI_KLASS_METHOD(fun) fun##TYPE* fun;
FMI_KLASS_METHOD(fmi3EnterContinuousTimeMode)
FMI_KLASS_METHOD(fmi3CompletedIntegratorStep)
FMI_KLASS_METHOD(fmi3SetTime)
FMI_KLASS_METHOD(fmi3SetContinuousStates)
FMI_KLASS_METHOD(fmi3GetContinuousStateDerivatives)
FMI_KLASS_METHOD(fmi3GetEventIndicators)
FMI_KLASS_METHOD(fmi3GetContinuousStates)
FMI_KLASS_METHOD(fmi3GetNominalsOfContinuousStates)
FMI_KLASS_METHOD(fmi3GetNumberOfEventIndicators)
FMI_KLASS_METHOD(fmi3GetNumberOfContinuousStates)
#undef FMI_KLASS_METHOD

private:
    virtual bool loadSymbols() override;
};

struct FMUCPP_IMPEXP Fmu3CoSimulation final : public FmuV3Common  {
    Fmu3CoSimulation();
    virtual ~Fmu3CoSimulation();

#define FMI_KLASS_METHOD(fun) fun##TYPE* fun;
FMI_KLASS_METHOD(fmi3EnterStepMode)
FMI_KLASS_METHOD(fmi3GetOutputDerivatives)
FMI_KLASS_METHOD(fmi3DoStep)
#undef FMI_KLASS_METHOD

private:
    virtual bool loadSymbols() override;
};

struct FMUCPP_IMPEXP Fmu3ScheduledExecution final : public FmuV3Common  {
    Fmu3ScheduledExecution();
    virtual ~Fmu3ScheduledExecution();

#define FMI_KLASS_METHOD(fun) fun##TYPE* fun;
FMI_KLASS_METHOD(fmi3ActivateModelPartition)
#undef FMI_KLASS_METHOD

private:
    virtual bool loadSymbols() override;
};


/**
* log some information into the Scilab console
*/
extern "C"
FMUCPP_IMPEXP void fmu_callback_logger(fmiComponent c, fmiString instanceName,
    fmiStatus status, fmiString category,
    fmiString message, ...);

/**
* log some information into the Scilab console (FMI 2.0)
*/
extern "C"
FMUCPP_IMPEXP void fmu2_callback_logger(fmi2ComponentEnvironment c, fmi2String instanceName,
    fmi2Status status, fmi2String category,
    fmi2String message, ...);

/**
* log some information into the Scilab console (FMI 3.0)
*/
extern "C"
FMUCPP_IMPEXP void fmu3_callback_logger(fmi3InstanceEnvironment c,
    fmi3Status status, fmi3String category,
    fmi3String message);

/**
* allocate memory which can be freed by any FMU runtime
*/
extern "C"
FMUCPP_IMPEXP void * fmu_callback_calloc(size_t nobj, size_t size);

/**
* free memory allocated by fmu_callback_calloc
*/
extern "C"
FMUCPP_IMPEXP void fmu_callback_free(void * obj);

/**
 * @brief Scicos block work field to be passed around instances.
 */
struct FMUCPP_IMPEXP workFMU {
    enum fmiType
    {
        ModelExchange,
        CoSimulation
    };
    fmiType type;

    enum fmiVersionEnum
    {
        fmiV1,
        fmiV2,
        fmiV3
    };
    fmiVersionEnum version;

    union
    {
        FmuModelExchange* fmuME1;
        FmuCoSimulation* fmuCS1;
        Fmu2ModelExchange* fmuME2;
        Fmu2CoSimulation* fmuCS2;
        Fmu3ModelExchange* fmuME3;
        Fmu3CoSimulation* fmuCS3;
    };
    void* instance; // could be fmiComponent or fmi2Component
    union
    {
        struct
        {
            fmiReal *states, *nominalStates, *events, *tmpevents, *derivatives;
            fmiBoolean *boolValue, callEventUpdate, stateEvent, timeEvent;
            fmiValueReference *refsStates, *realRefs, *intRefs, *boolRefs;
            fmiEventInfo eventInfo;
            fmiReal oldTime;
        } modelExchangeV1;
        struct
        {
            fmiReal *states, *nominalStates, *events, *tmpevents, *derivatives;
            fmiBoolean *boolValue, callEventUpdate, stateEvent, timeEvent;
            fmiValueReference *refsStates, *realRefs, *intRefs, *boolRefs;
            fmiEventInfo eventInfo;
            fmiReal oldTime;
        } coSimulationV1;
        struct
        {
            fmi2Boolean enterEventMode, timeEvent;
            fmi2EventInfo eventInfo;

            enum {
                Instantiated,
                InitializationMode,
                EventMode,
                ContinuousTimeMode
            } state;
        } modelExchangeV2;
        struct
        {
            fmi2Boolean callEventUpdate, stateEvent, timeEvent;
            fmi2EventInfo eventInfo;
            fmi2Real oldTime;

            enum {
                Instantiated,
                InitializationMode,
                StepCompleted,
                StepInProgress,
                StepFailed
            } state;
        } coSimulationV2;
        struct
        {
            fmi3Boolean enterEventMode, timeEvent;
            fmi3Boolean nextEventTimeDefined;
            fmi3Float64 nextEventTime;

            enum {
                Instantiated,
                InitializationMode,
                EventMode,
                ContinuousTimeMode
            } state;
        } modelExchangeV3;
        struct
        {
            fmi3Boolean callEventUpdate, stateEvent, timeEvent, nextEventTimeDefined;
            fmi3Float64 nextEventTime;
            fmi3Float64 oldTime;

            enum {
                Instantiated,
                InitializationMode,
                EventMode,
                StepMode
            } state;
        } coSimulationV3;
    } data;
};

/**
 * Detect the FMU wrapper implementation from entry points.
 * 
 * If multiple versions are available, list them all as comma separated.
 */
FMUCPP_IMPEXP
void detect_fmu_version(const std::string &path, const std::string &prefix, std::string &impl);

/**
 * Allocate the FMU wrapper from entry points.
 * 
 * If multiple versions are available:
 *  - ModelExchange is prefered over CoSimulation
 *  - CoSimulation is prefered over Scheduled Execution
 *  - a more recent version is prefered (3.0 then 2.0 then 1.0)
 */
FMUCPP_IMPEXP
FmuWrapper* allocate_fmu(const std::string& path, const std::string& prefix);


#endif /* !__FMU_WRAPPER_HXX_ */