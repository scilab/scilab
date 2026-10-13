/* --------------------------------------------------------------------------
 *
 *  Arrays typemaps for (const fmiValueReference vr[], size_t nvr, const FMITYPE value[])
 *
 * --------------------------------------------------------------------------*/

%include "sciint_swg_missing.i"

// callbacks are always the same
%typemap(in, numinputs=0) fmiCallbackFunctions {
    memset(&$1, 0, sizeof(fmiCallbackFunctions));
    $1.logger = fmu_callback_logger;
    $1.allocateMemory = fmu_callback_calloc;
    $1.freeMemory = fmu_callback_free;
}

// FMI 2.0 takes its callbacks by pointer and their members are const, so
// no Scilab value can build them. Supplied here, or fmi2Instantiate is
// unreachable and no FMI 2.0 FMU can be instantiated at all.
%typemap(in, numinputs=0) const fmi2CallbackFunctions *functions {
    static fmi2CallbackFunctions fmu2_callbacks
        { fmu2_callback_logger, fmu_callback_calloc, fmu_callback_free, NULL, NULL };
    $1 = &fmu2_callbacks;
}

// some types are boolean-like
%apply bool { fmiBoolean };
%apply bool* OUTPUT { fmiBoolean* OUTPUT };
%apply bool { fmi2Boolean };
%apply bool* OUTPUT { fmi2Boolean* OUTPUT };

// typemaps.i declares an unused "res" for these OUTPUT pointers, and the
// Scilab argout typemaps never reference it: redefine the "in" typemaps
%typemap(in, numinputs=0, noblock=1) fmiBoolean *OUTPUT (fmiBoolean temp),
                                     fmi2Boolean *OUTPUT (fmi2Boolean temp) {
    $1 = &temp;
}
%typemap(in, numinputs=0, noblock=1) fmiReal *OUTPUT (fmiReal temp),
                                     fmiInteger *OUTPUT (fmiInteger temp) {
    $1 = &temp;
}

// size_t indexes should be converted to int to apply the typemap
%apply (double IN[], int IN_SIZE) { (const fmiReal x[], size_t nx) };

// loggingOn is optional
%typemap(default) fmiBoolean loggingOn {
    $1 = fmiFalse;
}

// fmiComponent might be null after reporting an error on the callback, set a scilab error
%typemap(out) fmiComponent {
  if ($1 == NULL) {
    Scierror(SCILAB_API_ARGUMENT_ERROR, _("%s: %s model instantiation failed.\n"), fname, arg2);
  } else {
    if (!SWIG_IsOK(SWIG_Scilab_SetOutput(pvApiCtx, SwigScilabPtrFromObject(pvApiCtx, SWIG_Scilab_GetOutputPosition(), (new fmiComponent($1)), SWIGTYPE_p_fmiComponent, SWIG_POINTER_OWN |  0 , NULL)))) return SWIG_ERROR;
  }
}
%apply Pointer NONNULL { fmiComponent };

// typemap define for setters
%define %scilabfmi_asarray(FRAGMENTNAME, FMITYPE, SCILABTYPE)
%typemap(in, fragment="FRAGMENTNAME") (const FMITYPE value[]) (bool allocated) {
  int iRows = 0;
  int iCols = 0;
  SCILABTYPE* pTempData = NULL;
  if (FRAGMENTNAME(pvApiCtx, $argnum - 1, &iRows, &iCols, &pTempData, fname)) {
    return SWIG_ERROR;
  }
  size_t len = (size_t) iRows * (size_t) iCols;
  if (len != arg4) {
    Scierror(SCILAB_API_ARGUMENT_ERROR, _("%s: Wrong size for input argument #%d: A %d-element vector expected.\n"), fname, $argnum, arg4);
    return SWIG_ERROR;
  }
  if (sizeof(FMITYPE) != sizeof(SCILABTYPE)) {
    FMITYPE* pValues = (FMITYPE*) MALLOC(len * sizeof(FMITYPE));
    for (size_t i = 0; i < len; ++i) {
      pValues[i] = pTempData[i];
    }
    allocated = true;
    $1 = pValues;
  } else {
    allocated = false;
    $1 = (FMITYPE*) pTempData;
  }
}
%typemap(freearg) (const FMITYPE value[]) {
  if (allocated$argnum) FREE($1);
}
%enddef


