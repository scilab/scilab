// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Antoine ELIAS
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for 'for increment j'
//==============================================================================

// <-- BENCH NB RUN : 10 -->

// <-- BENCH START -->
n = 1d6;
j = 0;
for i=1:n
  j = j + 1;
end
// <-- BENCH END -->