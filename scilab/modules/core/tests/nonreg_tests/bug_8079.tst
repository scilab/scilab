// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8079 -->
//
// <-- JVM NOT MANDATORY -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8079
//
// <-- Short Description -->
// When the max number of variable available has been reached, it was not possible to leave Scilab.

if execstr("for i=1:100000,execstr(''a'' + string(i) + ''= 3;''),end", "errcatch") <> 18 then pause,end;


