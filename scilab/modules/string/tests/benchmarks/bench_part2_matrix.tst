// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for part function
//==============================================================================

// <-- BENCH NB RUN : 400 -->
// M*N are equals to bench_part2.tst NB RUN, bench timing should be the similar
M=5;
N=5;

c = 1e5;
str = strcat(string(int(rand(1, c) * 10)));
idx = int(rand(1, c) * c + 1);
str = str(ones(M, N));

// <-- BENCH START -->
part(str, idx);
// <-- BENCH END -->
