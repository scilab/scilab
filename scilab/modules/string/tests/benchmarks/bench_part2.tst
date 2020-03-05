// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Benchmark for part function
//==============================================================================

// <-- BENCH NB RUN : 10000 -->

c = 1e5;
str = strcat(string(int(rand(1, c) * 10)));
idx = int(rand(1, c) * c + 1);

// <-- BENCH START -->
part(str, idx);
// <-- BENCH END -->
