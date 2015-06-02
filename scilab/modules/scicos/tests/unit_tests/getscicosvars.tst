// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

refMsg = msprintf(_("%s: Undefined field in string matrix position : %d\n"), "getscicosvars", 2);
assert_checkerror("getscicosvars([""x"" ""fake""])", refMsg);

refMsg = msprintf(_("%s: Error with parameter ""%s"".\n"), "getscicosvars", "x");
assert_checkerror("getscicosvars([""x"" ""blocks""])", refMsg);
