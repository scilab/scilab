// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 6938 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6938
//
// <-- Short Description -->
// frexp returned a wrong message error with a complex as input argument.

[f, e] = frexp([1, %pi, -3, %eps]);
if or(size(e) <> [1 4]) then pause, end
if or(size(f) <> [1 4]) then pause, end
if or(e <> [1.    2.    2.  -51.]) then pause, end

if execstr("[f, e] = frexp();", "errcatch") <> 77 then pause, end
if execstr("[f] = frexp(%pi);", "errcatch") <> 78 then pause, end
if execstr("[f, e] = frexp(""a"");", "errcatch") <> 999 then pause, end

if execstr("[f, e] = frexp(%i);", "errcatch") <> 999 then pause, end
if lasterror() <> msprintf(_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "frexp", 1) then pause, end
