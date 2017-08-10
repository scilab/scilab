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
// column vector composed of 5000 '€' code points
m = repmat([226 130 172], 1, 50000);

// <-- BENCH START -->
s = ascii(m);
// <-- BENCH END -->
