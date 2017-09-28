// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// Benchmark test for a recursive function

// recursive fibo, O(2^n) complexity
function res = fibo(fn)
    if fn < 2 then
        res = 1
    else
        res = fibo(fn-1) + fibo(fn-2)
    end
endfunction

F = zeros(22,1);
// <-- BENCH NB RUN : 10 -->
// <-- BENCH START -->
for i=1:size(F, "*")
    F(i) = fibo(i);
end
// <-- BENCH END -->

