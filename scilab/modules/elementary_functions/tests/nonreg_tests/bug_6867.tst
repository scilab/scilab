// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6867 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6867
//
// <-- Short Description -->
// The error message when base2dec was called with an invalid base representation
// was wrong

execstr("base2dec(""FFFF"",13);","errcatch");
if lasterror() <> msprintf(gettext("%s: Wrong value for input argument #%d: Valid base %d representations expected.\n"),"base2dec",1,13) then pause, end

execstr("base2dec(""FFFF"",14);","errcatch");
if lasterror() <> msprintf(gettext("%s: Wrong value for input argument #%d: Valid base %d representations expected.\n"),"base2dec",1,14) then pause, end
