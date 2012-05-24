// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->


// unit tests for gamma() function
// =============================================================================

assert_checkequal ( isnan(gamma(-1.e308)) , %T );
assert_checkalmostequal ( gamma(-170.6) ,  -2.082900836150297478e-308 , 1.e2 * %eps );
assert_checkalmostequal ( gamma(-1.5) , 2.36327180120735480529 , %eps );
assert_checkequal ( isnan(gamma(-2.)) , %T );
assert_checkequal ( isnan(gamma(-1.)) , %T );
assert_checkalmostequal ( gamma(-0.5) , -3.5449077018110317638389 , %eps );
assert_checkequal ( gamma(0.0) , %inf );
assert_checkalmostequal ( gamma(1.e-307) , 1.e307 , %eps );
assert_checkalmostequal ( gamma(1.e-100) , 1.e100 , %eps );
assert_checkalmostequal ( gamma(1.e-30) , 1.e30 , %eps );
assert_checkalmostequal ( gamma(0.5) , 1.7724538509055161039640 , %eps );
assert_checkequal ( gamma(1.0) , 1.0 );
assert_checkequal ( gamma(2.0) , 1.0 );
assert_checkequal ( gamma(3.0) , 2.0 );
assert_checkequal ( gamma(4.0) , 6.0 );
assert_checkequal ( gamma(5.0) , 24.0 );
assert_checkequal ( gamma(10.0) , 362880.0 );
assert_checkalmostequal ( gamma(13.0) , 479001600.0 , 1000 * %eps );
assert_checkalmostequal ( gamma(171) , 7.257415615308056415e+306 , 1.e3 * %eps );
assert_checkequal ( gamma(172) , %inf );
assert_checkequal ( gamma(%inf) , %inf );
assert_checkequal ( gamma(-%inf) , %inf );

