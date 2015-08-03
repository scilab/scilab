// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// Unit test for conj

c=2*%i;
d=1;
c_d=1+2*%i;
A=[1+2*%i, 3+2*%i; 0, 0];
spA=sparse(A);
s=poly(0,"s");
M = hypermat([1 2 2],1:4);
M(:,1,1)=%i;

assert_checkequal(conj(%nan), %nan);
assert_checkequal(conj(%inf), %inf);
assert_checkequal(conj(complex(0,%inf)), complex(0,-%inf));
assert_checkequal(conj([]), []);
assert_checkequal(conj(c), -2*%i);
assert_checkequal(conj(d), 1);
assert_checkequal(conj(c_d), 1-2*%i);
assert_checkequal(conj(A), [1-2*%i, 3-2*%i; 0, 0]);
assert_checkequal(conj(spA), sparse(conj(A)));
assert_checkequal(conj(diag(A)), [1-2*%i; 0]);
assert_checkequal(conj(speye(4,4)), speye(4,4));
assert_checkequal(conj(1+s+%i), 1+s-%i);
assert_checkequal(conj(M),hypermat([1,2,2],[-%i;2;3;4]));

// Error messages
errmsg1=msprintf(_("%s: Wrong number of input argument(s): %d expected.\n"), "conj", 1);
assert_checkerror("conj()", errmsg1, 77);
assert_checkerror("conj(A,2)", errmsg1, 77);
errmsg2=msprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "conj", 1);
assert_checkerror("[res1, res2]=conj(A)", errmsg2, 78);
