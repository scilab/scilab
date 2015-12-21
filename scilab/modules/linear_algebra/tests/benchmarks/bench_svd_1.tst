// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for svd 1
//==============================================================================

// <-- BENCH NB RUN : 10 -->

a = 0;
b = 0;

a=rand(300,300);

b     = a*a';
[c,d] = lu(b);
inv(a);

// <-- BENCH START -->
v = svd(a);
// <-- BENCH END -->
