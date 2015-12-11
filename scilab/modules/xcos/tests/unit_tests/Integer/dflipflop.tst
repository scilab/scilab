// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->

// Import diagram
assert_checktrue(importXcosDiagram("SCI/modules/xcos/tests/unit_tests/Integer/dflipflop.zcos"));

// Redefining messagebox() to avoid popup from c_pass2.sci
prot = funcprot();
funcprot(0);
function messagebox(msg, modal, msg_title)
    disp(msg);
endfunction
funcprot(prot);

A_ref = int8(zeros(128,1));

try scicos_simulate(scs_m, 'nw'); catch disp(lasterror()); end

assert_checkequal(A.values, A_ref);
