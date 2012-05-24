// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 10626 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=10626
//
// <-- Short Description -->
// taucs_chdel(),  umf_ludel() returned an error

A = sparse( [ 2  3  0  0  0;
              3  0  4  0  6; 
              0 -1 -3  2  0; 
              0  0  1  0  0; 
              0  4  2  0  1] );
b = [8 ; 45; -3; 3; 19];
Lup = umf_lufact(A);
umf_ludel();
assert_checkequal(lasterror(), []);

A = sparse( [ 2 -1  0  0  0;
             -1  2 -1  0  0; 
              0 -1  2 -1  0; 
              0  0 -1  2 -1; 
              0  0  0 -1  2] );
b = [0 ; 0; 0; 0; 6];
Cp = taucs_chfact(A);
taucs_chdel();
assert_checkequal(lasterror(), []);
