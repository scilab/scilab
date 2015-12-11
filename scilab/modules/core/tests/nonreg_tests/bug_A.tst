//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug A -->
//
// <-- OLD BUG -->

if or((1&[1,0,1;1,1,0])<>([%T,%F,%T;%T,%T,%F])) then pause,end
