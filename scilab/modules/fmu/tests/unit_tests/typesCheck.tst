//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// <-- NO CHECK REF -->
// Check type of arguments

path = fmigetPath();
// arguments
argMatrix = [1,2,3,4];
argScalar = 10;
argBoolean = %t;
argBooleanMatrix = [%t,%t,%t];
argFMU = importFMU(fullfile(path, "tests", "unit_tests", "SCADE_CruiseControl__CruiseControl_FMU.fmu"));
argStringMatrix = ["What","is","up","?"];
argString = "fmi";
argFmuInstance = fmiInstantiateModel(argFMU);

// fmiGetVersion
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetVersion", 1, "FMU"); 
assert_checkerror("fmiGetVersion(argString)",errmsg);

// fmiGetModelTypesPlatform
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetModelTypesPlatform", 1, "FMU");
assert_checkerror("fmiGetModelTypesPlatform(argString)",errmsg);

// fmiInstantiateModel
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiInstantiateModel", 1, "FMU"); 
assert_checkerror("fmiInstantiateModel(argString)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: it is already an instance of FMU.\n"), "fmiInstantiateModel",1); 
assert_checkerror("fmiInstantiateModel(argFmuInstance)",errmsg);

// fmiSetDebugLogging
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSetDebugLogging", 1, "FMU instance"); 
assert_checkerror("fmiSetDebugLogging(argFMU,argBoolean)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSetDebugLogging", 2, "boolean"); 
assert_checkerror("fmiSetDebugLogging(argFmuInstance,argScalar)",errmsg);
assert_checkerror("fmiSetDebugLogging(argFmuInstance,argBooleanMatrix)",errmsg);

// fmiSetTime
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSetTime", 1, "FMU instance"); 
assert_checkerror("fmiSetTime(argFMU,argScalar)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSetTime", 2, "scalar"); 
assert_checkerror("fmiSetTime(argFmuInstance,argBoolean)",errmsg);
assert_checkerror("fmiSetTime(argFmuInstance,argMatrix)",errmsg);

// fmiSetReal
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSetReal", 1, "FMU instance"); 
assert_checkerror("fmiSetReal(argFMU,argScalar,argScalar)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiSetReal", 2); 
assert_checkerror("fmiSetReal(argFmuInstance,argBoolean,argBoolean)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiSetReal", 3);  
assert_checkerror("fmiSetReal(argFmuInstance,argMatrix,argBoolean)",errmsg);
assert_checkerror("fmiSetReal(argFmuInstance,argScalar,argString)",errmsg);
errmsg = msprintf(_("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"), "fmiSetReal",2,3);  
assert_checkerror("fmiSetReal(argFmuInstance,argScalar,argMatrix)",errmsg);

// fmiSetInteger
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSetInteger", 1, "FMU instance"); 
assert_checkerror("fmiSetInteger(argFMU,argScalar,argScalar)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiSetInteger", 2);
assert_checkerror("fmiSetInteger(argFmuInstance,argBoolean,argBoolean)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiSetInteger", 3);
assert_checkerror("fmiSetInteger(argFmuInstance,argMatrix,argBoolean)",errmsg);
assert_checkerror("fmiSetInteger(argFmuInstance,argScalar,argString)",errmsg);
errmsg = msprintf(_("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"), "fmiSetInteger",2,3);
assert_checkerror("fmiSetInteger(argFmuInstance,argScalar,argMatrix)",errmsg);

// fmiSetBoolean
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSetBoolean", 1, "FMU instance");
assert_checkerror("fmiSetBoolean(argFMU,argScalar,argBoolean)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiSetBoolean", 2);
assert_checkerror("fmiSetBoolean(argFmuInstance,argBoolean,argBoolean)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Boolean matrix expected.\n"), "fmiSetBoolean", 3); 
assert_checkerror("fmiSetBoolean(argFmuInstance,argMatrix,argMatrix)",errmsg);
assert_checkerror("fmiSetBoolean(argFmuInstance,argScalar,argString)",errmsg);
errmsg = msprintf(_("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"), "fmiSetBoolean",2,3);
assert_checkerror("fmiSetBoolean(argFmuInstance,argScalar,argBooleanMatrix)",errmsg);

