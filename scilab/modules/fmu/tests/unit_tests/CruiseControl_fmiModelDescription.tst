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

// Test of main attributes of Cruise Control and elements 

path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "SCADE_CruiseControl__CruiseControl_FMU.fmu")); 
//check number of input arguments
errmsg = msprintf(_("%s: Wrong number of input arguments: %d expected.\n"), "fmiModelDescription", 1);
assert_checkerror("fmiModelDescription",errmsg, 999);
assert_checkerror("fmiModelDescription()",errmsg, 999);
//check FMU type
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiModelDescription", 1, "FMU");
assert_checkerror("fmiModelDescription(777)",errmsg, 999);
assert_checkerror("fmiModelDescription(""string"")",errmsg, 999);

descrCruise = fmiModelDescription(fmu);
//attributes
assert_checkequal(descrCruise.fmiVersion, "1.0");
assert_checkequal(descrCruise.fmiType, "Model Exchange");
assert_checkequal(descrCruise.modelName, "CruiseControl__CruiseControl_FMU");
assert_checkequal(descrCruise.modelIdentifier, "CruiseControl__CruiseControl_FMU");
assert_checkequal(descrCruise.guid, "{8c4e810f-3df3-4a00-8276-176fa3c9f004}");
assert_checkequal(descrCruise.description, []);
assert_checkequal(descrCruise.author, []);
assert_checkequal(descrCruise.version, []);
assert_checkequal(descrCruise.generationTool, []);
assert_checkequal(descrCruise.generationDateAndTime, []);
assert_checkequal(descrCruise.variableNamingConvention, []);
assert_checkequal(descrCruise.numberOfContinuousStates, 0);
assert_checkequal(descrCruise.numberOfEventIndicators, 0);
//Unit definitions and type definitions
assert_checkequal(descrCruise.UnitDefinitions, 0);
assert_checkequal(descrCruise.TypeDefinitions, 0);
//default experiment
assert_checkequal(descrCruise.DefaultExperiment.startTime, []);
assert_checkequal(descrCruise.DefaultExperiment.stopTime, []);
assert_checkequal(descrCruise.DefaultExperiment.tolerance, []);
//vendor annotations tool
assert_checkequal(descrCruise.VendorAnnotations, 0);
//Model variables
assert_checkequal(descrCruise.ModelVariables, 12);
// inputs
assert_checkequal(fmu.setValues.inputs, ["Accel","Brake","Speed","On","Off","Resume","Set","QuickAccel","QuickDecel"]);
