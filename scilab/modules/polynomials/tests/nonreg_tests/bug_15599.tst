// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15599 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15599
//
// <-- Short Description -->
// degree of zero polynomial should be -Inf instead of 0

p=poly(0,'x','coeff');
q=poly(1,'x','coeff');
assert_checkequal(degree(p),-%inf);
assert_checkequal(degree([p q q-q 0*%s 0*%i*%s]),[-%inf 0 -%inf -%inf -%inf]);
assert_checkequal(degree(0),-%inf);
assert_checkequal(degree([0 0*%i 1]),[-%inf -%inf 0]);
assert_checkequal(degree([%nan %nan+%s %nan+0*%s]),[0 1 0]);
