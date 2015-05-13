// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Checks that Scilab:
// - has been compiled using Intel C++ Compiler version 14
// - and uses MKL

[a,b]=getdebuginfo();
assert_checkequal(b(5), "Compiled with Intel compiler (1400)");
assert_checkequal(b(6), "BLAS library optimized version: MKL");
