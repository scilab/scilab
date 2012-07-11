// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 4663 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4663
//
// <-- Short Description -->
//
// the call to the timer function prints an error message when the elapsed time between 2 calls is really small.

ierr = execstr('timer();timer();timer();','errcatch');
if ierr <> 0 then pause,end

