// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 9159 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9159
//
// <-- Short Description -->
// grand(1,3,'uin',1,0) crashed scilab.

if execstr("grand(1,3,''uin'',1,0)", "errcatch") <> 999 then pause, end
if execstr("grand(1,3,''unf'',1,0)", "errcatch") <> 999 then pause, end
