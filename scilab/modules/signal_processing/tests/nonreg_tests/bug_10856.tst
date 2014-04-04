// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 10856 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10856
//
// <-- Short Description -->
// analpf did not return the right result.

fr=10:10:1000000;
for i = 3:6
    hs=analpf(i,'butt',[.1 0],2800);
    // Check the degree of denominator
    assert_checkequal(degree(hs(3)),i);
    // Check that numerator degree is 0
    assert_checkequal(degree(hs(2)),0);
    // Check that first term of denominator is equal to numerator
    assert_checkalmostequal(horner(hs(3),0),horner(hs(2),0));
    // At low frequencies, amplitudes are almost 0
    assert_checktrue(horner(hs,5)<1);
    // Check that the slope at high frequencies has the right value
    assert_checktrue(20*log10(horner(hs,10e7))-20*log10(horner(hs,10e6))-20*i<0.1);
end
