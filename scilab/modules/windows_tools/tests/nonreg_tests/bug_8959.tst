// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8959 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8959
//
// <-- Short Description -->
// winqueryreg returned an incorrect value if the value name was false.

ierr = execstr("r = winqueryreg(""HKEY_LOCAL_MACHINE"", ""Software\Microsoft\Microsoft SDKs\Windows"", ""foo"")", "errcatch");
if ierr <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Cannot query value of this type.\n"), "winqueryreg") then pause, end
