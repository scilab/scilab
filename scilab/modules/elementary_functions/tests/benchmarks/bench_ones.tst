// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for ones function
//==============================================================================

// <-- BENCH NB RUN : 10 -->

a = 2250;

// <-- BENCH START -->
b = ones(a, a)./((1:a)' * ones(1, a) + ones(a, 1) * (0:(a-1)));
// <-- BENCH END -->
