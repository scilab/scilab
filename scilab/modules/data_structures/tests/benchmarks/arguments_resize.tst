// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Pass raw values as rhs / lhs
//==============================================================================

// <-- BENCH NB RUN : 10 -->

foo1 = zeros(4096,4096);
foo2 = zeros(4096,4096);
foo3 = [];

function foo3=computation(foo1, foo2)
    foo3 = foo1 + foo2
endfunction

// <-- BENCH START -->
foo3 = computation(foo1, foo2);
// <-- BENCH END -->

