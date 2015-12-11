// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Integer/summation.zcos"));

A_ref = int16([-4 -4 ; -5 -6]);

try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end

for i = 1:128
    assert_checkequal(A.values(:,:,i), A_ref);
end
