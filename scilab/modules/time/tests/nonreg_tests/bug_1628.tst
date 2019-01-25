// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 1628 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/1628
//
// <-- Short Description -->
// long programs (several hours) I get a negative value for timer()

// compares result with another tools ;)
// t= cputime; pause(9.9);e=cputime-t

timer();
sleep(9900);
assert_checktrue(timer()>0);

// another test
runs = 50;
for i = 1:runs
    a = abs(rand(1500, 1500, "n")/10);
    b = a';
    a = matrix(b, 750, 3000);
    b = a';
end
assert_checktrue(timer()>0);