// typemap define for getters
%define %scilabfmi_fromarray(FRAGMENTNAME, FMITYPE, SCILABTYPE)
%typemap(in, numinputs=1, fragment="FRAGMENTNAME") (const fmiValueReference vr[], size_t nvr, FMITYPE OUTPUT[]) (int iRows, int iCols) {
  unsigned int *pTempRefs = NULL;
  if (SWIG_SciDoubleOrUint32_AsUnsignedIntArrayAndSize(pvApiCtx, $argnum, &iRows, &iCols, &pTempRefs, fname)) {
    return SWIG_ERROR;
  }
  $1 = pTempRefs;
  $2 = (size_t) iRows * (size_t) iCols;
  if ($2 > 0) {
    $3 = ($3_ltype) MALLOC($2 * sizeof(FMITYPE));
  } else {
    $3 = NULL;
  }
}
%typemap(argout, noblock=1, fragment="FRAGMENTNAME") (const fmiValueReference vr[], size_t nvr, FMITYPE OUTPUT[]) {
  if ($2 <= 0) {
    %set_output(SWIG_SciDouble_FromDoubleArrayAndSize(pvApiCtx, $result, 0, 0, NULL));
  } else if (sizeof(FMITYPE) != sizeof(SCILABTYPE)) {
    SCILABTYPE* pValues = (SCILABTYPE*) MALLOC($2 * sizeof(SCILABTYPE));
    for (size_t i = 0; i < $2; ++i) {
      pValues[i] = $3[i];
    }
    FREE($3);
    %set_output(FRAGMENTNAME(pvApiCtx, $result, iRows$argnum, iCols$argnum, pValues));
    FREE(pValues);
  } else {
    %set_output(FRAGMENTNAME(pvApiCtx, $result, iRows$argnum, iCols$argnum, (SCILABTYPE*) $3));
    FREE($3);
  }
}
%enddef


// typemap define for both getters and setters
%define %scilabfmi_array_typemaps(FMITYPE, ASARRAY_FRAGMENT, FROMARRAY_FRAGMENT, SCILABTYPE)
  %scilabfmi_asarray(ASARRAY_FRAGMENT, FMITYPE, SCILABTYPE);
  %scilabfmi_fromarray(FROMARRAY_FRAGMENT, FMITYPE, SCILABTYPE);
%enddef

%scilabfmi_array_typemaps(fmiReal, SWIG_SciDouble_AsDoubleArrayAndSize, SWIG_SciDouble_FromDoubleArrayAndSize, double);
%scilabfmi_array_typemaps(fmiInteger, SWIG_SciDoubleOrInt32_AsIntArrayAndSize, SWIG_SciInt32_FromIntArrayAndSize, int);
%scilabfmi_array_typemaps(fmiBoolean, SWIG_SciBoolean_AsIntArrayAndSize, SWIG_SciBoolean_FromIntArrayAndSize, int);
%scilabfmi_array_typemaps(fmiString, SWIG_SciString_AsCharPtrArrayAndSize, SWIG_SciString_FromCharPtrArrayAndSize, char*);

%scilabfmi_array_typemaps(fmi2Real, SWIG_SciDouble_AsDoubleArrayAndSize, SWIG_SciDouble_FromDoubleArrayAndSize, double);
%scilabfmi_array_typemaps(fmi2Integer, SWIG_SciDoubleOrInt32_AsIntArrayAndSize, SWIG_SciInt32_FromIntArrayAndSize, int);
%scilabfmi_array_typemaps(fmi2Boolean, SWIG_SciBoolean_AsIntArrayAndSize, SWIG_SciBoolean_FromIntArrayAndSize, int);
%scilabfmi_array_typemaps(fmi2String, SWIG_SciString_AsCharPtrArrayAndSize, SWIG_SciString_FromCharPtrArrayAndSize, char*);


