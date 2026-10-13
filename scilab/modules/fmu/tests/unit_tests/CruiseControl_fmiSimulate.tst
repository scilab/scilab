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

// Simulation of the fmu Cruise Control

path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "SCADE_CruiseControl__CruiseControl_FMU.fmu"));

// check number of input arguments in the calling of 'fmiSimulate'
errmsg = msprintf(_("%s: Wrong number of input arguments: %d or %d expected.\n"), "fmiSimulate", 1, 3);
assert_checkerror("fmiSimulate", errmsg);
assert_checkerror("fmiSimulate()", errmsg);
assert_checkerror("fmiSimulate(""fmu"", [0, 0.1, 100])", errmsg);
errmsg = msprintf(_("Wrong number of input arguments.\n"));
assert_checkerror("fmiSimulate(""fmu"", [0, 0.1, 100], %t, [1])", errmsg);
// check type of input arguments
errmsg = msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSimulate", 1, "FMU");
assert_checkerror("fmiSimulate(111)", errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "fmiSimulate", 2);
assert_checkerror("fmiSimulate(fmu, [%t, %f], %f)", errmsg);
assert_checkerror("fmiSimulate(fmu, [0, 0.1], %f)", errmsg); 
assert_checkerror("fmiSimulate(fmu, [""fmi""], %f)", errmsg);
errmsg = msprintf(_("%s: Wrong type for input argument #%d: boolean expected.\n"), "fmiSimulate", 3);
assert_checkerror("fmiSimulate(fmu, [0, 0.1, 100], 1)", errmsg);
assert_checkerror("fmiSimulate(fmu, [0, 0.1, 100], ""true"")", errmsg);

// simulation with default values for all 'inputs/outputs' and 'start' variables
assert_checktrue(execstr("outDefault=fmiSimulate(fmu)", "errcatch")==0);

// defaults results of simulation 
// real
for i=1:5
    assert_checktrue(and(outDefault.Real.result(i).values==0));
end
// integer
    assert_checktrue(and(outDefault.Integer.result(1).values==0));
// boolean
for i=1:6
    assert_checktrue(and(outDefault.Boolean.result(i).values==%f));
end

// check that we have the results for all 'inputs/outputs' and 'start' vars
// Real
// names from model Description
inpOutAndStartNames = fmu.modelVariables.Real.name(~isnan(fmu.modelVariables.Real.start));
// names from result structure
names = emptystr(inpOutAndStartNames);
for i=1:size(outDefault.Real.result)
    names(i) = outDefault.Real.result(i).name;
end
assert_checkequal(inpOutAndStartNames, names);
// Integer 
inpOutAndStartNames = fmu.modelVariables.Integer.name(~isnan(fmu.modelVariables.Integer.start));
names = emptystr(inpOutAndStartNames);
names = outDefault.Integer.result.name;
assert_checkequal(inpOutAndStartNames, names);
// Boolean
inpOutAndStartNames = fmu.modelVariables.Boolean.name(find(fmu.modelVariables.Boolean.variability == "discrete" &...
                      fmu.modelVariables.Boolean.alias == "noAlias"));
// names from result structure
names = emptystr(inpOutAndStartNames);
for i=1:size(outDefault.Boolean.result)
    names(i) = outDefault.Boolean.result(i).name;
end
assert_checkequal(inpOutAndStartNames, names);

// simulation with user values
// get Values
fmu.getValues.realNames = ["Speed", "Brake", "CruiseSpeed"];
fmu.getValues.integerNames = ["CruiseState"];
fmu.getValues.booleanNames = ["On", "Set", "QuickAccel"];
assert_checktrue(execstr("outUser=fmiSimulate(fmu)", "errcatch")==0);
// real results
names = emptystr(fmu.getValues.realNames);
for i=1:size(outUser.Real.result)
    names(i) = outUser.Real.result(i).name;
end
assert_checkequal(fmu.getValues.realNames, names);
// integer results
names = outDefault.Integer.result.name;
assert_checkequal(fmu.getValues.integerNames, names);
// boolean results
names = emptystr(fmu.getValues.booleanNames);
for i=1:size(outUser.Boolean.result)
    names(i) = outUser.Boolean.result(i).name;
end
assert_checkequal(fmu.getValues.booleanNames, names);
// check the names
fmu.getValues.realNames = ["Speed", "WrongName", "Brake", "CruiseSpeed"];
errmsg = msprintf(_("%s: Wrong name(s) ""WrongName"" for getting real values.\n"), "fmiSimulate");
assert_checkerror("fmiSimulate(fmu)", errmsg);
fmu.getValues.realNames = ["Speed", "WrongName", "Brake", "CruiseSpeed", "UnknownName"];
errmsg = msprintf(_("%s: Wrong name(s) [""WrongName"",""UnknownName""] for getting real values.\n"), "fmiSimulate"); 
assert_checkerror("fmiSimulate(fmu)", errmsg);
fmu.getValues.realNames = [];

fmu.getValues.integerNames = ["WrongName", "CruiseState"];
errmsg = msprintf(_("%s: Wrong name(s) ""WrongName"" for getting integer values.\n"), "fmiSimulate");
assert_checkerror("fmiSimulate(fmu)", errmsg);
fmu.getValues.integerNames = [];

