// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark sparse insertion
//==============================================================================

// <-- BENCH NB RUN : 1 -->

N = 300000;
i = rand(N, 1) < 0.01623;
s = size(find(i),"*");
B = rand(s, s);

// <-- BENCH START -->
sp = spzeros(N,N);
sp(i,i) = B;
// <-- BENCH END -->

