// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- JVM NOT MANDATORY -->
//
// <-- Non-regression test for bug 8300 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8300
//
// <-- Short Description -->
// a(48400, 48400) = 0 crashed scilab
if execstr("b((2^32-1)/3,3) = 10;", "errcatch") == 0 then pause,end
clear b;
if execstr("a(48400, 48400) = 0;", "errcatch") == 0 then pause,end
X = 10;
if execstr("a(48400, 48400) = X;", "errcatch") == 0 then pause,end
clear X;
stacksize('max');
if execstr("b((2^32-1)/3,3) = 0;", "errcatch") == 0 then pause,end
clear b;
