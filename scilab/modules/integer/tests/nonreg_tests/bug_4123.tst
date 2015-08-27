//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 41237 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4123
//
// <-- Short Description -->
// %i_prod(int32([0 0])) crashs in release mode with F2C on windows

A = int32([0 0]);
ierr = execstr('B=prod(A);','errcatch');
if ierr <> 0 then pause,end

if B <> 0 then pause,end
