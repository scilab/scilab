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
// <-- Non-regression test for bug 15548 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/15548
//
// <-- Short Description -->
// [%t %t]./[%f %f] crashes Scilab

assert_checkequal([%t %t]./[%f %f],[%inf %inf]);
