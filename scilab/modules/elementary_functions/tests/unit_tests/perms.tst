// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
// =============================================================================
//
// <-- TEST WITH GRAPHIC -->  // for gda() and gdf()
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->

Ints = list();
for t = [1 2 4 8 11 12 14 18]
    Ints($+1) = iconvert([30 40], t);
end

assert_checkequal(perms([]), []);
L1 = list([%t %f], Ints(:), [%e %pi], sparse([12 0]), ["abcdé","ABCD"], [1 %i], ..
          [1+%z,1-%z], [gda(), gdf()], {3, %z});
for o = L1
    assert_checkequal(perms(o(1)), o(1));
end
for i = 1:length(L1)-1  // [cells] crash
    o = L1(i)
    assert_checkequal(perms(o), [o([2 1]); o]);
end

ref123 = [3,2,1;3,1,2;2,3,1;2,1,3;1,3,2;1,2,3];
assert_checkequal(perms(1:3), ref123);

for i = 2:7     // 7! = 5040  8! = 40320
    p = perms(1:i);
    assert_checkequal(size(p), [factorial(i) i]);
    np = size(p,1);
    for j = 1:i
        assert_checktrue(and(sum(p==j,"r")==np/i));
        assert_checktrue(and(sum(p==j,"c")==1));
    end
end
// With complex numbers
ref = [1+%i    %i      1
       1+%i    1      %i
        %i     1+%i    1
        %i     1      1+%i
        1     1+%i    %i
        1     %i      1+%i];
assert_checkequal(perms([1, %i, 1+%i]), ref);


// With duplicates
refDupl = [2,1,1,0,0;2,1,1,0,0;2,1,0,1,0;2,1,0,0,1;2,1,0,1,0;2,1,0,0,1;2,1,1,0,0;2,1,1,0,0;2,1,0,1,0;2,1,0,0,1;2,1,0,1,0;2,1,0,0,1;2,0,1,1,0;2,0,1,0,1;2,0,1,1,0;2,0,1,0,1;2,0,0,1,1;2,0,0,1,1;2,0,1,1,0;2,0,1,0,1;2,0,1,1,0;2,0,1,0,1;2,0,0,1,1;2,0,0,1,1;1,2,1,0,0;1,2,1,0,0;1,2,0,1,0;1,2,0,0,1;1,2,0,1,0;1,2,0,0,1;1,1,2,0,0;1,1,2,0,0;1,1,0,2,0;1,1,0,0,2;1,1,0,2,0;1,1,0,0,2;1,0,2,1,0;1,0,2,0,1;1,0,1,2,0;1,0,1,0,2;1,0,0,2,1;1,0,0,1,2;1,0,2,1,0;1,0,2,0,1;1,0,1,2,0;1,0,1,0,2;1,0,0,2,1;1,0,0,1,2;1,2,1,0,0;1,2,1,0,0;1,2,0,1,0;1,2,0,0,1;1,2,0,1,0;1,2,0,0,1;1,1,2,0,0;1,1,2,0,0;1,1,0,2,0;1,1,0,0,2;1,1,0,2,0;1,1,0,0,2;1,0,2,1,0;1,0,2,0,1;1,0,1,2,0;1,0,1,0,2;1,0,0,2,1;1,0,0,1,2;1,0,2,1,0;1,0,2,0,1;1,0,1,2,0;1,0,1,0,2;1,0,0,2,1;1,0,0,1,2;0,2,1,1,0;0,2,1,0,1;0,2,1,1,0;0,2,1,0,1;0,2,0,1,1;0,2,0,1,1;0,1,2,1,0;0,1,2,0,1;0,1,1,2,0;0,1,1,0,2;0,1,0,2,1;0,1,0,1,2;0,1,2,1,0;0,1,2,0,1;0,1,1,2,0;0,1,1,0,2;0,1,0,2,1;0,1,0,1,2;0,0,2,1,1;0,0,2,1,1;0,0,1,2,1;0,0,1,1,2;0,0,1,2,1;0,0,1,1,2;0,2,1,1,0;0,2,1,0,1;0,2,1,1,0;0,2,1,0,1;0,2,0,1,1;0,2,0,1,1;0,1,2,1,0;0,1,2,0,1;0,1,1,2,0;0,1,1,0,2;0,1,0,2,1;0,1,0,1,2;0,1,2,1,0;0,1,2,0,1;0,1,1,2,0;0,1,1,0,2;0,1,0,2,1;0,1,0,1,2;0,0,2,1,1;0,0,2,1,1;0,0,1,2,1;0,0,1,1,2;0,0,1,2,1;0,0,1,1,2];
assert_checkequal(perms([0 0 1 1 2]), refDupl);

// =============
// With "unique"
// =============
assert_checkequal(perms([], "unique"), []);

for o = L1
    assert_checkequal(perms(o(1), "unique"), o(1));
end
for i = 1:12    // For complex nums: sorting is possible but not full => unusable
    o = gsort(L1(i),"g","i");
    assert_checkequal(perms(o, "unique"), [o ; o([2 1])]);
end

assert_checkequal(perms(1:3, "unique"), ref123($:-1:1,:));

p = perms([1 2 3 4 zeros(1,9)], "unique");
assert_checkequal(size(p), [17160 13]);

assert_checkequal(perms([1 zeros(1,14)], "unique"), flipdim(eye(15,15),2));

ref = [0,0,1,1,2;0,0,1,2,1;0,0,2,1,1;0,1,0,1,2;0,1,0,2,1;0,1,1,0,2;0,1,1,2,0;0,1,2,0,1;0,1,2,1,0;0,2,0,1,1;0,2,1,0,1;0,2,1,1,0;1,0,0,1,2;1,0,0,2,1;1,0,1,0,2;1,0,1,2,0;1,0,2,0,1;1,0,2,1,0;1,1,0,0,2;1,1,0,2,0;1,1,2,0,0;1,2,0,0,1;1,2,0,1,0;1,2,1,0,0;2,0,0,1,1;2,0,1,0,1;2,0,1,1,0;2,1,0,0,1;2,1,0,1,0;2,1,1,0,0];
assert_checkequal(perms([0 0 1 1 2], "unique"), ref);


// ==============
// Error messages
// ==============
msg = "perms: Wrong number of input argument(s): 1 or 2 expected.";
assert_checkerror("perms()", msg);
assert_checkerror("perms(3,%t,4)", msg);

msg = "perms: The ""unique"" option can be used only for sortable real numbers, booleans or texts.";
assert_checkerror("perms([%i 1], ""unique"")", msg);
assert_checkerror("perms([%z 1], ""unique"")", msg);
assert_checkerror("perms([gdf() gda()], ""unique"")", msg);
assert_checkerror("perms({1,%z}, ""unique"")", msg);

msg = "perms: Too many permutations (479001600). Would run out of memory.";
assert_checkerror("perms([1 zeros(1,11)]);", msg);
msg = "perms: Too many permutations (672672000). Would run out of memory."; // 180 GB required
assert_checkerror("perms([0 0 0 0 1 1 1 2 2 2 3 3 3 4 4 4], ""unique"");", msg);
