// ================================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ================================================================================

//=================================================================================
// Benchmark for csvRead, the algorithm should be linear to the number of lines
//=================================================================================

// <-- BENCH NB RUN : 5 -->
// generate some CSV files

N=4;
space = logspace(0, 3, N);
R = rand(space($), space($));
for i=space,
    for j=space,
        fprintfMat(fullfile(TMPDIR, msprintf("%dx%d.csv", i, j)), R(1:i, 1:j));
    end
end

// <-- BENCH START -->
for i=space,
    for j=space,
        csvRead(fullfile(TMPDIR, msprintf("%dx%d.csv", i, j)));
    end
end
// <-- BENCH END -->



