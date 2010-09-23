// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 7688 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7688
//
// <-- Short Description -->
// set("color_map", hsvcolormap(128)) produced a EXCEPTION_STACK_OVERFLOW 

if execstr("set(""color_map"", hsvcolormap(128))", "errcatch") <> 999 then pause, end

