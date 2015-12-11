//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1198 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1198
//
// <-- Short Description -->
//    For the latest version of Scilab scilab-3.0-u-20050128 on a
//    Windows 2000 platform.
//
//    The program crashes instead of reporting a syntax error.

G   = syslin("c", [1 0 ; 0 1], [ 1; 1], [1 1]);
err = execstr("minss(G)(""A"");","errcatch");
if err <> 0 then pause,end
