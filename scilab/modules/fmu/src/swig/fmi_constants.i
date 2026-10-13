/***************************************************
 * FMI v1                                          *
 ***************************************************/

/* Type definitions */
typedef enum
{
    fmiOK,
    fmiWarning,
    fmiDiscard,
    fmiError,
    fmiFatal,
    fmiPending
} fmiStatus;

typedef enum
{
    fmiDoStepStatus,
    fmiPendingStatus,
    fmiLastSuccessfulTime
} fmiStatusKind;

typedef unsigned int fmiValueReference;
typedef double       fmiReal   ;
typedef int          fmiInteger;
typedef char         fmiBoolean;

typedef /*const*/ char*  fmiString ;

/* Values for fmiBoolean  */
#define fmiTrue 1
#define fmiFalse 0

/* Undefined value for fmiValueReference (largest unsigned int value) */
#define fmiUndefinedValueReference (unsigned int)(-1)


/***************************************************
 * FMI v2                                          *
 ***************************************************/

/* Type definitions */
typedef enum
{
    fmi2OK,
    fmi2Warning,
    fmi2Discard,
    fmi2Error,
    fmi2Fatal,
    fmi2Pending
} fmi2Status;

typedef enum
{
    fmi2ModelExchange,
    fmi2CoSimulation
} fmi2Type;

typedef enum
{
    fmi2DoStepStatus,
    fmi2PendingStatus,
    fmi2LastSuccessfulTime,
    fmi2Terminated
} fmi2StatusKind;

typedef unsigned int    fmi2ValueReference;
typedef double          fmi2Real   ;
typedef int             fmi2Integer;
typedef int             fmi2Boolean;
typedef char            fmi2Char;
typedef /*const*/ fmi2Char* fmi2String;
typedef char            fmi2Byte;

/* Values for fmi2Boolean  */
#define fmi2True 1
#define fmi2False 0


/***************************************************
 * FMI v3                                          *
 ***************************************************/

typedef enum {
    fmi3OK,
    fmi3Warning,
    fmi3Discard,
    fmi3Error,
    fmi3Fatal,
} fmi3Status;

typedef enum {
    fmi3Independent,
    fmi3Constant,
    fmi3Fixed,
    fmi3Tunable,
    fmi3Discrete,
    fmi3Dependent
} fmi3DependencyKind;

typedef enum {
    fmi3IntervalNotYetKnown,
    fmi3IntervalUnchanged,
    fmi3IntervalChanged
} fmi3IntervalQualifier;

/* tag::Component[] */
typedef           void* fmi3Instance;             /* Pointer to the FMU instance */
/* end::Component[] */

/* tag::ComponentEnvironment[] */
typedef           void* fmi3InstanceEnvironment;  /* Pointer to the FMU environment */
/* end::ComponentEnvironment[] */

/* tag::FMUState[] */
typedef           void* fmi3FMUState;             /* Pointer to the internal FMU state */
/* end::FMUState[] */

/* tag::ValueReference[] */
typedef        uint32_t fmi3ValueReference;       /* Handle to the value of a variable */
/* end::ValueReference[] */

/* tag::VariableTypes[] */
typedef           float fmi3Float32;  /* Single precision floating point (32-bit) */
/* tag::fmi3Float64[] */
typedef          double fmi3Float64;  /* Double precision floating point (64-bit) */
/* end::fmi3Float64[] */
typedef          int8_t fmi3Int8;     /* 8-bit signed integer */
typedef         uint8_t fmi3UInt8;    /* 8-bit unsigned integer */
typedef         int16_t fmi3Int16;    /* 16-bit signed integer */
typedef        uint16_t fmi3UInt16;   /* 16-bit unsigned integer */
typedef         int32_t fmi3Int32;    /* 32-bit signed integer */
typedef        uint32_t fmi3UInt32;   /* 32-bit unsigned integer */
typedef         int64_t fmi3Int64;    /* 64-bit signed integer */
typedef        uint64_t fmi3UInt64;   /* 64-bit unsigned integer */
typedef            bool fmi3Boolean;  /* Data type to be used with fmi3True and fmi3False */
typedef            char fmi3Char;     /* Data type for one character */
typedef /*const*/ fmi3Char* fmi3String;   /* Data type for character strings
                                         ('\0' terminated, UTF-8 encoded) */
typedef         uint8_t fmi3Byte;     /* Smallest addressable unit of the machine
                                         (typically one byte) */
typedef /*const*/ fmi3Byte* fmi3Binary;   /* Data type for binary data
                                         (out-of-band length terminated) */
typedef            bool fmi3Clock;    /* Data type to be used with fmi3ClockActive and
                                         fmi3ClockInactive */

/* Values for fmi3Boolean */
#define fmi3True  true
#define fmi3False false

/* Values for fmi3Clock */
#define fmi3ClockActive   true
#define fmi3ClockInactive false
/* end::VariableTypes[] */
