// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 11933 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/11933
//
// <-- Short Description -->
// For any polynomial p -- noticeably in s or z --, evaluating sci2exp(p)
// failed generating p.


p = (2*%i-%z)^4;
p = [p p p ; p p p];
s = sci2exp(p);
assert_checkequal(evstr(s), p);

p = (3-%s*%i)^11;
p = [p p p ; p p p];
s = sci2exp(p);
assert_checkequal(evstr(s), p);

abc = poly(0,"abc");
p = (4*%i-3*abc)^4;
p = [p p p ; p p p];
s = sci2exp(p);
assert_checkequal(evstr(s), p);
