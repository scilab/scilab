// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15261 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15261
//
// <-- Short Description -->
// Wrong default inserted in struct

s.t = "Test";
s(3).t = %pi;
type(s(2).t)==0
