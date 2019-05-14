// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Stéphane MOTTELET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 16012 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/16012
//
// <-- Short Description -->
//  [struct() struct()] crashes scilab

assert_checkequal([struct() struct()],struct())
