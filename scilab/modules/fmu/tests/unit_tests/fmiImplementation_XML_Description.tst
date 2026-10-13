//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2014 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// <-- NO CHECK REF -->
// <-- WINDOWS ONLY -->

// fmiImplementation for CoSimulation
path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "Dymola_CSControlledTemperature.fmu"));
assert_checkequal(fmu.fmiType, "cs");
assert_checkequal(typeof(fmu.descriptionFMU.Implementation), "CoSimulation_StandAlone");
assert_checkequal(fmu.descriptionFMU.Implementation.Capabilities("canBeInstantiatedOnlyOncePerProcess"), "true");
assert_checkequal(fmu.descriptionFMU.Implementation.Capabilities.canHandleEvents, "true");
assert_checkequal(fmu.descriptionFMU.Implementation.Capabilities("canHandleVarCommunicStepSize"), "true");
assert_checkequal(fmu.descriptionFMU.Implementation.Capabilities.canInterpolateInputs, []);
assert_checkequal(fmu.descriptionFMU.Implementation.Capabilities("canNotUseMemoryManagementFunctions"), []);
assert_checkequal(fmu.descriptionFMU.Implementation.Capabilities.canRejectSteps, []);
assert_checkequal(fmu.descriptionFMU.Implementation.Capabilities.canRunAsynchronuously, []);
assert_checkequal(fmu.descriptionFMU.Implementation.Capabilities.canSignalEvents, []);
assert_checkequal(fmu.descriptionFMU.Implementation.Capabilities.maxOutputDerivativeOrder, []);
