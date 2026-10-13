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
// <-- WINDOWS ONLY -->

// Test the model Variables of XML files from CATIA.

path = fmigetPath();
// CATIA
fmu = importFMU(fullfile(path, "tests", "unit_tests", "CATIA_Modelica_Mechanics_Rotational_Examples_CoupledClutches.fmu"));

// type definitions
typeNames = ["Modelica.Blocks.Interfaces.RealInput";"Modelica.Blocks.Interfaces.RealOutput";...
"Modelica.SIunits.Angle";"Modelica.SIunits.AngularAcceleration";"Modelica.SIunits.AngularVelocity";...
"Modelica.SIunits.Force";"Modelica.SIunits.Frequency";"Modelica.SIunits.Inertia";"Modelica.SIunits.Power";...
"Modelica.SIunits.Time";"Modelica.SIunits.Torque";"StateSelect"];
assert_checkequal(fmu.modelVariables.Type.name, typeNames);
description = "";
for i=1:size(fmu.modelVariables.Type.name,"*")
    assert_checkequal(fmu.modelVariables.Type.description(i), "");
end
quantity = ["";"";"Angle";"AngularAcceleration";"AngularVelocity";"Force";"Frequency";"MomentOfInertia";"Power";"Time";"Torque";""]
assert_checkequal(fmu.modelVariables.Type.quantity, quantity);
unit = ["";"";"rad";"rad/s2";"rad/s";"N";"Hz";"kg.m2";"W";"s";"N.m";""];
assert_checkequal(fmu.modelVariables.Type.unit, unit);

displayUnit = "";
gain = 1;
offset = 0;
relativeQuantity = %f;
nominal = 1;
for i=1:size(fmu.modelVariables.Type.name,"*")
    assert_checkequal(fmu.modelVariables.Type.displayUnit(i), "");
    assert_checkequal(fmu.modelVariables.Type.gain(i), 1);
    assert_checkequal(fmu.modelVariables.Type.offset(i), 0);
    assert_checkequal(fmu.modelVariables.Type.relativeQuantity(i), %f);
    assert_checkequal(fmu.modelVariables.Type.nominal(i), 1);
end

minValues = [-%inf;-%inf;-%inf;-%inf;-%inf;-%inf;-%inf;-%inf;-%inf;-%inf;-%inf;1];
maxValues = [%inf;%inf;%inf;%inf;%inf;%inf;%inf;%inf;%inf;%inf;%inf;5];
items = [0;0;0;0;0;0;0;0;0;0;0;5];
itemsList = ["never","";"avoid","";"default","";"prefer","";"always",""]   
assert_checkequal(fmu.modelVariables.Type.min, minValues);
assert_checkequal(fmu.modelVariables.Type.max, maxValues);
assert_checkequal(fmu.modelVariables.Type.items, items);
assert_checkequal(fmu.modelVariables.Type.itemList(1), itemsList);

// real variables
// test for only the first 10 variables
names = ["freqHz","T2","T3","J1.flange_a.phi","J1.flange_a.tau","J1.flange_b.phi",...
        "J1.flange_b.tau","J1.J","J1.phi","der(J1.phi)"];
refs = [16777216,16777217,16777218,33554432,637534208,33554432,637534209,16777219,33554432,587202560]; 
description = ["Frequency of sine function to invoke clutch1","Time when clutch2 is invoked",...
               "Time when clutch3 is invoked","Absolute rotation angle of flange",...
               "Cut torque in the flange","Absolute rotation angle of flange",...
               "Cut torque in the flange","Moment of inertia","Absolute rotation angle of component",...
               "der(Absolute rotation angle of component)"];
variability = ["parameter","parameter","parameter","continuous","continuous",...
               "continuous","continuous","parameter","continuous","continuous"];
causality = ["internal","internal","internal","internal","internal","internal",...
             "internal","internal","internal","internal"];
alias = ["noAlias","noAlias","noAlias","alias","noAlias","alias",...
         "noAlias","noAlias","noAlias","noAlias"];