fmu.getValues.booleanNames = ["WrongName", "Set", "Name", "On"];
errmsg = msprintf(_("%s: Wrong name(s) [""WrongName"",""Name""] for getting boolean values.\n"), "fmiSimulate");
assert_checkerror("fmiSimulate(fmu)", errmsg);
fmu.getValues.booleanNames = [];

// set Values
fmu.setValues.realNames = ["Speed"]; 
fmu.setValues.realValues = [100];
// simulation with new values
assert_checktrue(execstr("outUser=fmiSimulate(fmu)", "errcatch")==0);
// result for "Speed" 
if assert_checkequal(outUser.Real.result(3).name, "Speed") then
    assert_checktrue(outUser.Real.result(3).values==100); 
end

fmu.setValues.booleanNames = ["On", "QuickAccel", "Set", "Resume"];
fmu.setValues.booleanValues = [%t, %t, %t, %t];
fmu.getValues.booleanNames = fmu.setValues.booleanNames;
// simulation with new values
assert_checktrue(execstr("outUser=fmiSimulate(fmu)", "errcatch")==0);
if assert_checkequal(outUser.Boolean.result(1).name, "On") then
    assert_checktrue(outUser.Boolean.result(1).values==%t);
end
if assert_checkequal(outUser.Boolean.result(2).name, "QuickAccel") then
    assert_checktrue(outUser.Boolean.result(2).values==%t);
end
if assert_checkequal(outUser.Boolean.result(3).name, "Set") then
    assert_checktrue(outUser.Boolean.result(3).values==%t);
end
if assert_checkequal(outUser.Boolean.result(4).name, "Resume") then
    assert_checktrue(outUser.Boolean.result(4).values==%t);
end

// reset all previous values 
fmu.setValues.booleanNames = [];
fmu.setValues.realNames = [];
fmu.setValues.integerNames = [];
fmu.setValues.booleanValues = [];
fmu.setValues.integerValues = [];
fmu.setValues.realValues = [];
fmu.getValues.realNames = [];
fmu.getValues.booleanNames = [];


// Exemple the using of cruise control 

// get Values for outpts
fmu.getValues.realNames = ["CruiseSpeed", "ThrottleCmd"];
fmu.getValues.integerNames = "CruiseState";

// fmu.setValues.inputs
// inputs: ["Accel", "Brake", "Speed", "On", "Off", "Resume", "Set", "QuickAccel", "QuickDecel"];

// First simulation
// On cruise control
fmu.setValues.input_4_values(1, :) = %t; // values 
fmu.setValues.input_4_values(2, 1) = 10; // time
// set speed
fmu.setValues.input_3_values(1, 1:10) = 100;
fmu.setValues.input_3_values(2, 1:10) = 10:19;
fmu.setValues.input_7_values(1, 1) = %t;
fmu.setValues.input_7_values(2, 1) = 12;
// acceleration
fmu.setValues.input_8_values(1, 1) = %t;
fmu.setValues.input_8_values(2, 1) = 22;
fmu.setValues.input_1_values(1, 11:20) = 120;
fmu.setValues.input_1_values(2, 11:20) = 20:29;
// simulation with new values // start time = 0; dTime = 0.1; ent Time = 100; debug logging Off = %f
assert_checktrue(execstr("outUser=fmiSimulate(fmu, [0, 0.1, 100], %f)", "errcatch")==0);
// results
assert_checkequal(outUser.Real.result(1).values($), 100); //Cruise Speed
assert_checkequal(outUser.Real.result(2).values($), 120); //Throtle Speed

// Second simulation 
// Off cruise control
fmu.setValues.input_4_values = []; // reset cruise On
fmu.setValues.input_5_values(1, :) = %f; // values 
fmu.setValues.input_5_values(2, 1) = 10; // time
// same values for accelerartion and speed
// simulation with new values
assert_checktrue(execstr("outUser=fmiSimulate(fmu, [0, 0.1, 100], %f)", "errcatch")==0);
// results
assert_checkequal(outUser.Real.result(1).values($), 0); //Cruise Speed
assert_checkequal(outUser.Real.result(2).values($), 120); //Throtle Speed

// Third simulation
// On cruise control
fmu.setValues.input_4_values(1, :) = %t; // values 
fmu.setValues.input_4_values(2, 1) = 10; // time
// set speed
fmu.setValues.input_3_values(1, 1:10) = 150;
fmu.setValues.input_3_values(2, 1:10) = 10:19;
fmu.setValues.input_7_values(1, 1) = %t;
fmu.setValues.input_7_values(2, 1) = 12;
// deceleration
fmu.setValues.input_9_values(1, 1) = %t;
fmu.setValues.input_9_values(2, 1) = 22;
fmu.setValues.input_1_values(1, 11:20) = 90;
fmu.setValues.input_1_values(2, 11:20) = 20:29;
// simulation with new values
assert_checktrue(execstr("outUser=fmiSimulate(fmu, [0, 0.1, 100], %f)", "errcatch")==0);
// results
assert_checkequal(outUser.Real.result(1).values($), 150); //Cruise Speed
assert_checkequal(outUser.Real.result(2).values($), 90); //Throtle Speed


 

