// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// Unit tests for [,] and [;] with cblocks and other columns of regular types

T = msscanf(-1,["Alain 19";"Pierre 15";"Tom 12"],'%s %d');
e = mlist('cblock');
b = rand(3,2)<0.5;
i = int8(grand(3,1,"uin",-9,9));
d = (1:3)'*1.1;
t = ["Toulouse" "Lyon" "Le Mans"]';

assert_checkequal([e T], T);
assert_checkequal([T e], T);
assert_checkequal([e ; T], T);
assert_checkequal([T ; e], T);

assert_checkequal([[] T], T);
assert_checkequal([T []], T);
assert_checkequal([[] ; T], T);
assert_checkfalse(execstr("[T ; []]","errcatch")==0);

for o = list(b, i, d, t)
    so = size(o,2);
    r = [o T];
    assert_checkequal(size(r), [3 2+so]);
    assert_checkequal(r(:,1), o(:,1));
    assert_checkequal(r(:,$-1:$), T);
    r = [T o];
    assert_checkequal(size(r), [3 2+so]);
    assert_checkequal(r(:,$), o(:,$));
    assert_checkequal(r(:,1:2), T);
end

r = [T T];
assert_checkequal(size(r), [3 4]);
assert_checkequal(r(:,1), r(:,3));
assert_checkequal(r(:,2), r(:,4));
