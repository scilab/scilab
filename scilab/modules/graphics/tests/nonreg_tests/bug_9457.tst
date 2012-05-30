// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 9457 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9457
//
// <-- Short Description -->
// set("figure_style", "new") returned a error message

ierr = execstr("set(""figure_style"", ""new"")", "errcatch");
if ierr <> 0 then pause, end

ierr = execstr("set(""figure_style"", ""old"")", "errcatch");
if ierr <> 999 then pause, end
if lasterror() <> msprintf(gettext("Old graphic mode is no longer available. Please refer to the set help page.\n")) then pause, end

