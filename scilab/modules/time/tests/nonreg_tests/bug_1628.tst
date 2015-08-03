// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 1628 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1628
//
// <-- Short Description -->
// long programs (several hours) I get a negative value for timer()

// compares result with another tools ;)
// t= cputime; pause(9.9);e=cputime-t

timer();sleep(9900);timer()

// another test
runs = 50;
cumulate = 0; a = 0; b = 0;
for i = 1:runs
    timer();
    a = abs(rand(1500, 1500, "n")/10);
    b = a';
    a = matrix(b, 750, 3000);
    b = a';
    timing = timer();
    cumulate = cumulate + timing;
end;
timing = cumulate/runs;
times(1, 1) = timing;
disp("Creation, transp., deformation of a 1500x1500 matrix (sec): " + string(timing));
clear("a"); clear("b");


