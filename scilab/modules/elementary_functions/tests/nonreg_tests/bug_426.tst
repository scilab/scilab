// <-- Non-regression test for bug 426 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=426
//
// <-- Short Description -->
// Wrong permutation
// Scilab Project - Sylvestre LEDRU
// Copyright INRIA 2008
// Date : March 2008
a = ['aaa';'eee';'ccc';'bbb';'ddd';'rrr'];
trueValue=[1; 4; 3; 5; 2; 6];

[a result] = sort(a)
if result <> trueValue then pause,end

