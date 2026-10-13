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

// check the number of arguments

path = fmigetPath();
// arguments
arg1 = "fmu";
arg2 = [1,2,3];
arg3 = [3,2,1];
arg4 = %t;
arg5 = %f;

// fmiGetVersion
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetVersion",1); 
assert_checkerror("fmiGetVersion()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetVersion(arg1,arg2)",errmsg);
// fmiGetModelTypesPlatform
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetModelTypesPlatform",1); 
assert_checkerror("fmiGetModelTypesPlatform()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetModelTypesPlatform(arg1,arg2)",errmsg);
// fmiInstantiateModel
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiInstantiateModel",1); 
assert_checkerror("fmiInstantiateModel()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n"));
assert_checkerror("fmiInstantiateModel(arg1,arg2,arg3)",errmsg);
// fmiSetDebugLogging
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiSetDebugLogging",2); 
assert_checkerror("fmiSetDebugLogging()",errmsg); 
assert_checkerror("fmiSetDebugLogging(arg1)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiSetDebugLogging(arg1,arg2,arg3)",errmsg);
// fmiSetTime
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiSetTime",2); 
assert_checkerror("fmiSetTime()",errmsg); 
assert_checkerror("fmiSetTime(arg1)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiSetTime(arg1,arg2,arg3)",errmsg);
// fmiSetReal 
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiSetReal",3); 
assert_checkerror("fmiSetReal()",errmsg); 
assert_checkerror("fmiSetReal(arg1)",errmsg); 
assert_checkerror("fmiSetReal(arg1,arg2)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiSetReal(arg1,arg2,arg3,arg4)",errmsg);
// fmiSetInteger
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiSetInteger",3); 
assert_checkerror("fmiSetInteger()",errmsg);
assert_checkerror("fmiSetInteger(arg1)",errmsg); 
assert_checkerror("fmiSetInteger(arg1,arg2)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiSetInteger(arg1,arg2,arg3,arg4)",errmsg);
// fmiSetBoolean
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiSetBoolean",3); 
assert_checkerror("fmiSetBoolean()",errmsg);
assert_checkerror("fmiSetBoolean(arg1)",errmsg);
assert_checkerror("fmiSetBoolean(arg1,arg2)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiSetBoolean(arg1,arg2,arg3,arg4)",errmsg);
// fmiSetString
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiSetString",3); 
assert_checkerror("fmiSetString()",errmsg);
assert_checkerror("fmiSetString(arg1)",errmsg);
assert_checkerror("fmiSetString(arg1,arg2)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiSetString(arg1,arg2,arg3,arg4)",errmsg);
// fmiInitialize
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiInitialize",3); 
assert_checkerror("fmiInitialize()",errmsg); 
assert_checkerror("fmiInitialize(arg1)",errmsg); 
assert_checkerror("fmiInitialize(arg1,arg2)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiInitialize(arg1,arg2,arg3,arg4)",errmsg);
// fmiGetContinuousStates
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetContinuousStates",1); 
assert_checkerror("fmiGetContinuousStates()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetContinuousStates(arg1,arg2,arg3)",errmsg);
// fmiGetNominalContStates
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetNominalContStates",1); 
assert_checkerror("fmiGetNominalContStates()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetNominalContStates(arg1,arg2,arg3)",errmsg);
// fmiGetEventIndicators
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetEventIndicators",1); 
assert_checkerror("fmiGetEventIndicators()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetEventIndicators(arg1,arg2,arg3)",errmsg);
// fmiGetReal
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetReal",2); 
assert_checkerror("fmiGetReal()",errmsg); 
assert_checkerror("fmiGetReal(arg1)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetReal(arg1,arg2,arg3)",errmsg);
// fmiGetInteger
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetInteger",2); 
assert_checkerror("fmiGetInteger()",errmsg); 
assert_checkerror("fmiGetInteger(arg1)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetInteger(arg1,arg2,arg3)",errmsg);
// fmiGetBoolean
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetBoolean",2); 
assert_checkerror("fmiGetBoolean()",errmsg); 
assert_checkerror("fmiGetBoolean(arg1)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetBoolean(arg1,arg2,arg3)",errmsg);
// fmiGetString
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetString",2); 
assert_checkerror("fmiGetString()",errmsg); 
assert_checkerror("fmiGetString(arg1)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetString(arg1,arg2,arg3)",errmsg);
// fmiGetStateValueRefs
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetStateValueRefs",1); 
assert_checkerror("fmiGetStateValueRefs()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetStateValueRefs(arg1,arg2)",errmsg);
// fmiGetDerivatives
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiGetDerivatives",1); 
assert_checkerror("fmiGetDerivatives()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiGetDerivatives(arg1,arg2)",errmsg);
// fmiSetContinuousStates
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiSetContinuousStates",2); 
assert_checkerror("fmiSetContinuousStates()",errmsg);
assert_checkerror("fmiSetContinuousStates(arg1)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiSetContinuousStates(arg1,arg2,arg3)",errmsg);
// fmiCompletedIntegrStep
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiCompletedIntegrStep",1); 
assert_checkerror("fmiCompletedIntegrStep()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiCompletedIntegrStep(arg1,arg2)",errmsg);
// fmiEventUpdate
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiEventUpdate",2); 
assert_checkerror("fmiEventUpdate()",errmsg); 
assert_checkerror("fmiEventUpdate(arg1)",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiEventUpdate(arg1,arg2,arg3)",errmsg);
// fmiTerminate
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiTerminate",1); 
assert_checkerror("fmiTerminate()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiTerminate(arg1,arg2)",errmsg);
// fmiFreeModelInstance
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"),"fmiFreeModelInstance",1); 
assert_checkerror("fmiFreeModelInstance()",errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n")); 
assert_checkerror("fmiFreeModelInstance(arg1,arg2)",errmsg);
