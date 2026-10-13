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

//Test of all variables of fmu Cruise Control 

path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "SCADE_CruiseControl__CruiseControl_FMU.fmu"));

//get all info for Model Variables
info = fmiModelVariables(fmu);

//Real Variables
names = ["Accel" "Brake" "Speed" "CruiseSpeed" "ThrottleCmd"];
refs = 0:4;
descriptions = ["Accel" "Brake" "Speed" "CruiseSpeed" "ThrottleCmd"];
variability = "continuous" + emptystr(refs);
causality = ["input" "input" "input" "output" "output"];
alias = "noAlias" + emptystr(refs);
declaredType = emptystr(refs);
quantity = emptystr(refs);
unit = emptystr(refs);
displayUnit = emptystr(refs);
relativeQuantity = 1 == zeros(refs);
varMin = -%inf + zeros(refs);
varMax = %inf + zeros(refs);
nominal = ones(refs);
start = zeros(refs);
fixed = 0 == zeros(refs);

assert_checkequal(info.Real.name, names);
assert_checkequal(info.Real.valueReference, refs);
assert_checkequal(info.Real.description, descriptions);
assert_checkequal(info.Real.variability, variability);
assert_checkequal(info.Real.causality, causality);
assert_checkequal(info.Real.alias, alias);
assert_checkequal(info.Real.declaredType, declaredType);
assert_checkequal(info.Real.quantity, quantity);
assert_checkequal(info.Real.unit, unit);
assert_checkequal(info.Real.displayUnit, displayUnit);
assert_checkequal(info.Real.relativeQuantity, relativeQuantity);
assert_checkequal(info.Real.min, varMin);
assert_checkequal(info.Real.max, varMax);
assert_checkequal(info.Real.nominal, nominal);
assert_checkequal(info.Real.start, start);
assert_checkequal(info.Real.fixed, fixed);

//Integer variables
assert_checkequal(info.Integer.name, "CruiseState");
assert_checkequal(info.Integer.valueReference, 0);
assert_checkequal(info.Integer.description, "CruiseState");
assert_checkequal(info.Integer.variability, "discrete");
assert_checkequal(info.Integer.causality, "output");
assert_checkequal(info.Integer.alias, "noAlias");
assert_checkequal(info.Integer.declaredType, "");
assert_checkequal(info.Integer.quantity, "");
assert_checkequal(info.Integer.min, -2**31);
assert_checkequal(info.Integer.max, 2**31-1);
assert_checkequal(info.Integer.start, 0);
assert_checkequal(info.Integer.fixed, %t);

//Boolean variables
names = ["On" "Off" "Resume" "Set" "QuickAccel" "QuickDecel"];
refs = 0:5;
descriptions = ["On" "Off" "Resume" "Set" "QuickAccel" "QuickDecel"];
variability = "discrete" + emptystr(refs);
causality = "input" + emptystr(refs);
alias = "noAlias" + emptystr(refs);
declaredType = emptystr(refs);
start = 1 == zeros(refs);
fixed = 0 == zeros(refs);

assert_checkequal(info.Boolean.name, names);
assert_checkequal(info.Boolean.valueReference, refs);
assert_checkequal(info.Boolean.description, descriptions);
assert_checkequal(info.Boolean.variability, variability);
assert_checkequal(info.Boolean.causality, causality);
assert_checkequal(info.Boolean.alias, alias);
assert_checkequal(info.Boolean.declaredType, declaredType);
assert_checkequal(info.Boolean.start, start);
assert_checkequal(info.Boolean.fixed, fixed);

//String variables
assert_checkequal(length(info.String.name), 0);
//Enumerations
assert_checkequal(length(info.Enumeration.name), 0);
//Type definitions 
assert_checkequal(length(info.Type.name), 0);

