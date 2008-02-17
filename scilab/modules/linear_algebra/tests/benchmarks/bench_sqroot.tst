// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for sqroot function
//==============================================================================

// <-- BENCH NB RUN : 10000 -->

X = rand(5,2)*rand(2,5);
X = X*X';

// <-- BENCH START -->
W = sqroot(X);
// <-- BENCH END -->
