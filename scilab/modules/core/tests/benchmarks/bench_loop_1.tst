// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for 'for' with a condition
//==============================================================================

// <-- BENCH NB RUN : 1 -->
n = 1d7;j = 0; 
// <-- BENCH START -->
for i=1:n
  if i
    j;
  end
end
// <-- BENCH END -->
