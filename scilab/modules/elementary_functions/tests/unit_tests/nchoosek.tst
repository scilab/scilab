// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2010 - DIGITEO - Michael Baudin
// Copyright (C) 2019 - Samuel GOUGEON - Le Mans Université
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// ===========================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->
//
// <-- Description -->
// -------------------------
// Unit tests for nchoosek()
// -------------------------

assert_checkequal( nchoosek( 4 , 1 ) , 4 );
assert_checkequal( nchoosek( 5 , 0 ) , 1 );
assert_checkequal( nchoosek( 5 , 1 ) , 5 );
assert_checkequal( nchoosek( 5 , 2 ) , 10 );
assert_checkequal( nchoosek( 5 , 3 ) , 10 );
assert_checkequal( nchoosek( 5 , 4 ) , 5 );
assert_checkequal( nchoosek( 5 , 5 ) , 1 );
assert_checkalmostequal ( nchoosek( 10000 , 134 ) , 2.050083865033972676e307 , 1.e-13 );
assert_checkequal( nchoosek(10,0:10) , [1,10,45,120,210,252,210,120,45,10,1] );
assert_checkequal( nchoosek(1:6,0:5) , 1:6 );

// Special cases
// -------------
// C(n,1) = n
assert_checkalmostequal( nchoosek(1e20, 1), 1e20, 1e-15 );
assert_checkalmostequal( nchoosek(1e200, 1), 1e200, 1e-13 );
// C(n,n) = 1
assert_checkalmostequal( nchoosek(1e20, 1e20), 1, %eps);
assert_checkalmostequal( nchoosek(1e200, 1e200), 1, %eps);
// C(n,n-1) = n : limit case
assert_checkalmostequal( nchoosek(1/%eps, 1/%eps-1), 1/%eps, 20*%eps);
// For any k < n*%eps, numerically C(n,k) = n^k/k!
assert_checkalmostequal( nchoosek(1e20, 2), 1e40/2, 1e-13 );
assert_checkalmostequal( nchoosek(1e100, 2),1e200/2, 1e-13 );
assert_checkalmostequal( nchoosek(1e20, 3), 1e60/6, 1e-13 );
assert_checkalmostequal( nchoosek(1e50, 3), 1e150/6, 1e-13 );

// Log argout / Huge values
// ------------------------
// Refs from  W o l f r a m  alpha
[logb, b] = nchoosek(1029, 514);
ref = [1.42982068649890408D+308, 308.1552815761035049036];
assert_checkalmostequal(b, ref(1), 1e-13);
assert_checkalmostequal(logb, ref(2), 0);
logb = nchoosek(1029, 514, "10.mant");
assert_checkalmostequal(logb, 308.1429820686498904, 1e-15);
[logb, b] = nchoosek(1030, 515);
assert_checkalmostequal(b, %inf);
assert_checkalmostequal(logb, 308.45631157176751324655, 1e-15);
logb = nchoosek(1030, 515, "10.mant");
assert_checkalmostequal(logb, 308.28596413729978081637, 1e-15);

// 1e14, 1e4 : 33.51338111119899206051 × 10^104340
logb = nchoosek(1e14, 1e4, "10.mant");
assert_checkalmostequal(logb, 104340.35133826920627, 1e-6);
// 1e13, 100 : 1.071510287595069331e1142
logb = nchoosek(1e13, 100, "log10");
assert_checkalmostequal(logb, 1142.0299963450692358, 1e-5);
logb = nchoosek(1e12, 100, "log10");
assert_checkalmostequal(logb, 1042.0299963431344539, 1e-6);
// 1e11, 100  : 1.071510235085708956115 × 10^942
logb = nchoosek(1e11, 100, "10.mant");
assert_checkalmostequal(logb, 942.10715102350857089, 1e-8);
// 1e11, 1000 : 2.485155729882870340071 × 10^8432
logb = nchoosek(1e11, 1000, "10.mant");
assert_checkalmostequal(logb, 8432.24851557298828703, 1e-8);
// 1e11, 10000 : 3.51162679090294954661 × 10^74340
logb = nchoosek(1e11, 10000, "10.mant");
assert_checkalmostequal(logb, 74340.3511626790902950, 1e-8);
// 1e11, 1e5 : 3.3681041688073051733304 × 10^643426
logb = nchoosek(1e11, 1e5, "10.mant");
assert_checkalmostequal(logb, 643426.336810416880730, 1e-9);
// 1e11, 1e6 : 8.1533447321246152778592 × 10^5434288
logb = nchoosek(1e11, 1e6, "10.mant");
assert_checkalmostequal(logb, 5434288.8153344732124, 1e-10);

// Check error cases
// -----------------
msg = "nchoosek: Wrong number of input arguments: 2 or 3 expected."
assert_checkerror("nchoosek()", msg);
assert_checkerror("nchoosek([])", msg );
assert_checkerror("nchoosek(4)", msg );
assert_checkerror("[logb, b]=nchoosek()", msg);
assert_checkerror("[logb, b]=nchoosek(4)", msg );

msg = "nchoosek: Arguments #1 and #2: Incompatible sizes.";
assert_checkerror("nchoosek(10:12, 2:6)", msg );

msg = "nchoosek: Argument #1: Decimal integer expected.";
assert_checkerror("nchoosek( [4.5 1.5], [2 1])", msg);
assert_checkerror("nchoosek( [4.5 1.5], [2 -1])", msg);
msg = "nchoosek: Argument #2: Decimal integer expected.";
assert_checkerror("nchoosek( [4  3], [2.5 1])", msg );
assert_checkerror("nchoosek( [4  3], ""log10"")", msg );

msg = "nchoosek: Argument #1: Non-negative integers expected.";
assert_checkerror("nchoosek( [-4 3], [2  1])", msg);
assert_checkerror("nchoosek( [-4 3], [2 -1])", msg);
msg = "nchoosek: Argument #2: Non-negative integers expected.";
assert_checkerror("nchoosek( [4 3], [2 -1])", msg);

assert_checkerror("nchoosek( 17 , 18 )", "nchoosek: n(1) < k(1) is forbidden." );
