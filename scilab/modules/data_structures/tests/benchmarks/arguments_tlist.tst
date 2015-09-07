// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

//==============================================================================
// Pass tlist as rhs / lhs, let scilab reuse the allocated space
//==============================================================================

// <-- BENCH NB RUN : 10 -->

data = tlist(["mydata" "foo1" "foo2" "foo3"], [], [], []);

data.foo1 = zeros(4096,4096);
data.foo2 = zeros(4096,4096);
data.foo3 = zeros(4096,4096);

function data=computation(data)
    data.foo3 = data.foo1 + data.foo2
endfunction

// <-- BENCH START -->
data = computation(data);
// <-- BENCH END -->

