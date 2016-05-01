// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for matrix function
//==============================================================================

// <-- BENCH NB RUN : 10 -->

a = 0;
b = 0;

a = abs(rand(1500,1500,"n")/10);

// <-- BENCH START -->
b = a';
a = matrix(b,750,3000);
b = a';
// <-- BENCH END -->
