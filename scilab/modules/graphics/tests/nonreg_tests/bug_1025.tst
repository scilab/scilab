// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 1025 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1025
//
// <-- Short Description -->
//    winsid ne marche pas.

set figure_style old;
xset('window',1000);
set figure_style old;
xset('window',0);
set figure_style new;

A = winsid();

if or(A <> [0,1000]) then pause,end
