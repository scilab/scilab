// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 6814 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6814
//
// <-- Short Description -->
// The exported links points (link.xx and link.yy) must be column vectors.
//

result = importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_6814.zcos");
if result <> %t then pause,end

// checking the link
currentLink = scs_m.objs(3);
if typeof(currentLink) <> "Link" then pause, end

// checking the points
xxSize = size(currentLink.xx);
yySize = size(currentLink.yy);

// checking that xx and yy are coherents
if xxSize <> yySize then pause, end

// checking that link.xx is a column vector
if xxSize(2) <> 1 then pause, end

