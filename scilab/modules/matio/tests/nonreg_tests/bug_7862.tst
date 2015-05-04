// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 7862 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7862
//
// <-- Short Description -->
// "savematfile" only saved the first string in the vector (or matrix) when variable is a vector (or matrix) of strings.

a0x0 = "";
savematfile(TMPDIR + filesep() + "a0x0.mat", "a0x0");
a0x0_saved = a0x0;
clear a0x0
loadmatfile(TMPDIR + filesep() + "a0x0.mat");
if or(a0x0<>a0x0_saved) then pause; end
clear a0x0 a0x0_saved

a1x1 = "abcde";
savematfile(TMPDIR + filesep() + "a1x1.mat", "a1x1");
a1x1_saved = a1x1;
clear a1x1
loadmatfile(TMPDIR + filesep() + "a1x1.mat");
if or(a1x1<>a1x1_saved) then pause; end
clear a1x1 a1x1_saved

a1x3 = ["aaaa", "bbbb", "cccc"];
ierr = execstr("savematfile(TMPDIR + filesep() + ""a1x3.mat"", ""a1x3"");", "errcatch");
msg = lasterror();
if ierr<>999 | msg<>msprintf(_("%s: Row array of strings saving is not implemented.\n"), "GetCharMatVar") then pause;end

a3x1 = ["aaaa"; "bbbb"; "cccc"];
savematfile(TMPDIR + filesep() + "a3x1.mat", "a3x1");
a3x1_saved = a3x1;
clear a3x1
loadmatfile(TMPDIR + filesep() + "a3x1.mat");
if or(a3x1<>a3x1_saved) then pause; end
clear a3x1 a3x1_saved

a3x1 = ["aa"; "bbb"; "cccc"];
ierr = execstr("savematfile(TMPDIR + filesep() + ""a3x1.mat"", ""a3x1"");", "errcatch");
msg = lasterror();
if ierr<>999 | msg<>msprintf(_("%s: Column array of strings with different lengths saving is not implemented.\n"), "GetCharMatVar") then pause;end

a2x3 = ["aaaa", "bbbb", "cccc"; "dddd", "eeee", "ffff"];
ierr = execstr("savematfile(TMPDIR + filesep() + ""a2x3.mat"", ""a2x3"");", "errcatch");
msg = lasterror();
if ierr<>999 | msg<>msprintf(_("%s: 2D array of strings saving is not implemented.\n"), "GetCharMatVar") then pause;end
