// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8768 -->
//
// <-- JVM NOT MANDATORY -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8768
//
// <-- Short Description -->
// GetRhsVar(..., STRING_DATATYPE did not check size of input argument


if execstr("with_module([""ffa"", ""ff""])", "errcatch") <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"), "with_module", 1) then pause, end

if execstr("clearfun([""myfuncA"", ""myfuncB""])", "errcatch") <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"), "clearfun", 1) then pause, end

if execstr("format([""e"", ""v""])", "errcatch") <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Wrong size for input argument #%d: A string expected.\n"), "format", 1) then pause, end
