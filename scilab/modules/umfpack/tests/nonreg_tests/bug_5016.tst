// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 5016 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5016
//
// <-- Short Description -->
// condestsp could yield different results when repeated calls were performed.

A = [
1.7478328000000000752578  -0.1903736000000000039289 1.3090182999999999680085  -1.3628348999999999602295 0.3444064000000000014268
0.2762794000000000083084  -1.0118259000000000558117 1.1937549000000000631871  -0.3618465000000000153513 1.417060200000000103060
-1.0079914000000000928736 -1.2770016                -0.9433875999999999928392 -0.1351713999999999971102 -0.1938765000000000071623
-0.4281269999999999797957 -1.5414133000000000972562 0.5169858000000000508578  0.9329383999999999455000  -1.2854697999999999957765
-0.5434953999999999618709 -0.3191552999999999751246 0.8207168000000000240846  -1.0325953999999999410875 -1.1262562000000000406175
];
function c = cond1 ( A )
    SA = sparse ( A );
    c = condestsp ( SA );
endfunction

refCond = 7.0462354;

Cond = cond1 ( A );
assert_checkalmostequal(Cond, refCond, [], 1d-6);
Cond = cond1 ( A );
assert_checkalmostequal(Cond, refCond, [], 1d-6);
Cond = cond1 ( A );
assert_checkalmostequal(Cond, refCond, [], 1d-6);
Cond = cond1 ( A );
assert_checkalmostequal(Cond, refCond, [], 1d-6);
Cond = cond1 ( A );
assert_checkalmostequal(Cond, refCond, [], 1d-6);
