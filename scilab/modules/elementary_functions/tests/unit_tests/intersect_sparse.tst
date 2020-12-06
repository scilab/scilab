// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Short Description -->
// Unitary tests for intersect() with sparse boolean or numeric matrices

// ===============
// Sparse booleans
// ===============
[T, F] = (%t, %f);
[esp, espb] = (sparse([]), sparse(F)); espb(1) = []

// With an empty
assert_checkequal(intersect(T, espb), []);
[v, ka, kb] = intersect(T, espb);
assert_checkequal(list(v, ka, kb), list([], [], []));

assert_checkequal(intersect(espb, F), []);
[v, ka, kb] = intersect(espb, F);
assert_checkequal(list(v, ka, kb), list([], [], []));
//
A = [F F T F T F
     T F F T T T
     T T F T F F ];
B = [F T F T F
     T F F F T
     F T F F T ];
ref = [F F T
       T T F
       F T F ];
[spA, spB, spref] = (sparse(A), sparse(B), sparse(ref));
assert_checkequal(intersect(spA, B), sparse([F T]));
assert_checkequal(intersect(A, spB), sparse([F T]));
assert_checkequal(intersect(spA, spB), sparse([F T]));

LA = list(spA, A, spA);
LB = list(B, spB, spB);
for i = 1:3
    assert_checkequal(intersect(LA(i), LB(i), "c"), spref);
    [v,ka,kb] = intersect(LA(i), LB(i), "c");
    assert_checkequal(v, spref);
    assert_checkequal(ka, [6 1 3]);
    assert_checkequal(kb, [1 5 4]);
    assert_checkequal(sparse(LA(i)(:,ka)), v);
    assert_checkequal(sparse(LB(i)(:,kb)), v);

    assert_checkequal(intersect(LA(i)', LB(i)', "r"), spref');
    [v,ka,kb] = intersect(LA(i)', LB(i)', "r");
    assert_checkequal(v, spref');
    assert_checkequal(ka, [6 1 3]);
    assert_checkequal(kb, [1 5 4]);
    assert_checkequal(sparse(LA(i)(:,ka))', v);
    assert_checkequal(sparse(LB(i)(:,kb))', v);
end

// ============
// Sparse reals
// ============
A = sparse([-3 5 %nan 0 1e20 -%inf 123 1000]);
B = sparse([1 %nan -3 123 4530 0 -%inf ]);
assert_checkequal(intersect(A,B), sparse([-%inf -3 0 123]));
[v, ka, kb] = intersect(A,B);
assert_checkequal(v, sparse([-%inf -3 0 123]));
assert_checkequal([ka ; kb], [6 1 4 7 ; 7 3 6 4]);
//
A = sparse([1  0  0  0  0  1  2  0  2  0
            2  0  2  0  1  0  1  0  0  2
            2  0  0  0  0  1  0  0  0  0 ]);
B = sparse([2  1  1  0  1  0  0  2  2  0
            2  0  1  1  0  0  1  1  0  0
            0  2  0  0  1  1  1  2  0  0 ]);
Ref = sparse([0  0  1  2
              0  1  0  0
              0  0  1  0 ]);
kaRef  = [2  5  6  9];
kbRef  = [10  4  5  9];

assert_checkequal(intersect(A,B,"c"), Ref);
[v, ka, kb] = intersect(A,B,"c");
assert_checkequal(v, Ref);
assert_checkequal(ka, kaRef);
assert_checkequal(kb, kbRef);

assert_checkequal(intersect(A',B',"r"), Ref');
[v, ka, kb] = intersect(A',B',"r");
assert_checkequal(v, Ref');
assert_checkequal(ka, kaRef);
assert_checkequal(kb, kbRef);


// ==============
// Sparse complex
// ==============
r = [-3  5   %nan 0   1e20 %inf 123  0];
i = [1  %nan -3   123 4530 0    -%inf 1];
A = sparse(complex(r,i))
r = [-3  0  1e20 %nan 0    %inf 123 ];
i = [0   1  1    -3   123  0   -%inf];
B = sparse(complex(r,i))
Ref = sparse(complex([0 0 123 %inf], [1 123 -%inf 0]));
assert_checkequal(intersect(A,B), Ref);

// "c" and "r" modes
// -----------------
r  = [0.   1.   0.   0.   0.   1.   0.   1.   0.   0.
      1.   0.   1.   1.   1.   0.   1.   0.   1.   0. ];
i  = [1.   0.   0.   1.   1.   0.   1.   0.   0.   0.
      1.   1.   0.   0.   1.   0.   0.   0.   1.   0. ];
A = sparse(complex(r,i));

r  = [0.   1.   1.   1.   0.   1.   0.   0.   0.   0.
      1.   1.   1.   1.   1.   0.   0.   1.   0.   1. ];
i  = [0.   0.   1.   1.   1.   1.   1.   0.   1.   1.
      0.   0.   0.   0.   1.   0.   0.   1.   1.   1. ];
B = sparse(complex(r,i));

Ref = sparse([0,  0   ,   %i ;
              1,  1+%i, 1+%i]);
kaRef  = [3 9 1];
kbRef  = [1 8 5];

assert_checkequal(intersect(A,B,"c"), Ref);
[v, ka, kb] = intersect(A,B,"c");
assert_checkequal(v, Ref);
assert_checkequal(ka, kaRef);
assert_checkequal(kb, kbRef);

assert_checkequal(intersect(A',B',"r"), Ref');
[v, ka, kb] = intersect(A',B',"r");
assert_checkequal(v, Ref');
assert_checkequal(ka, kaRef);
assert_checkequal(kb, kbRef);
