// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

refMsg = msprintf(_("%s: scicosim is not running.\n"), "set_blockerror");
assert_checkerror("psim = set_blockerror(-1)", refMsg);
assert_checkerror("psim = set_blockerror(-2)", refMsg);
assert_checkerror("psim = set_blockerror(-3)", refMsg);
assert_checkerror("psim = set_blockerror(-16)", refMsg);
