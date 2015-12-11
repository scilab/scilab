// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Integer/jkflipflop.zcos"));

A_ref1 = int8(ones(8,1));
A_ref2 = int8(zeros(20,1));
A_ref3 = int8(ones(20,1));

try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end

assert_checkequal(A.values(1:8),   A_ref1);
assert_checkequal(A.values(9:28),  A_ref2);
assert_checkequal(A.values(29:48), A_ref3);
assert_checkequal(A.values(49:68), A_ref2);
