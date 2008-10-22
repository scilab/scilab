// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for ascii function
//==============================================================================

code_test_mat = int( 128 * rand(100,100) );

// <-- BENCH START -->
a = ascii(code_test_mat);
// <-- BENCH END -->

