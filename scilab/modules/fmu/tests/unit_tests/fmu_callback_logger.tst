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
//
// <-- NO CHECK REF -->

//Test of fmu_callback_logger and fmiInstantiateModel
//Only if we have an error fmu_callback_logger will be called.

path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "SCADE_CruiseControl__CruiseControl_FMU.fmu"));

// c functions
// all ok => null message of callback
library = fmu_link(fmu.libraryFile, fmu.descriptionFMU.modelIdentifier);
instance = fmu_call(library, "fmiInstantiateModel", fmu.descriptionFMU.modelIdentifier, fmu.descriptionFMU.guid);

//Not ok => message of callback
errmsg = msprintf(_("%s: %s model instantiation failed.\n"),"FmuModelExchange_fmiInstantiateModel", fmu.descriptionFMU.modelIdentifier);
assert_checkerror("fmu_call(library, ""fmiInstantiateModel"", fmu.descriptionFMU.modelIdentifier, ""TOTO"")",errmsg, 999);
// fmu_ulink has been done in gateway.

library = fmu_link(fmu.libraryFile, fmu.descriptionFMU.modelIdentifier);
instance = fmu_call(library, "fmiInstantiateModel", fmu.descriptionFMU.modelIdentifier, fmu.descriptionFMU.guid);
status=fmu_call(library, "fmiSetReal",instance,[1073741824,1073741825,1073741826],[10,15,25]);
assert_checkequal(status,3); // Illegal value reference fmiStatus = 3;
status=fmu_call(library, "fmiSetInteger",instance,[731824,1041825,141826],[1,1,2]);
assert_checkequal(status,3); // Illegal value reference fmiStatus = 3;
status=fmu_call(library, "fmiSetBoolean",instance,[1824,101825,14826],[%t,%t,%t]);
assert_checkequal(status,3);
status=fmu_call(library, "fmiSetString",instance,[184,10825,1426],["t","t","t"]);
assert_checkequal(status,3);

[status,values]=fmu_call(library, "fmiGetReal",instance,[1073741824,1073741825,1073741826]);
assert_checkequal(status,3); // Illegal value reference fmiStatus = 3;
[status,values]=fmu_call(library, "fmiGetInteger",instance,[731824,1041825,141826]);
assert_checkequal(status,3); // Illegal value reference fmiStatus = 3;
[status,values]=fmu_call(library, "fmiGetBoolean",instance,[1824,101825,14826]);
assert_checkequal(status,3);
// get string not supported

[status]=fmu_call(library, "fmiSetDebugLogging",instance,%f);
assert_checkequal(status,0);
[status]=fmu_call(library, "fmiSetDebugLogging",instance,%t);
assert_checkequal(status,0);
status=fmu_call(library, "fmiSetReal",instance,[1,2],[1,1]);
assert_checkequal(status,0);
status=fmu_call(library, "fmiSetInteger",instance,0,1);
assert_checkequal(status,0);
