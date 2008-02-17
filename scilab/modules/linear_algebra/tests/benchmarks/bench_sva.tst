// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for sva function
//==============================================================================

// <-- BENCH NB RUN : 1000 -->

A = rand(50,40)*rand(40,50);

// <-- BENCH START -->
[U,s,V] = sva(A,2);
// <-- BENCH END -->
