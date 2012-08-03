// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 3520 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3520
//
// <-- Short Description -->
// lex_sort() doesn't work anymore in Scilab 5.

a = [   0.        1608. ; 
    0.        2014.  ;
    0.        2322.  ;
    0.        2332.  ;
    0.        1600.  ;
    10.       2332.  ;
    10.       2322.  ;
    10.       2014.  ;
    10.       1600.  ;
    10.       1608.  ];


[table,idx] = lex_sort(a,"unique"); 

ref_table = [  0.     1600.;
    0.     1608.;
    0.     2014.;
    0.     2322.;
    0.     2332.;
    10.    1600.;
    10.    1608.;
    10.    2014.;
    10.    2322.;
    10.    2332. ];  
    
ref_idx = [   5.    1.    2.    3.    4.    9.    10.    8.    7.    6. ]';

if or(ref_table<>table) then pause,end
if or(ref_idx<>idx) then pause,end
