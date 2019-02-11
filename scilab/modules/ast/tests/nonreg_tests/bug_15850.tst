// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 15850 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15850
//
// <-- Short Description -->
// ['x' +'x'] and ['x'+ 'x'] are ['x' 'x'] , while ['x'+'x'] and ['x' + 'x'] are ['xx'] ..

assert_checkerror("+''x''",["Undefined operation for the given operands.";"check or define function %c_a for overloading."]);
assert_checkerror("[''x'',+''x'']",["Undefined operation for the given operands.";"check or define function %c_a for overloading."]);
assert_checkerror("[''x'' +''x'']",["Undefined operation for the given operands.";"check or define function %c_a for overloading."]);
assert_checkequal(['x'+'x'],['xx'])
assert_checkequal(['x' + 'x'],['xx'])