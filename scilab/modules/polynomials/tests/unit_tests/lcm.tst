// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Bruno JOFRET
// Copyright (C) 2017, 2018 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

// With polynomials
// ----------------
s = %s;
p = [s, s*(s-1)^2, s^2*(s+2)];
[pp, fact] = lcm(p);

expected = s^2*(s-1)^2*(s+2);
assert_checkequal(pp, expected);
r =  expected./p;
r.num = r.num ./ coeff(r.den);
R = r.num;
assert_checkequal(clean(fact-R), [0 0 0]*%s);

// With negative integers
// ----------------------
// http://bugzilla.scilab.org/15058
v = int16([-2*3*5*7  7*11  -13*17]);
LCMref = 2*3*5*7*11*13*17;
factRef = [-11*13*17, 2*3*5*13*17, -2*3*5*7*11];
[LCM, fact] = lcm(v);
assert_checkequal(LCM, LCMref);
assert_checkequal(fact, factRef);
[LCM, fact] = lcm(-v);
assert_checkequal(LCM, LCMref);
assert_checkequal(fact, -factRef);

// With decimal or encoded integers
// --------------------------------
//--> pr = primes(50)
// pr  =
//   2.  3.  5.  7.  11.  13.  17.  19.  23.  29.  31.  37.  41.  43.  47.

V = int32([2^2*3^5, 2^3*3^2, 2^2*3^4*5]);
assert_checkequal(lcm(V), 2^3*3^5*5);

v = [2*3*7, 5*7];
Lcm = 2*3*5*7;                  // 210  int8: 2^7 = 128
v = [v; [2*2*3*7*13, 5*7*11*13]];
Lcm = [Lcm 2*2*3*5*7*11*13];    // 60060. int16 : 2^15 = 32768
d =  3*7*13*29*37;
v = [v; [d*2*19*27, d*5*11*23]];
Lcm = [Lcm d*2*5*11*19*23*27];  // 3.802D+11 int32: 2^31 ~ 2.147D+09
// int64    2^51 ~2.252D+15  2^63 ~ 9.223D+18
d = 9*49*13*23*29*37*47;        // 6.650D+09
v = [v; [d*2*25*31, d*7*29]];   // 1.031D+13 ; 1.350D+12
Lcm = [Lcm  d*2*7*25*29*31];    // ~ 2.092D+15

// inttype: [1 11 2 12 4 14 8 18]
for i = 1:4
    it = 2^(i-1);
    L = lcm(iconvert([-v(i,1) v(i,2)],it));     // inttype [1 2 4 8]
    assert_checkequal(L, Lcm(i));
    L = lcm(iconvert([v(i,1) v(i,2)],it+10));   // inttype [11 12 14 18]
    assert_checkequal(L, Lcm(i));
end
assert_checkequal(lcm(v(4,:)), Lcm(4));         // inttype 0 (decimal)
