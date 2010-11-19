// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 8079 -->
//
// <-- INTERACTIVE TEST -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8177
//
// <-- Short Description -->
// format mode was modified by some macros and not restored


ft1 = format();
exec("SCI/contrib/toolbox_skeleton/builder.sce");
ft2 = format();
if ~and(ft1 == ft2) then pause, end

ft3 = format();
W = ssrand(1,1,6);
r = trfmod(W, "p");
ft4 = format();
if ~and(ft3 == ft4) then pause, end
