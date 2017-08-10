//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2707 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2707
//
// <-- Short Description -->
// The getmd5 function does not handle the SCI/ ~/ ...  shortcuts

// test 1
if execstr("getmd5(""SCI/modules/core/etc/core.start"");","errcatch") <> 0 then pause, end

// test 2
mputl(" ",TMPDIR+"/bug_2707.txt");
if execstr("getmd5(""TMPDIR/bug_2707.txt"");","errcatch") <> 0 then pause, end
