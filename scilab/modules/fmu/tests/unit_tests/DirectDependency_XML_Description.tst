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

//Test of Direct Dependency 

path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "invalidFMU", "DirectDependencyFakeFMU.fmu"));
// size
assert_checkequal(fmu.descriptionFMU.DirectDependency, 3);

// only for causality == "output"
assert_checkequal(fmu.modelVariables.Real.DirectDependency(1), "");
assert_checkequal(fmu.modelVariables.Real.DirectDependency(2), "");
// find outputs
index = find(fmu.modelVariables.Real.causality == "output");
// names of outputs
assert_checkequal(fmu.modelVariables.Real.name(index), ["a3", "a4", "a6"]); 

// a3
dep = ["dependency_a3_1";"dependency_a3_2";"dependency_a3_3";"dependency_a3_4"];
assert_checkequal(fmu.modelVariables.Real.DirectDependency(3)(:), dep);
// a4
dep = ["dependency_a4_1";"dependency_a4_2"];
assert_checkequal(fmu.modelVariables.Real.DirectDependency(4)(:), dep);

assert_checkequal(fmu.modelVariables.Real.DirectDependency(5), "");
// a5
dep = ["dependency_a6_1"];
assert_checkequal(fmu.modelVariables.Real.DirectDependency(6)(:), dep);
