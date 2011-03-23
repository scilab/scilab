// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//
// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5308 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5308
//
// <-- Short Description -->
// editor does not check input arguments type

// Interactive since it could have some modal messagebox

ierr = execstr('editor([], [])','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('editor([])','errcatch');
if ierr <> 999 then pause,end

ierr = execstr('editor(''fff'',''fff'')','errcatch');
if ierr <> 0 then pause,end



