// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2021 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16623 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16483
//
// <-- Short Description -->
// rand(2,2,2) ^ 2  now yields a wrong result instead of trying to call the %s_p_s overload for input hypermatrices

assert_checkerror("rand(2,2,2)^2",_(["Undefined operation for the given operands.";"check or define function %s_t for overloading."]"))
