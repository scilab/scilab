// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 366 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=366
//
// <-- Short Description -->
// The error message of plot2d is sort of understandable

// should complain about strf (normally a string of length 3).
ierr = execstr('plot2d(0,0,0,''0'')','errcatch');
if ierr <> 999 then pause,end
if lasterror() <> 'plot2d: Wrong size for input argument #4: String of 3 characters expected.' then pause,end


