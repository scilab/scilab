// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15741 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=15741
//
// <-- Short Description -->
// Operations between 2 polynomials with distinct variables no longer call the corresponding overloads (Regression)

assert_checkerror("%s+%z",["Undefined operation for the given operands.";"check or define function %p_a_p for overloading."])
assert_checkerror("%s-%z",["Undefined operation for the given operands.";"check or define function %p_s_p for overloading."])
assert_checkerror("%s*%z",["Undefined operation for the given operands.";"check or define function %p_m_p for overloading."])
