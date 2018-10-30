// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 15853 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15853
//
// <-- Short Description -->
// mlist('cblock') entered an infinite "operation +: Warning adding a empty.." loop

mlist("cblock")
