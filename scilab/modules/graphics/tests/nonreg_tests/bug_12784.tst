// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 12784 -->
//
// <-- Bugzilla Ua rL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=12784
//
// <-- Short Description -->
//    Misleading error message in many functions when passing an integer 
//    argument instead of double argument.
// =============================================================================

assert_checkerror("autumncolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "autumncolormap", 1);
assert_checkerror("autumncolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "autumncolormap", 1);

assert_checkerror("bonecolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "bonecolormap", 1);
assert_checkerror("bonecolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "bonecolormap", 1);

assert_checkerror("coolcolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "coolcolormap", 1);
assert_checkerror("coolcolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "coolcolormap", 1);

assert_checkerror("coppercolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "coppercolormap", 1);
assert_checkerror("coppercolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "coppercolormap", 1);

assert_checkerror("graycolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "graycolormap", 1);
assert_checkerror("graycolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "graycolormap", 1);

assert_checkerror("hotcolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "hotcolormap", 1);
assert_checkerror("hotcolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "hotcolormap", 1);

assert_checkerror("hsvcolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "hsvcolormap", 1);
assert_checkerror("hsvcolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "hsvcolormap", 1);

assert_checkerror("jetcolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "jetcolormap", 1);
assert_checkerror("jetcolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "jetcolormap", 1);

assert_checkerror("oceancolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "oceancolormap", 1);
assert_checkerror("oceancolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "oceancolormap", 1);

assert_checkerror("pinkcolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "pinkcolormap", 1);
assert_checkerror("pinkcolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "pinkcolormap", 1);

assert_checkerror("rainbowcolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "rainbowcolormap", 1);
assert_checkerror("rainbowcolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "rainbowcolormap", 1);

assert_checkerror("springcolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "springcolormap", 1);
assert_checkerror("springcolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "springcolormap", 1);

assert_checkerror("summercolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "summercolormap", 1);
assert_checkerror("summercolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "summercolormap", 1);

assert_checkerror("whitecolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "whitecolormap", 1);
assert_checkerror("whitecolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "whitecolormap", 1);

assert_checkerror("wintercolormap(uint8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "wintercolormap", 1);
assert_checkerror("wintercolormap(int8(1))", "%s: Wrong type for input argument #%d: a real scalar expected.\n", 10000, "wintercolormap", 1);
