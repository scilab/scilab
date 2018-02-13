// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 15418 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15418
//
// <-- Short Description -->
// With colordef("black") and colordef("none"), titles and axes labels were
//  invisible or almost invisible.

colordef("none")
xdel(winsid())
scf(1);
param3d(), xtitle("General title","X title","Y title", "Z title")
scf(2);
plot(), xtitle("General title","X title","Y title")
// Check that titles are well visible in white

colordef("black")
xdel(winsid())
scf(1);
param3d(), xtitle("General title","X title","Y title", "Z title")
scf(2);
plot(), xtitle("General title","X title","Y title")
// Check that titles are well visible in white

colordef("white")
xdel(winsid())
scf(1);
param3d(), xtitle("General title","X title","Y title", "Z title")
scf(2);
plot(), xtitle("General title","X title","Y title")
// Check that titles are well visible in black on grey (unchanged)

colordef("default")
xdel(winsid())
scf(1);
param3d(), xtitle("General title","X title","Y title", "Z title")
scf(2);
plot(), xtitle("General title","X title","Y title")
// Check that titles are well visible in black on white (unchanged)