// fmiEventInfo* is always an output argument
%typemap(in, numinputs=0) (fmiEventInfo *eventInfo) (fmiEventInfo eventInfo) {
    $1 = &eventInfo;
}
%typemap(argout) (fmiEventInfo *eventInfo) {
    double data[] = {
      static_cast<double>($1->iterationConverged),
      static_cast<double>($1->stateValueReferencesChanged),
      static_cast<double>($1->stateValuesChanged),
      static_cast<double>($1->terminateSimulation),
      static_cast<double>($1->upcomingTimeEvent),
      $1->nextEventTime,
    };
    %set_output(SWIG_SciDouble_FromDoubleArrayAndSize(pvApiCtx, $result, sizeof(data) / sizeof(*data), 1, data));
}


// fmiReal[] as output argument
%typemap(in, numinputs=1, noblock=1, fragment="SWIG_SciDoubleOrInt32_AsInt") (fmiReal OUT[], int OUT_SIZE) {
  if (SWIG_SciDoubleOrInt32_AsInt(pvApiCtx, $argnum, &$2, fname)) {
    return SWIG_ERROR;
  }
  $1 = (fmiReal*) malloc($2 * sizeof(fmiReal)); 
}
%typemap(argout, noblock=1, fragment="SWIG_SciDouble_FromDoubleArrayAndSize") (fmiReal OUT[], int OUT_SIZE) {
  %set_output(SWIG_SciDouble_FromDoubleArrayAndSize(pvApiCtx, SWIG_Scilab_GetOutputPosition(), 1, $2, $1) == SWIG_OK);
}
%typemap(freearg, noblock=1) (fmiReal OUT[], int OUT_SIZE) {
  free($1);
}


// fmiValueReference[] as input argument
%typemap(in, numinputs=1, fragment="SWIG_SciDoubleOrUint32_AsUnsignedIntArrayAndSize") (const fmiValueReference vr[], size_t nvr) {
  int iRows = 0;
  int iCols = 0;
  unsigned int *pTempRefs = NULL;
  if (SWIG_SciDoubleOrUint32_AsUnsignedIntArrayAndSize(pvApiCtx, $argnum, &iRows, &iCols, &pTempRefs, fname)) {
    return SWIG_ERROR;
  }
  $1 = pTempRefs;
  $2 = (size_t) iRows * (size_t) iCols;
}


// FMI 2.0 declares its own value-reference type, so the 1.0 typemaps do
// not match its getters: value[] stays an input and only the status comes
// back. Reusing them returns the values and deduces nvr from vr[].
%apply (const fmiValueReference vr[], size_t nvr) {
       (const fmi2ValueReference vr[], size_t nvr) };
%apply (const fmiValueReference vr[], size_t nvr, fmiReal OUTPUT[]) {
       (const fmi2ValueReference vr[], size_t nvr, fmi2Real OUTPUT[]) };
%apply (const fmiValueReference vr[], size_t nvr, fmiInteger OUTPUT[]) {
       (const fmi2ValueReference vr[], size_t nvr, fmi2Integer OUTPUT[]) };
%apply (const fmiValueReference vr[], size_t nvr, fmiBoolean OUTPUT[]) {
       (const fmi2ValueReference vr[], size_t nvr, fmi2Boolean OUTPUT[]) };

// fmiValueReference[] as output argument
%typemap(in, numinputs=1, noblock=1, fragment="SWIG_SciDoubleOrInt32_AsInt") (fmiValueReference OUT[], int OUT_SIZE) {
  if (SWIG_SciDoubleOrInt32_AsInt(pvApiCtx, $argnum, &$2, fname)) {
    return SWIG_ERROR;
  }
  $1 = (fmiValueReference*) malloc($2 * sizeof(fmiValueReference)); 
}
%typemap(argout, noblock=1, fragment="SWIG_SciDouble_FromUnsignedIntArrayAndSize") (fmiValueReference OUT[], int OUT_SIZE) {
  %set_output(SWIG_SciDouble_FromUnsignedIntArrayAndSize(pvApiCtx, SWIG_Scilab_GetOutputPosition(), 1, $2, $1) == SWIG_OK);
}
%typemap(freearg, noblock=1) (fmiValueReference OUT[], int OUT_SIZE) {
  free($1);
}
