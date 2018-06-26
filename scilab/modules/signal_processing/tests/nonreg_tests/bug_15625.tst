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
// <-- Non-regression test for bug 15625 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15625
//
// <-- Short Description -->
// delip(1,1)=0, instead of Inf

assert_checkequal(delip(1,1),%inf)
