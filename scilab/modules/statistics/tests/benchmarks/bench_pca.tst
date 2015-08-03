// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for sva function
//==============================================================================

// <-- BENCH NB RUN : 100 -->
A = rand(1000,100,"n");

// <-- BENCH START -->
[lambda,facpr,comprinc] = pca(A);
// <-- BENCH END -->
