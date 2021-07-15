// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - Samuel Gougeon
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// unit test of union()
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->

// Empty - empty
// -------------
assert_checkequal(union([],[]), []);
assert_checkequal(union([],[],"r"), []);
assert_checkequal(union([],[],"c"), []);
[u,ka,kb] = union([],[]);
assert_checkequal(u, []);
assert_checkequal(ka, []);
assert_checkequal(kb, []);

// empty, a
// --------
assert_checkequal(union(1,[]),1);
assert_checkequal(union([1 2],[]),[1 2]);
assert_checkequal(union([1 2]',[]),[1 2]);
assert_checkequal(union([1 2 ; 2 0],[]),[0 1 2]);
assert_checkequal(union(cat(3,[1 2],[0 1]), 0),[0 1 2]);

assert_checkequal(union([],1),1);
assert_checkequal(union([], [1 2]),[1 2]);
assert_checkequal(union([],[1 2]'),[1 2]);

a  = [4 1 %nan -3 %inf 10 %nan -%inf %inf 1];
ref = [-%inf -3 1 4 10 %inf %nan %nan];
assert_checkequal(union(a, []), ref);
assert_checkequal(union(a, [], "r"), a);
assert_checkequal(union(a, [], "c"), ref);

assert_checkequal(union([], a), ref);
assert_checkequal(union([], a, "r"), a);
assert_checkequal(union([], a, "c"), ref);

// With Booleans
// -------------
[T, F] = (%t, %f);
a = [T T F T F T
     F F F F T T
     T F F F F T ];
b  = [F F T T F F
      T T T T T T
      T F T T T F ];
Ref = [F F F T T T
       F T T F F T
       F F T F T T ];
assert_checkequal(union(T,T),T);
assert_checkequal(union(T,F),[F T]);
assert_checkequal(union([T F]',F),[F T]);
assert_checkequal(union([T F]',F),[F T]);
assert_checkequal(union(cat(3,a,b),F),[F T]);
// "c", "r"
assert_checkequal(union(a,b,"c"),Ref);
[v, ka, kb] = union(a, b, "c");
assert_checkequal(list(v, ka, kb), list(Ref,[3 5 2 1 6], 1));

assert_checkequal(union(a',b',"r"), Ref');
[v, ka, kb] = union(a', b', "r");
assert_checkequal(list(v, ka, kb), list(Ref',[3 5 2 1 6], 1));

// With real numbers
// -----------------
a  = [2.   2.   1.   2.   0.
      0.   2.   1.   2.   1. ];
b  = [1.   0.   2.   0.   1.
      2.   0.   0.   1.   1. ];
Ref= [0.   0.   1.   1.   2.   2.
      0.   1.   1.   2.   0.   2. ];
assert_checkequal(union(a,b,"c"), Ref);
assert_checkequal(union(a',b',"r"), Ref');
[v,ka,kb] = union(a,b,"c");
assert_checkequal(list(v,ka,kb), list(Ref,[5 3 1 2],[2 1]));
[v,ka,kb] = union(a',b',"r");
assert_checkequal(list(v,ka,kb), list(Ref',[5 3 1 2],[2 1]));


// ===========
// With sparse
// ===========
es = sparse([]);
esb = sparse(%t); esb(1) = [];

// Element-wise processing
// -----------------------
// Results with sparse input(s) must be equal to those got with dense inputs,
//  but be sparse.
objects = list(es, sparse(0), sparse(5), sparse([2 %nan 0 -3 0 4]), ..
    sparse([2 0 -3 0 %inf 4]'), sparse([0 2 -%inf ; %nan 0 2]), ..
    sparse(complex([0 1 %inf -1 %nan],[3 -%inf 2 -1 0])), ..
    esb, sparse(%t), sparse(%f), sparse([%t %f %f %t %f]), ..
    sparse([%t %f %f %t %f]'), sparse([%t %f %t ; %t %t %f]));
for a = objects
    for b = objects
        uref = union(full(a), full(b));
        u = union(a, b);
        if ~((isequal(a,es) | isequal(a,esb)) & (isequal(b,es) | isequal(b,esb)))
            assert_checktrue(issparse(u));
        else
            assert_checkequal(u,[]);
        end
        assert_checkequal(full(u), uref);

        [uref, karef, kbref] = union(full(a), full(b));
        [u, ka, kb] = union(a, b);
        assert_checkequal(ka, karef);
        assert_checkequal(kb, kbref);
    end
end
// "r" and "c" processing
// ----------------------
add = list(0, %i);  // real, then complex numbers
for p = add
    a  = [2.   2.   1.   2.   0.
          0.   2.   1.   2.   1. ] + p;
    b  = [1.   0.   2.   0.   1.
          2.   0.   0.   1.   1. ] + p;
    Ref= [0.   0.   1.   1.   2.   2.
          0.   1.   1.   2.   0.   2. ] + p;
    [a, b, Ref] = (sparse(a), sparse(b), sparse(Ref));
    assert_checkequal(union(a,b,"c"), Ref);
    assert_checkequal(union(a',b',"r"), Ref');
    [v,ka,kb] = union(a,b,"c");
    assert_checkequal(list(v,ka,kb), list(Ref,[5 3 1 2],[2 1]));
    [v,ka,kb] = union(a',b',"r");
    assert_checkequal(list(v,ka,kb), list(Ref',[5 3 1 2],[2 1]));
end
