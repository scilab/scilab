//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2020 - ESI Group - Clement DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// <-- NO CHECK REF -->

// unit test for loadFMU

path = fmigetPath();
filepath = fullfile(path, "tests", "unit_tests", "SCADE_CruiseControl__CruiseControl_FMU.fmu");

[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(filepath);

assert_checktrue(isdir(workdir));
assert_checktrue(typeof(xml) == "XMLDoc");
assert_checktrue(typeof(libname) == "string");
assert_checkequal(modelIdentifier, "CruiseControl__CruiseControl_FMU");
assert_checkequal(fmuImpl, "me 1.0");

// simulate multiple calls
[workdir, xml, libname] = loadFMU(filepath);
assert_checktrue(isdir(workdir));
assert_checktrue(typeof(xml) == "XMLDoc");
assert_checktrue(typeof(libname) == "string");

[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(filepath, workdir);
assert_checktrue(isdir(workdir));
assert_checktrue(typeof(xml) == "XMLDoc");
assert_checktrue(typeof(libname) == "string");
assert_checkequal(modelIdentifier, "CruiseControl__CruiseControl_FMU");
assert_checkequal(fmuImpl, "me 1.0");
