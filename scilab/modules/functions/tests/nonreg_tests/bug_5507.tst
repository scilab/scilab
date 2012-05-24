// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5507 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5507
//
// <-- Short Description -->
// whereis(bin2dec) returned a error
// =============================================================================
ierr = execstr('a = whereis(bin2dec);','errcatch');
if ierr <> 0 then pause,end
// =============================================================================
