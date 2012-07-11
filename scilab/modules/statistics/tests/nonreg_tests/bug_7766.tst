// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
//
//
// <-- Non-regression test for bug 7766 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7744
//
// <-- Short Description -->
// cdff, cdffnc, ... functions did not display %inf in error messages

if execstr("cdff(""Dfn"", 7, .99, .01, 7.460435)", "errcatch") <> 999 then pause, end;
r = lasterror();
ref = msprintf(_("cdff: Answer appears to be higher than greatest search bound %s\n"), "%inf");
if r <> ref then pause,end

if execstr("cdffnc(""Dfd"", 1.5, 0.3945792, 0.6054208, 1, 5)",  "errcatch") <> 999 then pause, end; 
r = lasterror();
ref = msprintf(_("cdffnc: Answer appears to be higher than greatest search bound %s\n"), "%inf");
if r <> ref then pause,end
