// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

c = complex(1);
assert_checkequal([real(c);imag(c)],[1;0]);
//
c = complex(1,2);
assert_checkequal([real(c);imag(c)],[1;2]);
//
c = complex([1 2]);
assert_checkequal([real(c);imag(c)],[1 2;0 0]);
//
c = complex([1 2 3], 4);
assert_checkequal([real(c);imag(c)],[1 2 3;4 4 4]);
//
c = complex(1, [2 3 4]);
assert_checkequal([real(c);imag(c)],[1 1 1;2 3 4]);
//
c = complex([1 2],[3 4]);
assert_checkequal([real(c);imag(c)],[1 2;3 4]);
//
L = list(1, [1 2], sparse(1), sparse([1 2]), %t, [%f %t], sparse(%t), sparse([%f %t]));
for Re = L
    assert_checkequal(issparse(complex(Re)), issparse(Re));
    for Im = L
        if ~issparse(Re) & ~issparse(Im)
            continue
        end
        c = complex(Re, Im);
        ref = and(size(Re)==size(Im)) && issparse(Re) && issparse(Im)
        assert_checkequal(issparse(c), ref);
    end
end

// Special values
// --------------
g = [-%inf 0 1 %inf %nan];
// Generating all possible (rel, imag) combinations:
r = g .*. ones(1,5);
i = ones(1,5) .*. g;

c = complex(r, i);
assert_checkequal(real(c), r);
assert_checkequal(imag(c), i);

r = sparse(r);
i = sparse(i);
c = complex(r, i);
assert_checkequal(real(c), r);
assert_checkequal(imag(c), i);
