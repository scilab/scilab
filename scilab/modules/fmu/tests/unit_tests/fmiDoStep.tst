// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2026 - Dassault Systèmes - Clément DAVID
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
// <-- NO CHECK REF -->
// <-- WINDOWS ONLY -->

// FMI 1.0 co-simulation from the console, the FMU also exports the model exchange functions
path = fmigetPath();
model = importFMU(fullfile(path, "tests", "unit_tests", "Dymola_CSControlledTemperature.fmu"));
assert_checkequal(model.fmiType, "cs");

inst = fmiInstantiateSlave(model, 5, %f);
assert_checkequal(typeof(inst), "FMUinst");
assert_checkfalse(inst.isInitialized);

errmsg = msprintf(_("%s: Model %s is not initialized.\n"), "fmiDoStep", inst.instanceName);
assert_checkerror("fmiDoStep(inst, 0, 0.1)", errmsg);

inst = fmiInitializeSlave(inst, 0, 1);
assert_checktrue(inst.isInitialized);

r = model.modelVariables.Real;
TAmb = r.valueReference(r.name == "TAmb");
for t = 0:0.1:0.9
    assert_checkequal(fmiDoStep(inst, t, 0.1, %t), 0);
end
assert_checkalmostequal(fmiGetReal(inst, TAmb), 293.15);

fmiTerminateSlave(inst);
fmiFreeSlaveInstance(inst);
