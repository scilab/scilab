// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Integer/dlatch.zcos"));

A_ref1 = int8(ones(7,1));
A_ref2 = int8(zeros(24,1));
A_ref3 = int8(ones(15,1));
A_ref4 = int8(zeros(26,1));

// Redefining messagebox() to avoid popup from c_pass2.sci
prot = funcprot();
funcprot(0);
function messagebox(msg, modal, msg_title)
    disp(msg);
endfunction
funcprot(prot);

try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end

assert_checkequal(A.values(1:7),   A_ref1);
assert_checkequal(A.values(8:31),  A_ref2);
assert_checkequal(A.values(32:46), A_ref3);
assert_checkequal(A.values(47:72), A_ref4);
