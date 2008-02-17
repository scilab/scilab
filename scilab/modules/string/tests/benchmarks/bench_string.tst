// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for string function
//==============================================================================

test_mat = rand(10,10);

// <-- BENCH START -->
a = string(test_mat);
// <-- BENCH END -->

