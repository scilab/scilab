// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for getdate function
//==============================================================================

a = rand(10,10) * 2e9;

// <-- BENCH START -->
w=getdate(a);
// <-- BENCH END -->

