//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge.Steer@inria.fr
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3880 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3880
//
// <-- Short Description -->
//    division by zero occurs in g_margin with Scilab v5.0.3
//
s=%s/(2*%pi);
g=20*(s+1)/(s*(s+5)*(s^2+2*s+10));
gs=syslin("c",g);
if execstr("[gg,wcp]=g_margin(gs)","errcatch")<>0 then pause,end

