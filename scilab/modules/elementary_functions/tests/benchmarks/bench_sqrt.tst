// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for sqrt function
//==============================================================================

// <-- BENCH NB RUN : 1000 -->

a   = 0;
b   = 0;
phi = 1.6180339887498949;

a = floor(1000 * rand(7500, 1, "u"));

// <-- BENCH START -->
b = (phi.^a - (-phi).^(-a)) / sqrt(5);
// <-- BENCH END -->
