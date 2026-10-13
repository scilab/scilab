#ifdef _MSC_VER
// Workaround for mingw builds
#define NOCRYPT
#define NOGDI
#define NOMINMAX
#include <windows.h>
#undef NOMINMAX
#else
#include <dlfcn.h>
#endif

extern "C"
{
#include "dynlib_fmucpp_wrapper.h"
#include "sciprint.h"
#include "getFullFilename.h"
#include "sci_malloc.h"
}
#include "fmu_wrapper.hxx"

static FmuWrapper *current;
/** set the currently used FMU library */
extern "C" FmuWrapper *fmucpp_wrapper(FmuWrapper *w)
{
    FmuWrapper *pre = current;
    if (w != nullptr)
        current = w;
    return pre;
}

namespace
{
    void *loadSymbol(void *lib, const std::string &prefix, const std::string &name)
    {
        // detect more specific name first (with modelIdentifier)
        std::string prefixedName = prefix + "_" + name;
        void *function_ptr;
#ifdef _MSC_VER
        function_ptr = (void *)GetProcAddress((HMODULE)lib, prefixedName.data());
#else
        function_ptr = dlsym(lib, prefixedName.data());
#endif

        // detect without prefix, for the DLL builds
        if (function_ptr == nullptr)
        {
#ifdef _MSC_VER
            function_ptr = (void *)GetProcAddress((HMODULE)lib, name.data());
#else
            function_ptr = dlsym(lib, name.data());
#endif
        }

        return function_ptr;
    };

    struct AutoReleaseLibrary
    {
        void* _lib;
        AutoReleaseLibrary(void* lib) : _lib(lib) {};

        ~AutoReleaseLibrary()
        {
#ifdef _MSC_VER
        FreeLibrary((HMODULE) _lib);
#else
        dlclose(_lib);
#endif
        };
    };
} // namespace

FmuWrapper::FmuWrapper(std::string _version, std::string _type) : lib(nullptr), name(), version(_version), type(_type) {};

FmuWrapper::~FmuWrapper()
{
    if (lib != nullptr)
        AutoReleaseLibrary a(lib);
}

bool FmuWrapper::create(const std::string &_path, const std::string &_name)
{
    name = _name;
    char* resolved = getFullFilename(_path.c_str());
#ifdef _MSC_VER
    lib = LoadLibraryA(resolved);
#else
    lib = dlopen(resolved, RTLD_NOW | RTLD_LOCAL);
#endif
    FREE(resolved);

    if (lib == nullptr)
    {
        sciprint("fmu_link: unable to load library from \"%s\"\n", _path.c_str());
        return false;
    }

    if (loadCommonSymbols() == false)
    {
        return false;
    }

    return loadSymbols();
}

FmuV1Common::FmuV1Common(std::string _type) : FmuWrapper("1.0", _type) {};
FmuV1Common::~FmuV1Common() {};

