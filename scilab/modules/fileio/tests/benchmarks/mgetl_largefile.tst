// ================================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// ================================================================================

//=================================================================================
// Benchmark for mgetl function
//==============================================================================

// <-- BENCH NB RUN : 10 -->
filename = fullfile(TMPDIR, "largefile.txt");
mat = rand(2000, 1000);
fprintfMat(filename, mat, "%0.6f");

// <-- BENCH START -->
M = mgetl(filename);
// <-- BENCH END -->