declType = ["Modelica.SIunits.Frequency","Modelica.SIunits.Time",...
            "Modelica.SIunits.Time","Modelica.SIunits.Angle","Modelica.SIunits.Torque",...
            "Modelica.SIunits.Angle","Modelica.SIunits.Torque","Modelica.SIunits.Inertia",...
            "Modelica.SIunits.Angle",""];
quantity = ["Frequency","Time","Time","Angle","Torque","Angle","Torque","MomentOfInertia","Angle",""];
unit = ["Hz","s","s","rad","N.m","rad","N.m","kg.m2","rad","rad/s"];
displayUnit = [""];
realtiveQuantity = [%f];
minValue = [-%inf,-%inf,-%inf,-%inf,-%inf,-%inf,-%inf,0,-%inf,-%inf];
maxValue = %inf;
startValue = [0.2,0.4,0.9,%nan,%nan,%nan,%nan,1,0,%nan];
nominalValue = 1;
fixed = %t;
assert_checkequal(fmu.modelVariables.Real.name(1:10), names);
assert_checkequal(fmu.modelVariables.Real.valueReference(1:10), refs);
assert_checkequal(fmu.modelVariables.Real.description(1:10), description);
assert_checkequal(fmu.modelVariables.Real.variability(1:10), variability);
assert_checkequal(fmu.modelVariables.Real.causality(1:10), causality);
assert_checkequal(fmu.modelVariables.Real.alias(1:10), alias);
assert_checkequal(fmu.modelVariables.Real.declaredType(1:10), declType);
assert_checkequal(fmu.modelVariables.Real.quantity(1:10), quantity);
assert_checkequal(fmu.modelVariables.Real.min(1:10), minValue);
assert_checkequal(fmu.modelVariables.Real.start(1:10), startValue);
for i=1:10
    assert_checkequal(fmu.modelVariables.Real.displayUnit(i), displayUnit);
    assert_checkequal(fmu.modelVariables.Real.relativeQuantity(i), relativeQuantity);
    assert_checkequal(fmu.modelVariables.Real.max(i), maxValue);
    assert_checkequal(fmu.modelVariables.Real.nominal(i), nominalValue);
    assert_checkequal(fmu.modelVariables.Real.fixed(i), fixed);
end

// integer variables
// test for only 10 variables
names = ["clutch1.Backward","clutch1.mode","clutch2.Unknown","clutch2.Free",...
         "clutch2.Forward","clutch2.Stuck","clutch2.Backward","clutch2.mode",...
         "clutch3.Unknown","clutch3.Free","clutch3.Forward"];
refs = [100663313,905969682,100663338,100663339,100663340,100663341,...
        100663342,905969711,100663363,100663364,100663365];
description = ["w_rel < 0 (backward sliding)","","Value of mode is not known","Element is not active",...
               "w_rel > 0 (forward sliding)","w_rel = 0 (forward sliding, locked or backward sliding)",...
               "w_rel < 0 (backward sliding)","","Value of mode is not known","Element is not active",...
               "w_rel > 0 (forward sliding)"];
variability = ["constant","discrete","constant","constant","constant","constant",...
               "constant","discrete","constant","constant","constant"];
causality = "internal";
alias = "noAlias";
declType = "";
quantity = "";
minValue = [-2147483648,-1,-2147483648,-2147483648,-2147483648,-2147483648,...
            -2147483648,-1,-2147483648,-2147483648,-2147483648];
maxValue = [2147483647,3,2147483647,2147483647,2147483647,2147483647,...
            2147483647,3,2147483647,2147483647,2147483647];
startValue = [%nan,3,%nan,%nan,%nan,%nan,%nan,3,%nan,%nan,%nan];
fixed = %t;
assert_checkequal(fmu.modelVariables.Integer.name(5:15), names);
assert_checkequal(fmu.modelVariables.Integer.valueReference(5:15), refs);
assert_checkequal(fmu.modelVariables.Integer.description(5:15), description);
assert_checkequal(fmu.modelVariables.Integer.variability(5:15), variability);
assert_checkequal(fmu.modelVariables.Integer.min(5:15), minValue);
assert_checkequal(fmu.modelVariables.Integer.start(5:15), startValue);
for i=1:10
    assert_checkequal(fmu.modelVariables.Integer.causality(i), causality);
    assert_checkequal(fmu.modelVariables.Integer.alias(i), alias);
    assert_checkequal(fmu.modelVariables.Integer.declaredType(i), declType);
    assert_checkequal(fmu.modelVariables.Integer.quantity(i), quantity);
    assert_checkequal(fmu.modelVariables.Integer.fixed(i), fixed);
