// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 8714 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/8714
//
// <-- Short Description -->
// Problem with Incremental Search bar when Code Navigator is docked

scinotes();

// F3 to start an Incremental Search.
// ctrl+G to have the Code Navigator.
// Dock the Code Navigator, undock it
// In the two previous cases the Search bar must stay at its place. 