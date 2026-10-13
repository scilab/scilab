//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// <-- NO CHECK REF -->

// FMI 2.0 declares its own value-reference type, so the FMI 1.0 typemaps
// did not match its getters: the value array stayed an input, only the
// status came back, and the reference count had to be passed by hand.

path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "Reference-FMUs", "2.0", "BouncingBall.fmu"));
h = 1; g = 5; e = 6; gStart = -9.81; gSet = -9;
library = fmu_link(fmu.libraryFile, fmu.descriptionFMU.modelIdentifier);

instance = fmu_call(library, "fmi2Instantiate", ..
                    fmu.descriptionFMU.modelIdentifier, 1, ..
                    fmu.descriptionFMU.guid, "", %f, %f);
assert_checkequal(fmu_call(library, "fmi2SetupExperiment", instance, %f, 0, 0, %f, 0), 0);
assert_checkequal(fmu_call(library, "fmi2EnterInitializationMode", instance), 0);
assert_checkequal(fmu_call(library, "fmi2ExitInitializationMode", instance), 0);

// h, g and e, read in a single call, against their declared start values.
[status, values] = fmu_call(library, "fmi2GetReal", instance, [h g e]);
assert_checkequal(status, 0);
assert_checkequal(values, [1 gStart 0.7]);

// The count comes from the reference array, so no setter takes one either.
assert_checkequal(fmu_call(library, "fmi2SetReal", instance, [g e], [gSet 0.5]), 0);
[status, values] = fmu_call(library, "fmi2GetReal", instance, [g e]);
assert_checkequal(values, [gSet 0.5]);

fmu_call(library, "fmi2FreeInstance", instance);
