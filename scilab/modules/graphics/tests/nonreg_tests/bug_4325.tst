// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4325 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4325
//
// <-- Short Description -->
// set("auto_clear","off") crashes Scilab.
// 

if execstr("set(""auto_clear"",""off"")", "errcatch") <> 999 then pause, end
// should not crash Scilab.
