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

// FMI 2.0 takes its callbacks by pointer and their members are const, so
// no Scilab value can build them. They are supplied by a typemap; without
// it fmi2Instantiate expects an eighth argument nothing can provide.

path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "Reference-FMUs", "2.0", "BouncingBall.fmu"));
assert_checkequal(fmu.descriptionFMU.fmiVersion, "2.0");

library = fmu_link(fmu.libraryFile, fmu.descriptionFMU.modelIdentifier);
assert_checkequal(fmu_call(library, "fmi2GetVersion"), "2.0");

// fmi2CoSimulation is 1 in the fmi2Type enumeration.
instance = fmu_call(library, "fmi2Instantiate", ..
                    fmu.descriptionFMU.modelIdentifier, 1, ..
                    fmu.descriptionFMU.guid, "", %f, %f);
assert_checkequal(typeof(instance), "pointer");

fmu_call(library, "fmi2FreeInstance", instance);
