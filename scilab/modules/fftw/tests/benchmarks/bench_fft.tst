// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for fft function
//==============================================================================

// <-- BENCH NB RUN : 100 -->
a = 0; b = 0;
a = rand(800000, 1, "n");

// <-- BENCH START -->
b = fft(a, -1);
// <-- BENCH END -->

