// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->
//
// <-- Non-regression test for bug 9211 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9211
//
// <-- Short Description -->
// set("userdata", list()) crashed Scilab.

if execstr("set(""userdata"", list())", "errcatch") <> 999 then pause, end
if execstr("set(""userdata"", ""titi"")", "errcatch") <> 999 then pause, end

if execstr("set(""default_values"", 1)", "errcatch") <> 0 then pause, end

f = scf(1000);
if execstr("set(""current_entity"",f)", "errcatch") <> 0 then pause, end
if execstr("set(""hdl"",f)", "errcatch") <> 0 then pause, end
if execstr("set(""current_figure"", f)", "errcatch") <> 0 then pause, end

curaxe = gca();
if execstr("set(""current_axes"", curaxe)", "errcatch") <> 0 then pause, end