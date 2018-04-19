// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Stéphane Mottelet
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 15539 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15539
//
// <-- Short Description -->
// Zero step in integer implicit list crashes scilab

assert_checkequal(uint8(0):0:1,[]);