end

// boolean variables
// test for 8 variables
// test for only 10 variables
names = ["clutch2.startForward","clutch2.startBackward","clutch2.locked","clutch2.useHeatPort",...
         "clutch3.free","clutch3.startForward","clutch3.startBackward","clutch3.locked",...
         "clutch3.useHeatPort"];
refs = [637534247,637534248,637534249,100663346,637534270,637534272,...
        637534273,637534274,100663371];
description = ["true, if w_rel=0 and start of forward sliding","true, if w_rel=0 and start of backward sliding",...
               "true, if w_rel=0 and not sliding","=true, if heatPort is enabled","true, if frictional element is not active",...
               "true, if w_rel=0 and start of forward sliding","true, if w_rel=0 and start of backward sliding",...
               "true, if w_rel=0 and not sliding","=true, if heatPort is enabled"];
variability = ["discrete","discrete","discrete","constant","discrete","discrete",...
               "discrete","discrete","constant"];
causality = "internal";
alias = "noAlias";
declType = "";
startValue = %f;
fixed = [%t,%t,%f,%t,%t,%t,%t,%f,%t];
assert_checkequal(fmu.modelVariables.Boolean.name(8:16), names);
assert_checkequal(fmu.modelVariables.Boolean.valueReference(8:16), refs);
assert_checkequal(fmu.modelVariables.Boolean.description(8:16), description);
assert_checkequal(fmu.modelVariables.Boolean.variability(8:16), variability);
assert_checkequal(fmu.modelVariables.Boolean.fixed(8:16), fixed);
for i=1:8
    assert_checkequal(fmu.modelVariables.Boolean.causality(i), causality);
    assert_checkequal(fmu.modelVariables.Boolean.alias(i), alias);
    assert_checkequal(fmu.modelVariables.Boolean.declaredType(i), declType);
    assert_checkequal(fmu.modelVariables.Boolean.start(i), startValue);
end

// string 
assert_checkequal(fmu.modelVariables.String.name, []);

// enumerations
names = ["J1.stateSelect","clutch1.stateSelect","J2.stateSelect","clutch2.stateSelect",...
         "J3.stateSelect","clutch3.stateSelect","J4.stateSelect"];
refs = [100663298,100663301,100663325,100663330,100663350,100663355,100663376];
description = ["Priority to use phi and w as states","Priority to use phi_rel and w_rel as states",...
               "Priority to use phi and w as states","Priority to use phi_rel and w_rel as states",...
               "Priority to use phi and w as states","Priority to use phi_rel and w_rel as states",...
               "Priority to use phi and w as states"]; 
variability = "constant";
causality = "internal";
alias = "noAlias";
declType = "StateSelect";
quantity = "";
startValue = %nan;
minValue = 1;
maxValue = 5;
fixed = %t;
assert_checkequal(fmu.modelVariables.Enumeration.name, names);
assert_checkequal(fmu.modelVariables.Enumeration.valueReference, refs);
assert_checkequal(fmu.modelVariables.Enumeration.description, description);
for i=1:size(fmu.modelVariables.Enumeration.name, "*")
    assert_checkequal(fmu.modelVariables.Enumeration.variability(i), variability);
    assert_checkequal(fmu.modelVariables.Enumeration.causality(i), causality);
    assert_checkequal(fmu.modelVariables.Enumeration.alias(i), alias);
    assert_checkequal(fmu.modelVariables.Enumeration.declaredType(i), declType);
    assert_checkequal(fmu.modelVariables.Enumeration.start(i), startValue);
    assert_checkequal(fmu.modelVariables.Enumeration.min(i), minValue);
    assert_checkequal(fmu.modelVariables.Enumeration.max(i), maxValue);
    assert_checkequal(fmu.modelVariables.Enumeration.fixed(i), fixed);
end