bool FmuV1Common::loadCommonSymbols()
{
#define FMI_KLASS_METHOD(fun)                             \
    this->fun = (fun##TYPE *)loadSymbol(lib, name, #fun); \
    if (this->fun == NULL)                                \
    {                                                     \
        sciprint("fmu_link: unable to load symbol %s\n", #fun);       \
        return false;                                     \
    }

    FMI_KLASS_METHOD(fmiGetVersion);
    FMI_KLASS_METHOD(fmiSetDebugLogging);
    FMI_KLASS_METHOD(fmiSetReal);
    FMI_KLASS_METHOD(fmiSetInteger);
    FMI_KLASS_METHOD(fmiSetBoolean);
    FMI_KLASS_METHOD(fmiSetString);
    FMI_KLASS_METHOD(fmiGetReal);
    FMI_KLASS_METHOD(fmiGetInteger);
    FMI_KLASS_METHOD(fmiGetBoolean);
    FMI_KLASS_METHOD(fmiGetString);
#undef FMI_KLASS_METHOD
    return true;
}

FmuModelExchange::FmuModelExchange() : FmuV1Common("me") {};
FmuModelExchange::~FmuModelExchange() {};

bool FmuModelExchange::loadSymbols()
{
#define FMI_KLASS_METHOD(fun)                             \
    this->fun = (fun##TYPE *)loadSymbol(lib, name, #fun); \
    if (this->fun == NULL)                                \
    {                                                     \
        sciprint("fmu_link: unable to load symbol %s", #fun);       \
        return false;                                     \
    }

    FMI_KLASS_METHOD(fmiGetModelTypesPlatform);
    FMI_KLASS_METHOD(fmiInstantiateModel);
    FMI_KLASS_METHOD(fmiFreeModelInstance);
    FMI_KLASS_METHOD(fmiSetTime);
    FMI_KLASS_METHOD(fmiSetContinuousStates);
    FMI_KLASS_METHOD(fmiCompletedIntegratorStep);
    FMI_KLASS_METHOD(fmiInitialize);
    FMI_KLASS_METHOD(fmiGetDerivatives);
    FMI_KLASS_METHOD(fmiGetEventIndicators);
    FMI_KLASS_METHOD(fmiEventUpdate);
    FMI_KLASS_METHOD(fmiGetContinuousStates);
    FMI_KLASS_METHOD(fmiGetNominalContinuousStates);
    FMI_KLASS_METHOD(fmiGetStateValueReferences);
    FMI_KLASS_METHOD(fmiTerminate);
#undef FMI_KLASS_METHOD
    return true;
}

FmuCoSimulation::FmuCoSimulation() : FmuV1Common("cs") {};
FmuCoSimulation::~FmuCoSimulation() {};

bool FmuCoSimulation::loadSymbols()
{
#define FMI_KLASS_METHOD(fun)                             \
    this->fun = (fun##TYPE *)loadSymbol(lib, name, #fun); \
    if (this->fun == NULL)                                \
    {                                                     \
        sciprint("fmu_link: unable to load symbol %s", #fun);       \
        return false;                                     \
    }

    FMI_KLASS_METHOD(fmiGetTypesPlatform);
    FMI_KLASS_METHOD(fmiInstantiateSlave);
    FMI_KLASS_METHOD(fmiInitializeSlave);
    FMI_KLASS_METHOD(fmiTerminateSlave);
    FMI_KLASS_METHOD(fmiResetSlave);
    FMI_KLASS_METHOD(fmiFreeSlaveInstance);
    FMI_KLASS_METHOD(fmiSetRealInputDerivatives);
    FMI_KLASS_METHOD(fmiGetRealOutputDerivatives);
    FMI_KLASS_METHOD(fmiDoStep);
    FMI_KLASS_METHOD(fmiCancelStep);
    FMI_KLASS_METHOD(fmiGetStatus);
    FMI_KLASS_METHOD(fmiGetRealStatus);
    FMI_KLASS_METHOD(fmiGetIntegerStatus);
    FMI_KLASS_METHOD(fmiGetBooleanStatus);
    FMI_KLASS_METHOD(fmiGetStringStatus);
#undef FMI_KLASS_METHOD
    return true;
}

FmuV2Common::FmuV2Common(std::string _type) : FmuWrapper("2.0", _type) {};
FmuV2Common::~FmuV2Common() {};

bool FmuV2Common::loadCommonSymbols()
{
#define FMI_KLASS_METHOD(fun)                             \
    this->fun = (fun##TYPE *)loadSymbol(lib, name, #fun); \
    if (this->fun == NULL)                                \
    {                                                     \
        sciprint("fmu_link: unable to load symbol %s\n", #fun);       \
        return false;                                     \
    }

    FMI_KLASS_METHOD(fmi2GetTypesPlatform);
    FMI_KLASS_METHOD(fmi2GetVersion);
    FMI_KLASS_METHOD(fmi2SetDebugLogging);
    FMI_KLASS_METHOD(fmi2Instantiate);
    FMI_KLASS_METHOD(fmi2FreeInstance);
    FMI_KLASS_METHOD(fmi2SetupExperiment);
    FMI_KLASS_METHOD(fmi2EnterInitializationMode);
    FMI_KLASS_METHOD(fmi2ExitInitializationMode);
    FMI_KLASS_METHOD(fmi2Terminate);
    FMI_KLASS_METHOD(fmi2Reset);
    FMI_KLASS_METHOD(fmi2SetReal);
    FMI_KLASS_METHOD(fmi2SetInteger);
    FMI_KLASS_METHOD(fmi2SetBoolean);
    FMI_KLASS_METHOD(fmi2SetString);
    FMI_KLASS_METHOD(fmi2GetReal);
    FMI_KLASS_METHOD(fmi2GetInteger);
    FMI_KLASS_METHOD(fmi2GetBoolean);
    FMI_KLASS_METHOD(fmi2GetString);
    FMI_KLASS_METHOD(fmi2GetFMUstate);
    FMI_KLASS_METHOD(fmi2SetFMUstate);
    FMI_KLASS_METHOD(fmi2FreeFMUstate);
    FMI_KLASS_METHOD(fmi2SerializedFMUstateSize);
    FMI_KLASS_METHOD(fmi2SerializeFMUstate);
    FMI_KLASS_METHOD(fmi2DeSerializeFMUstate);
    FMI_KLASS_METHOD(fmi2GetDirectionalDerivative);
#undef FMI_KLASS_METHOD
    return true;
}

Fmu2ModelExchange::Fmu2ModelExchange() : FmuV2Common("me") {};
Fmu2ModelExchange::~Fmu2ModelExchange() {};

bool Fmu2ModelExchange::loadSymbols()
{
#define FMI_KLASS_METHOD(fun)                             \
    this->fun = (fun##TYPE *)loadSymbol(lib, name, #fun); \
    if (this->fun == NULL)                                \
    {                                                     \
        sciprint("fmu_link: unable to load symbol %s", #fun);       \
        return false;                                     \
    }

    FMI_KLASS_METHOD(fmi2EnterEventMode);
    FMI_KLASS_METHOD(fmi2NewDiscreteStates);
    FMI_KLASS_METHOD(fmi2EnterContinuousTimeMode);
    FMI_KLASS_METHOD(fmi2CompletedIntegratorStep);
    FMI_KLASS_METHOD(fmi2SetTime);
    FMI_KLASS_METHOD(fmi2SetContinuousStates);
    FMI_KLASS_METHOD(fmi2GetEventIndicators);
    FMI_KLASS_METHOD(fmi2GetContinuousStates);
    FMI_KLASS_METHOD(fmi2GetDerivatives);
    FMI_KLASS_METHOD(fmi2GetNominalsOfContinuousStates);
#undef FMI_KLASS_METHOD
    return true;
}

Fmu2CoSimulation::Fmu2CoSimulation() : FmuV2Common("cs") {};
Fmu2CoSimulation::~Fmu2CoSimulation() {};

bool Fmu2CoSimulation::loadSymbols()
{
#define FMI_KLASS_METHOD(fun)                             \
    this->fun = (fun##TYPE *)loadSymbol(lib, name, #fun); \
    if (this->fun == NULL)                                \
    {                                                     \
        sciprint("fmu_link: unable to load symbol %s", #fun);       \
        return false;                                     \
    }

    FMI_KLASS_METHOD(fmi2SetRealInputDerivatives);
    FMI_KLASS_METHOD(fmi2GetRealOutputDerivatives);
    FMI_KLASS_METHOD(fmi2DoStep);
    FMI_KLASS_METHOD(fmi2CancelStep);
    FMI_KLASS_METHOD(fmi2GetStatus);
    FMI_KLASS_METHOD(fmi2GetRealStatus);
    FMI_KLASS_METHOD(fmi2GetIntegerStatus);
    FMI_KLASS_METHOD(fmi2GetBooleanStatus);
    FMI_KLASS_METHOD(fmi2GetStringStatus);
#undef FMI_KLASS_METHOD
    return true;
}

FmuV3Common::FmuV3Common(std::string _type) : FmuWrapper("3.0", _type) {};
FmuV3Common::~FmuV3Common() {};

bool FmuV3Common::loadCommonSymbols()
{
#define FMI_KLASS_METHOD(fun)                             \
    this->fun = (fun##TYPE *)loadSymbol(lib, name, #fun); \
    if (this->fun == NULL)                                \
    {                                                     \
        sciprint("fmu_link: unable to load symbol %s\n", #fun);       \
        return false;                                     \
    }

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
    return true;
}

Fmu3ModelExchange::Fmu3ModelExchange() : FmuV3Common("me") {};
Fmu3ModelExchange::~Fmu3ModelExchange() {};

bool Fmu3ModelExchange::loadSymbols()
{
#define FMI_KLASS_METHOD(fun)                             \
    this->fun = (fun##TYPE *)loadSymbol(lib, name, #fun); \
    if (this->fun == NULL)                                \
    {                                                     \
        sciprint("fmu_link: unable to load symbol %s", #fun);       \
        return false;                                     \
    }

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
    return true;
}

Fmu3CoSimulation::Fmu3CoSimulation() : FmuV3Common("cs") {};
Fmu3CoSimulation::~Fmu3CoSimulation() {};

bool Fmu3CoSimulation::loadSymbols()
{
#define FMI_KLASS_METHOD(fun)                             \
    this->fun = (fun##TYPE *)loadSymbol(lib, name, #fun); \
    if (this->fun == NULL)                                \
    {                                                     \
        sciprint("fmu_link: unable to load symbol %s", #fun);       \
        return false;                                     \
    }

    FMI_KLASS_METHOD(fmi3EnterStepMode)
    FMI_KLASS_METHOD(fmi3GetOutputDerivatives)
    FMI_KLASS_METHOD(fmi3DoStep)
#undef FMI_KLASS_METHOD
    return true;
}

Fmu3ScheduledExecution::Fmu3ScheduledExecution() : FmuV3Common("se") {};
Fmu3ScheduledExecution::~Fmu3ScheduledExecution() {};

bool Fmu3ScheduledExecution::loadSymbols()
{
#define FMI_KLASS_METHOD(fun)                             \
    this->fun = (fun##TYPE *)loadSymbol(lib, name, #fun); \
    if (this->fun == NULL)                                \
    {                                                     \
        sciprint("fmu_link: unable to load symbol %s", #fun);       \
        return false;                                     \
    }

    FMI_KLASS_METHOD(fmi3ActivateModelPartition)
#undef FMI_KLASS_METHOD
    return true;
}

void fmu_callback_logger(fmiComponent, fmiString instanceName, fmiStatus status, fmiString category, fmiString message, ...)
{
    va_list ap;
    const char* name = instanceName ? instanceName : "?";
    const char* cat = category ? category : "?";

    sciprint("fmiStatus = %d\n", status);
    sciprint("%s (%s)\n", name, cat);
    va_start(ap, message);

    scivprint(message, ap);
    va_end(ap);

    /* FMI for ModelExchange v1.0: "logger function will append a line break to each message" */
    sciprint("\n");
}

void fmu2_callback_logger(fmi2ComponentEnvironment, fmi2String instanceName, fmi2Status status, fmi2String category, fmi2String message, ...)
{
    va_list ap;
    const char* name = instanceName ? instanceName : "?";
    const char* cat = category ? category : "?";

    sciprint("fmiStatus = %d\n", status);
    sciprint("%s (%s)\n", name, cat);
    va_start(ap, message);

    scivprint(message, ap);
    va_end(ap);

    sciprint("\n");
}

void fmu3_callback_logger(fmi3InstanceEnvironment, fmi3Status status, fmi3String category, fmi3String message)
{
    const char* cat = category ? category : "?";

    sciprint("fmiStatus = %d\n (%s) %s\n", status, cat, message);
}

void * fmu_callback_calloc(size_t nobj, size_t size)
{
#ifdef _MSC_VER
    /* the FMU frees this memory with its own CRT, use the process heap */
    return HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nobj * size);
#else
    return calloc(nobj, size);
#endif
}

void fmu_callback_free(void * obj)
{
#ifdef _MSC_VER
    HeapFree(GetProcessHeap(), 0, obj);
#else
    free(obj);
#endif
}

void detect_fmu_version(const std::string &path, const std::string &prefix, std::string &impl)
{
    impl.clear();
    
    void *lib;
    char* resolved = getFullFilename(path.c_str());
#ifdef _MSC_VER
    lib = LoadLibraryA(resolved);
#else
    lib = dlopen(resolved, RTLD_LAZY | RTLD_LOCAL);
#endif
    FREE(resolved);

    if (lib == nullptr)
    {
        return;
    }
    AutoReleaseLibrary _l(lib);

    void *fmi3SetTime = loadSymbol(lib, prefix, "fmi3SetTime");
    if (fmi3SetTime != nullptr)
    {
        impl += "me 3.0, ";
    }

    void *fmi2SetTime = loadSymbol(lib, prefix, "fmi2SetTime");
    if (fmi2SetTime != nullptr)
    {
        impl += "me 2.0, ";
    }

    void *fmiInstantiateModel = loadSymbol(lib, prefix, "fmiInstantiateModel");
    if (fmiInstantiateModel != nullptr)
    {
        impl += "me 1.0, ";
    }

    void *fmi3DoStep = loadSymbol(lib, prefix, "fmi3DoStep");
    if (fmi3DoStep != nullptr)
    {
        impl += "cs 3.0, ";
    }

    void *fmi2DoStep = loadSymbol(lib, prefix, "fmi2DoStep");
    if (fmi2DoStep != nullptr)
    {
        impl += "cs 2.0, ";
    }

    void *fmiInstantiateSlave = loadSymbol(lib, prefix, "fmiInstantiateSlave");
    if (fmiInstantiateSlave != nullptr)
    {
        impl += "cs 1.0, ";
    }

    void *fmi3ActivateModelPartition = loadSymbol(lib, prefix, "fmi3ActivateModelPartition");
    if (fmi3ActivateModelPartition != nullptr)
    {
        impl += "se 3.0, ";
    }

    if (impl.size() > 2)
        impl.erase(impl.size()-2, 2);
}

FmuWrapper* allocate_fmu(const std::string &path, const std::string &prefix)
{
    void *lib;
    char* resolved = getFullFilename(path.c_str());
#ifdef _MSC_VER
    lib = LoadLibraryA(resolved);
#else
    lib = dlopen(resolved, RTLD_LAZY | RTLD_LOCAL);
#endif
    FREE(resolved);

    if (lib == nullptr)
    {
        return nullptr;
    }
    AutoReleaseLibrary _l(lib);

    void *fmi3SetTime = loadSymbol(lib, prefix, "fmi3SetTime");
    if (fmi3SetTime != nullptr)
    {
        Fmu3ModelExchange* wrapper = new Fmu3ModelExchange();
        if(wrapper->create(path, prefix))
        {
            return wrapper;
        }
        delete wrapper;
        return nullptr;
    }

    void *fmi2SetTime = loadSymbol(lib, prefix, "fmi2SetTime");
    if (fmi2SetTime != nullptr)
    {
        Fmu2ModelExchange* wrapper = new Fmu2ModelExchange();
        if(wrapper->create(path, prefix))
        {
            return wrapper;
        }
        delete wrapper;
        return nullptr;
    }

    void *fmiInstantiateModel = loadSymbol(lib, prefix, "fmiInstantiateModel");
    if (fmiInstantiateModel != nullptr)
    {
        FmuModelExchange* wrapper = new FmuModelExchange();
        if(wrapper->create(path, prefix))
        {
            return wrapper;
        }
        delete wrapper;
        return nullptr;
    }

    void *fmi3DoStep = loadSymbol(lib, prefix, "fmi3DoStep");
    if (fmi3DoStep != nullptr)
    {
        Fmu3CoSimulation* wrapper = new Fmu3CoSimulation();
        if(wrapper->create(path, prefix))
        {
            return wrapper;
        }
        delete wrapper;
        return nullptr;
    }

    void *fmi2DoStep = loadSymbol(lib, prefix, "fmi2DoStep");
    if (fmi2DoStep != nullptr)
    {
        Fmu2CoSimulation* wrapper = new Fmu2CoSimulation();
        if(wrapper->create(path, prefix))
        {
            return wrapper;
        }
        delete wrapper;
        return nullptr;
    }

    void *fmiInstantiateSlave = loadSymbol(lib, prefix, "fmiInstantiateSlave");
    if (fmiInstantiateSlave != nullptr)
    {
        FmuCoSimulation* wrapper = new FmuCoSimulation();
        if(wrapper->create(path, prefix))
        {
            return wrapper;
        }
        delete wrapper;
        return nullptr;
    }

    void *fmi3ActivateModelPartition = loadSymbol(lib, prefix, "fmi3ActivateModelPartition");
    if (fmi3ActivateModelPartition != nullptr)
    {
        Fmu3ScheduledExecution* wrapper = new Fmu3ScheduledExecution();
        if(wrapper->create(path, prefix))
        {
            return wrapper;
        }
        delete wrapper;
        return nullptr;
    }

    return nullptr;
}
