// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 6586 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6586
//
// <-- Short Description -->
// getmd5 crashed if the file (1st input argument) didn't exist

if execstr("getmd5(''"+TMPDIR+"/phantom_file'')","errcatch") == 0 then pause, end