// fmiSetString
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSetString", 1, "FMU instance");
assert_checkerror("fmiSetString(argFMU,argScalar,argString)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiSetString", 2);
assert_checkerror("fmiSetString(argFmuInstance,argBoolean,argString)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "fmiSetString", 3);
assert_checkerror("fmiSetString(argFmuInstance,argMatrix,argMatrix)",errmsg);
assert_checkerror("fmiSetString(argFmuInstance,argScalar,argBoolean)",errmsg);
errmsg = msprintf(_("%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n"), "fmiSetString",2,3);
assert_checkerror("fmiSetString(argFmuInstance,argScalar,argStringMatrix)",errmsg);

// fmiInitialize
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiInitialize", 1, "FMU instance");
assert_checkerror("fmiInitialize(argFMU,argScalar,argString)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiInitialize", 2, "boolean");
assert_checkerror("fmiInitialize(argFmuInstance,argBooleanMatrix,argString)",errmsg);
assert_checkerror("fmiInitialize(argFmuInstance,argScalar,argScalar)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiInitialize", 3, "scalar");
assert_checkerror("fmiInitialize(argFmuInstance,argBoolean,argMatrix)",errmsg);
assert_checkerror("fmiInitialize(argFmuInstance,argBoolean,argBoolean)",errmsg);

// fmiGetContinuousStates
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetContinuousStates", 1, "FMU instance");
assert_checkerror("fmiGetContinuousStates(argFMU)",errmsg);

// fmiGetNominalContStates
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetNominalContStates", 1, "FMU instance");
assert_checkerror("fmiGetNominalContStates(argFMU)",errmsg);

// fmiGetEventIndicators
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetEventIndicators", 1, "FMU instance");
assert_checkerror("fmiGetEventIndicators(argFMU)",errmsg);

// fmiGetReal
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetReal", 1, "FMU instance"); 
assert_checkerror("fmiGetReal(argFMU,argScalar)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiGetReal", 2);
assert_checkerror("fmiGetReal(argFmuInstance,argStringMatrix)",errmsg);  

// fmiGetInteger
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetInteger", 1, "FMU instance"); 
assert_checkerror("fmiGetInteger(argFMU,argScalar)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiGetInteger", 2);
assert_checkerror("fmiGetInteger(argFmuInstance,argString)",errmsg);
assert_checkerror("fmiGetInteger(argFmuInstance,argBoolean)",errmsg);

// fmiGetBoolean
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetBoolean", 1, "FMU instance");
assert_checkerror("fmiGetBoolean(argFMU,argScalar)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiGetBoolean", 2);
assert_checkerror("fmiGetBoolean(argFmuInstance,argStringMatrix)",errmsg);

// fmiGetString
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetString", 1, "FMU instance");
assert_checkerror("fmiGetString(argFMU,argScalar)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiGetString", 2);
assert_checkerror("fmiGetString(argFmuInstance,argBoolean)",errmsg);

// fmiGetStateValueRefs
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetStateValueRefs", 1, "FMU instance");
assert_checkerror("fmiGetStateValueRefs(argFMU)",errmsg);

// fmiGetDerivatives
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiGetDerivatives", 1, "FMU instance");
assert_checkerror("fmiGetDerivatives(argFMU)",errmsg);

// fmiSetContinuousStates 
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSetContinuousStates", 1, "FMU instance");
assert_checkerror("fmiSetContinuousStates(argFMU,argMatrix)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiSetContinuousStates", 2);
assert_checkerror("fmiSetContinuousStates(argFmuInstance,argBoolean)",errmsg);

// fmiCompletedIntegrStep
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiCompletedIntegrStep", 1, "FMU instance");
assert_checkerror("fmiCompletedIntegrStep(argFMU)",errmsg);

// fmiEventUpdate 
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiEventUpdate", 1, "FMU instance");
assert_checkerror("fmiEventUpdate(argFMU,argMatrix)",errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiEventUpdate", 2, "boolean");
assert_checkerror("fmiEventUpdate(argFmuInstance,argBooleanMatrix)",errmsg);
assert_checkerror("fmiEventUpdate(argFmuInstance,argString)",errmsg);

// fmiTerminate
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiTerminate", 1, "FMU instance");
assert_checkerror("fmiTerminate(argFMU)",errmsg);

// fmiFreeModelInstance
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiFreeModelInstance", 1, "FMU instance");
assert_checkerror("fmiFreeModelInstance(argFMU)",errmsg);
