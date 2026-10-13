//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2026 - Dassault Systèmes S.E - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
// <-- NO CHECK REF -->

// A modelDescription.xml may carry XML comments between its variables;
// Dymola (for example) writes one before every ScalarVariable.

path = fmigetPath();
plain    = importFMU(fullfile(path, "tests", "unit_tests", "CruiseControl__CruiseControl_FMU.fmu"));
commented = importFMU(fullfile(path, "tests", "unit_tests", "CruiseControlWithXmlComments.fmu"));

a = fmiModelVariables(plain);
b = fmiModelVariables(commented);

// The comments must be invisible to the reader.
assert_checkequal(b.Real.name, a.Real.name);
assert_checkequal(b.Real.valueReference, a.Real.valueReference);
assert_checkequal(b.Real.causality, a.Real.causality);
assert_checkequal(b.Real.variability, a.Real.variability);

assert_checkequal(b.Integer.name, a.Integer.name);
assert_checkequal(b.Integer.valueReference, a.Integer.valueReference);

assert_checkequal(b.Boolean.name, a.Boolean.name);
assert_checkequal(b.Boolean.valueReference, a.Boolean.valueReference);

// The count is what the defect got wrong, so assert it on its own.
assert_checkequal(size(b.Real.name, "*"), size(a.Real.name, "*"));
assert_checkequal(size(b.Integer.name, "*"), size(a.Integer.name, "*"));
