// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

// Allocate a Link and check its default values
lnk = scicos_link()
assert_checkequal(lnk.xx,    []);
assert_checkequal(lnk.yy,    []);
assert_checkequal(lnk.id,    "");
assert_checkequal(lnk.thick, [0 0]);
assert_checkequal(lnk.ct,    [1 1]);
assert_checkequal(lnk.from,  [0 0 0]);
assert_checkequal(lnk.to,    [0 0 1]);


// Check that the Link is freed
clear
