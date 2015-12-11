// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

x = [0.2113249 0.0002211 0.6653811; 0.7560439 0.9546254 0.6283918];
fre = [1 2 3; 3 4 3];

refM = 0.63623244375;
refV = 0.096057419504;
[v, m] = variancef(x, fre);
assert_checkalmostequal([v m], [refV refM]);

refM = [0.61986415 0.636490633333 0.64688645];
refV = [0.074179697240 0.242903351429 0.000410462494];
[v, m] = variancef(x, fre, "r");
assert_checkalmostequal([v m], [refV refM]);

refM = [0.367985066667; 0.79718087];
refV = [0.112075256021; 0.021077902385];
[v, m] = variancef(x, fre, "c");
assert_checkalmostequal([v m], [refV refM]);

// With the a priori mean
refM = 0.63623244375;
refV = 0.090053830785;
[v, m] = variancef(x, fre, "*", meanf(x,fre));
assert_checkalmostequal([v m], [refV refM]);

refM = [0.61986415 0.636490633333 0.64688645];
refV = [0.055634772930 0.202419459524 0.000342052079];
[v, m] = variancef(x, fre, "r", meanf(x,fre,"r"));
assert_checkalmostequal([v m], [refV refM]);

refM = [0.367985066667; 0.79718087];
refV = [0.093396046684; 0.018970112146];
[v, m] = variancef(x, fre, "c", meanf(x,fre,"c"));
assert_checkalmostequal([v m], [refV refM]);

// Biased variance
refM = 0.63623244375;
refV = 0.090053830785;
[v, m] = variancef(x, fre, "*", %nan);
assert_checkalmostequal([v m], [refV refM]);
