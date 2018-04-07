// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
// Copyright (C) 2018 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
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

x = factorial ( 1 );
assert_checkequal ( x , 1 );
x = factorial ( [1 2 3 4] );
assert_checkequal ( x , [1 2 6 24] );
x = factorial ( [1 2 3 4]' );
assert_checkequal ( x , [1 2 6 24]' );
x = factorial ( 170 );
assert_checkalmostequal ( x , 7.25741561530799896739e306 , 10 * %eps );
// Test with a matrix
n = [
1 2 3
4 5 6
7 8 9
];
x = factorial ( n );
e = [
    1.       2.        6.
    24.      120.      720.
    5040.    40320.    362880.
];
assert_checkequal ( x , e );

// Test with an hypermatrix
clear n expected
n(2,2,2) = 1;
n(1:8) = 1:8;
expected(2,2,2) = 1;
expected(1:8) = e'(1:8)
assert_checkequal(factorial(n), expected);

// -----------
// Accurate values from Wolfram Alpha
expected = [
   1.
   1.
   2.
   6.
   24.
   120.
   720.
   5040.
   40320.
   362880.
   3628800.
   39916800.
   479001600.
   6227020800.
   87178291200.
   1307674368000.
   20922789888000.
   355687428096000.
   6402373705728000.
   121645100408832000.
   2432902008176640000.
   51090942171709440000.
   1124000727777607700000.
   25852016738884978000000.
 ];
assert_checkequal(factorial(0:23)', expected);

expected = [
   2.652528598121910D+32     8.1591528324789774D+47   3.04140932017133780D+64 ..
   1.19785716699698918D+100  9.3326215443944153D+157  5.7133839564458546D+262
   ];
assert_checkalmostequal(factorial([30 40 50 70 100 150]), expected, 5*%eps);

[f, p, m] = factorial(171);
assert_checkequal(f, %inf);
assert_checkequal(p, 309);
assert_checkalmostequal(m, 1.2410180702176678, 1.5e-13); // 2e-14

[f, p, m] = factorial(300);
assert_checkequal(f, %inf);
assert_checkequal(p, 614);
assert_checkalmostequal(m, 3.060575122164406360, 2e-13); // 1e-13

[f, p, m] = factorial(200:100:900);
m_exp = [7.886578673647905036  3.06057512216440636  6.40345228466238953 ..
         1.220136825991110068  1.26557231622543074  2.42204012475027218 ..
         7.710530113353860041  6.75268022096458416
        ];
p_exp = [ 374  614  868  1134  1408  1689  1976  2269  ];
assert_checkequal(p, p_exp);
assert_checkalmostequal(m, m_exp, 5e-13);

[f, p, m] = factorial(1000:1000:1e4);
m_exp = [
   4.02387260077093774  ..
   3.31627509245063324   4.149359603437854086   1.828801951514065013 ..
   4.22857792660554352   2.683999765726739596   8.842007956963112247 ..
   5.18418106048087694   8.099589986687190858   2.846259680917054519
    ];
p_exp = [ 2567  5735  9130  12673  16325  20065  23877  27752  31681  35659 ];
assert_checkequal(p, p_exp);
assert_checkalmostequal(m, m_exp, 6e-12);  // 1.2e-11

[f, p, m] = factorial(2e4:1e4:1e5);
m_exp = [
   1.81920632023034513   2.75953724621938460   2.09169242212132363  ..
   3.34732050959714483   1.56413770802606692   1.17681241537969008 ..
   3.09772225166224929   1.58011915409779537   2.82422940796034787
    ];
p_exp = [ 77337  121287  166713  213236  260634  308759  357506  406798  456573 ];
assert_checkequal(p, p_exp);
assert_checkalmostequal(m, m_exp, 1.5e-10);  //3e-10

[f, p, m] = factorial(round(10^(6:14)));
m_exp = [
   8.26393168833124006  1.202423340051590346  1.61720379492146239  ..
   9.90462657922299373  2.325796205673083365  3.74892859910502696  ..
   1.40366116037375609  2.403330084340115344  1.64560205598729788
   ];
p_exp = [5565708  65657059  756570556  8565705522  95657055186 ..
         1056570551815  11565705518103  125657055180974  1356570551809682
        ];
//r_err = [3e-9 3e-8 3e-7 2e-6 5e-6 4e-4 5e-3 7e-2 2e-1]; // with 1st implementation
//r_err = [3e-9 4e-8 2e-7 2e-6 3e-5 2e-4 2e-3 5e-2 8e-2]; // with gammaln(n+1)
r_err  = [1e-10 6e-8 2e-8 2e-7 2e-6 7e-6 2e-4 5e-4 8e-3];
for i = 1:length(m_exp)
    assert_checkequal(p(i), p_exp(i));
    assert_checkalmostequal(m(i), m_exp(i), r_err(i));
end
