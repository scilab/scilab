// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 16197 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=16197
//
// <-- Short Description -->
// Extraction of real components of complex matrices is counterintuitive

p=poly([1 1 1 1],"x","coeff"); 
r=roots(p);

assert_checktrue(isreal(r(1)) | isreal(r(2)) | isreal(r(3)))

x = [1+%i 1 2 3 1-%i];

assert_checktrue(isreal(x(2:4)));