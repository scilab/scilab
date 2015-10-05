// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark sparse insertion
//==============================================================================

// <-- BENCH NB RUN : 1000 -->

N = 3000;
N1 = 50;
N2 = 50**2;
ratio = .04; //4%
N3 = N2 * ratio;

// <-- BENCH START -->
sp = spzeros(N,N);
sp(1:N1,1:N1) = 1;
sp(N2 + (1:N3+1)) = 1;

sp = spzeros(N,N);
sp(1:N2) = 1;
sp(N2 + (1:N3-1)) = 1;
// <-- BENCH END -->

