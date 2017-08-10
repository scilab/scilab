// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
// Copyright (C) 2016 - Samuel GOUGEON <sgougeon@free.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Benchmark test for eval function
n = 100;
m = rand(n, n+1);
s = string(m);

// <-- BENCH NB RUN : 10 -->
// <-- BENCH START -->
e = eval(s);
// <-- BENCH END -->
