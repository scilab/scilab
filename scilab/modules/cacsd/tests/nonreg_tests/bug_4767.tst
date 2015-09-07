//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4767 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4767
//
// <-- Short Description -->
// minreal.sci contains a couple of references to the variable 'T', which is not defined in the function

A = [-eye(2,2),rand(2,2);zeros(2,2),-2*eye(2,2)];
B = [rand(2,2);zeros(2,2)];
C = rand(2,4);
sl = syslin('c',A,B,C);
ierr = execstr('slb = minreal(sl);','errcatch');
if ierr <> 0 then pause,end

