// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for read function
//==============================================================================

// <-- BENCH NB RUN : 5 -->
nrows = 50000;
ncols = 5;
A = rand(nrows, ncols);
filename = fullfile(TMPDIR, 'read_bench_mat');
deletefile(filename);
write(filename, A);
clear A;

// <-- BENCH START -->
B = read(filename, nrows, ncols);
// <-- BENCH END -->

