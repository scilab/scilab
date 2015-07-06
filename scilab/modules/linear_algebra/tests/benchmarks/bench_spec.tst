// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for spec function
//==============================================================================

// <-- BENCH NB RUN : 10 -->

a = 0;
b = 0;

a = rand(320,320,"n");

// <-- BENCH START -->
b = spec(a);
// <-- BENCH END -->
