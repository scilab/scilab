// =============================================================================
// Scilab ( http://wwwscilaborg/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
// =============================================================================

a = [223;111;2;4;2;2];
b = [2;3;21;223;123;22];
REF_K = [4 ; 2];
for f = list(double, int8, uint8, int16, uint16, int32, uint32, int64, uint64)
    REF_V = f([4 ; 111]);
    [v,k]= setdiff(f(a), f(b));
    assert_checkequal(v, REF_V);
    assert_checkequal(k, REF_K);
end

// =============================================================================

REF_V = ['111' ; '4'];
REF_K = [2; 4];
[v,k] = setdiff(string(a),string(b));
if and(v == REF_V) <> %t then pause,end
if and(k == REF_K) <> %t then pause,end

// =============================================================================
s  = 7;
s2 = 5;
r  = [14   0  4  7   8  15   7  17  15  12];
r2 = [ 2  11  1  0  15   7  10   5];
c  = r';
c2 = r2';
m  = [
   1  3  2  0  0  0  1  2  1  3
   1  3  2  1  3  1  2  3  1  0
];
m2  = [
   0  3  1  1  3  2  0  2  0
   2  0  0  2  2  1  2  2  3
 ];
h = cat(3, m(:,1:6), m2(:,1:6));
h2 = cat(3, m([2 1],1:6), m2([2 1],1:6));
L  = list(s,  r,  c,  m,  h);
L2 = list(s2, r2, c2, m2, h2);
msgr = "setdiff: Arguments #1 and #2: Same numbers of columns expected.";
msgc = "setdiff: Arguments #1 and #2: Same numbers of rows expected.";

// With a=[] , orien = none|"r"|"c"
// --------------------------------
for i = 1:length(L)
    o = L(i);
    [va, ka] = setdiff([], o);
    assert_checkequal(va, []);
    assert_checkequal(ka, []);
    [va, ka] = setdiff([], o, "r");
    assert_checkequal(va, []);
    assert_checkequal(ka, []);
    [va, ka] = setdiff([], o, "c");
    assert_checkequal(va, []);
    assert_checkequal(ka, []);
    assert_checkerror("setdiff(o, [], ''r'')", msgr);
    assert_checkerror("setdiff(o, [], ''c'')", msgc);
end

// "r" and "c" options
// -------------------
for f = list(double, int8, uint8, int16, uint16, int32, uint32, int64, uint64)
    // With row vectors
    [rr,k] = setdiff(f(r), f([r2 0 2]), "r");
    assert_checkequal(rr, f(r));
    assert_checkequal(k, 1);
    [rc,k] = setdiff(f(r), f(r2), "c");
    assert_checkequal(rc, f([4  8  12  14  17]));
    assert_checkequal(k,    [3  5  10   1   8]);

    // With column vectors
    [rc,k] = setdiff(f(r'), f([r2 0 2]'), "c");
    assert_checkequal(rc, f(r'));
    assert_checkequal(k, 1);
    [rr,k] = setdiff(f(r'), f(r2'), "r");
    assert_checkequal(rr, f([4  8  12  14  17]'));
    assert_checkequal(k,    [3  5  10   1  8]');

    // With matrices
    [mc,k] = setdiff(f(m), f(m2), "c");
    assert_checkequal(mc, f([0  1  2  3
                             1  1  3  3]));
    assert_checkequal(k,    [4  1  8  2]);
    [mr,k] = setdiff(f(m'), f(m2'), "r");
    assert_checkequal(mr, f([0  1  2  3
                            1  1  3  3]'));
    assert_checkequal(k,   [4  1  8  2]');

    // With hypermatrices
    [hc,k] = setdiff(f(h), f(h2), "c");
    assert_checkequal(hc, f([0  3
                             2  2]));
    assert_checkequal(k,    [7 11]);

    hr  = permute(h, [2 1 3]);
    h2r = permute(h2,[2 1 3]);
    [hr,k] = setdiff(f(hr), f(h2r), "r");
    assert_checkequal(hr, f([0  2
                             3  2]));
    assert_checkequal(k,    [7 11]');
end

// =============================================================================
// Error messages
msg = "%s: Wrong number of input argument(s): %d or %d expected.\n";
assert_checkerror("setdiff()", msg , [], "setdiff", 2, 3);
