// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 14199 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=14199
//
// <-- Short Description -->
//    sqrt does not manage matrix with more than 2 dims

m = rand(3, 3, 3);
val = sqrt(m);
ref = zeros(3, 3, 3);

for i=1:3
    for j=1:3
        for k=1:3
            ref(i,j,k) = sqrt(m(i,j,k));
        end
    end
end

assert_checkequal(size(val), size(m));
assert_checkequal(val, ref);