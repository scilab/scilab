// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// unit tests for gcd()
// ====================

// Checking arguments
// ------------------
[g, f] = gcd([]);
assert_checkequal(g, []);
assert_checkequal(f, 1);
d = 9*49*13*23*29*37*47;    // 6.650D+09   2^63 ~ 9.223D+18
v = [d*2*25*31  d*29*37  d*31];
[g, f] = gcd(v);
assert_checkequal(size(g), [1 1]);
assert_checkequal(size(f), [3 3]);

// Checking errors
// ---------------
assert_checkfalse(execstr("gcd()", "errcatch")==0);
assert_checkfalse(execstr("gcd(%z,%s)", "errcatch")==0);
assert_checkfalse(execstr("[a,b,c]=gcd(v)", "errcatch")==0);
assert_checkfalse(execstr("gcd(sparse([25 21 110]))", "errcatch")==0);
assert_checkfalse(execstr("gcd([25 21 110]+0.1)", "errcatch")==0);

// Checking results
// ================
// With encoded integers
// ---------------------
//--> pr = primes(50)
// pr  =
//   2.  3.  5.  7.  11.  13.  17.  19.  23.  29.  31.  37.  41.  43.  47.
// int8
Gcd = 10;     // 2^7 = 128
v = [10*3 ; 10*11];
// int16
d =  3*7*13;  // 273.  2^15 = 32768
Gcd = [Gcd  d];
v = [v  [d*3*17 ; d*5*23]];
// int32
d =  3*7*13*29*37;  // 292929.  2^31 ~ 2.147D+09
Gcd = [Gcd  d];
v = [v  [d*2*19*27 ; d*5*11*23]];
// int64
d = 9*49*13*23*29*37*47;  // 6.650D+09   2^63 ~ 9.223D+18
Gcd = [Gcd  d];
v = [v  [d*2*25*31 ; d*7*29]];  // 1.031D+13 ; 1.350D+12
for j = 1:length(Gcd)
    for i = [0 10]
        ty = 2^(j-1)+i;  // inttype: [1 11 2 12 4 14 8 18]
        [g, m] = gcd(iconvert(v(:,j), ty));
        assert_checkequal(inttype(g), ty);
        assert_checkequal(g, iconvert(Gcd(j), ty));
        assert_checkequal(inttype(m), modulo(ty,10));
        if ty~=18
            assert_checkequal(v(:,j)'*m, iconvert([0 Gcd(j)],inttype(m)));
            // Otherwise there are expected round-off errors
        end
    end
end

// With decimal integers
// ---------------------
// Decimals  2^31 ~ 2.147D+09 < X < 2^52 ~ 4.504D+15
v = [d*2*25*31 ; d*29*37];  //    1.031D+13 ; 7.135D+12
[g, f] = gcd(v);
assert_checkequal(g, d);
assert_checkequal(v'*f, [0 g]);


// With negative numbers
// ---------------------
// http://bugzilla.scilab.org/15058
v = v(:)';
v(1) = -v(1);
[g, f] = gcd(v);
assert_checktrue(g>0);
assert_checkequal(g, d);
assert_checkequal(v*f, [0 g]);
v = -v;
[g, f] = gcd(v);
assert_checktrue(g>0);
assert_checkequal(g, d);
assert_checkequal(v*f, [0 g]);
v = -abs(v);
[g, f] = gcd(v);
assert_checktrue(g>0);
assert_checkequal(g, d);
assert_checkequal(v*f, [0 g]);


// With polynomials
// ----------------
d = (1-%z)*(2+%z)*(5*%z -4);
v = d*[%z (%z+1)];
[g, f] = gcd(v);
assert_checkequal(type(g), 2);
assert_checktrue(clean(g+d/5)==0);
assert_checkequal(clean(f - [-5-5*%z, 0.2 ; 5*%z, -0.2]),zeros(2,2)*%z);
