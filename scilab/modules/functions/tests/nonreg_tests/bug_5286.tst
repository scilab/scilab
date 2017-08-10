// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5286 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5286
//
// <-- Short Description -->
// calling the 'lib' function with the wrong path makes scilab hangs on Mandriva 64
// =============================================================================
ierr = execstr('lib(''test'');','errcatch');
if ierr == 0 then pause,end
// =============================================================================
