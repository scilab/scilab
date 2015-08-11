// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for matrix ops
//==============================================================================

// <-- BENCH NB RUN : 10 -->

a = 0;
b = 0;
a = rand(700,700,"n");

// <-- BENCH START -->
b = a'*a;
// <-- BENCH END -->
