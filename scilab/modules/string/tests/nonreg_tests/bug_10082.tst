// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- Non-regression test for bug 10082 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/10082
//
// <-- Short Description -->
// string(complex) did not remove the leading space replacing "+" for real()>0
assert_checkequal(string(1+%i), "1+%i");
assert_checkequal(string(30-%i), "30-%i");
assert_checkequal(string(2.3-2*%i), "2.3-%i*2");
assert_checkequal(string(complex(2, %nan)), "2+%i*Nan");
assert_checkequal(string(complex(%inf, 21)), "Inf+%i*21");
assert_checkequal(string(complex(-%nan, -%inf)), "Nan-%i*Inf");
