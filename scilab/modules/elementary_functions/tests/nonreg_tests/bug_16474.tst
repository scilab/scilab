// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16474 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16474
//
// <-- Short Description -->
// imult(%z) crashes Scilab 

assert_checkequal(imult(%i+%s),-1+%i*%s)