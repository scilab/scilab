// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for inv function
//==============================================================================

// <-- BENCH NB RUN : 100 -->

a = 0;
b = 0;

a = rand(400, 400,"n");

// <-- BENCH START -->
b = inv(a);
// <-- BENCH END -->
