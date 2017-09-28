// ================================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises
//
//  This file is distributed under the same license as the Scilab package.
// ================================================================================

//=================================================================================
// Benchmark for mgetl function
//==============================================================================

// <-- BENCH NB RUN : 1 -->
function a = randInt(start, stop, count)
    a = int(rand(count, 1) * ((stop-1) - start) + start);
endfunction

function a = randStr(pool, count)
    s = size(pool, "*");
    i = rand(count, 1) * (s-1) + 1;
    a = matrix(pool(i), -1, 1);
endfunction

filename = fullfile(TMPDIR, "verylongfile.txt");

nb = 5000000;
rI = randInt(1d5, 1d6, nb);
strs = msprintf("%d,%d,%d,%d,%d,%d\n", rI, rI, rI, rI, rI, rI);
mputl(strs, filename);

// <-- BENCH START -->
M = mgetl(filename);
// <-- BENCH END -->
