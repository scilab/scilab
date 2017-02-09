// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Sscilab Enterprises - Antoine Elias
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

a=sparse([1 6;1 5;1 3;2 4;2 1;4 4;4 3;5 1;6 6],1:9,[6 6]);
ai = a * (2 - 4 * %i);
ref = full(a);
refi = full(ai);
[m,n] = size(a);

// i
for i = 1:m*n
    b = a(i);
    r = ref(i);
    assert_checkequal(full(b), r);
    bi = ai(i);
    ri = refi(i);
    assert_checkequal(full(bi), ri);
end

// i,j
for i = 1:m
    for j = 1:n
        b = a(i,j);
        r = ref(i,j);
        assert_checkequal(full(b), r);
        bi = ai(i,j);
        ri = refi(i,j);
        assert_checkequal(full(bi), ri);
    end
end

// :
b = a(:);
r = ref(:);
assert_checkequal(full(b), r);

bi = ai(:);
ri = refi(:);
assert_checkequal(full(bi), ri);

// :,:
b = a(:,:);
r = ref(:,:);
assert_checkequal(full(b), r);

bi = ai(:,:);
ri = refi(:,:);
assert_checkequal(full(bi), ri);

// 1:$,1:$
b = a(1:$,1:$);
r = ref(1:$,1:$);
assert_checkequal(full(b), r);

bi = ai(1:$,1:$);
ri = refi(1:$,1:$);
assert_checkequal(full(bi), ri);

// 1:2:$,1:2:$
b = a(1:2:$,1:2:$);
r = ref(1:2:$,1:2:$);
assert_checkequal(full(b), r);

bi = ai(1:2:$,1:2:$);
ri = refi(1:2:$,1:2:$);
assert_checkequal(full(bi), ri);
