// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- CLI SHELL MODE -->

//
// Check behaviour with default settings.
//

function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test optimsimplex_size with 3 vertices
//
simplex = [...
    0.    0.  
    1.    0.  
    0.    2.  
];
s1 = optimsimplex_new();
s1 = optimsimplex_setn ( s1 , 2 );
s1 = optimsimplex_setnbve ( s1 , 3 );
s1 = optimsimplex_setallx ( s1 , simplex );
ssize = optimsimplex_size ( s1 , "Nash" );
assert_checkalmostequal ( ssize , 3.0, %eps );
ssize = optimsimplex_size ( s1 , "diameter" );
assert_checkalmostequal ( ssize , sqrt(5), %eps );
ssize = optimsimplex_size ( s1 , "sigmaplus" );
assert_checkalmostequal ( ssize , 2.0, %eps );
ssize = optimsimplex_size ( s1 , "sigmaminus" );
assert_checkalmostequal ( ssize , 1.0, %eps );
s1 = optimsimplex_destroy(s1);
//
// Test optimsimplex_size with 5 vertices
//
simplex = [...
0.0 0.0 0.0
0.0 1.0 0.0
0.0 0.0 2.0
0.0 1.0 2.0
0.0 2.0 2.0
];
s1 = optimsimplex_new();
s1 = optimsimplex_setall ( s1 , simplex );
ssize = optimsimplex_size ( s1 , "Nash" );
assert_checkalmostequal ( ssize , 10.0 , %eps );
ssize = optimsimplex_size ( s1 , "diameter" );
assert_checkalmostequal ( ssize , 2.8284271247461902909492 , %eps );
ssize = optimsimplex_size ( s1 , "sigmaplus" );
assert_checkalmostequal ( ssize , 2.8284271247461902909492 , %eps );
ssize = optimsimplex_size ( s1 , "sigmaminus" );
assert_checkalmostequal ( ssize , 1.0 , %eps );
s1 = optimsimplex_destroy(s1);

// Test unknown size method
s1 = optimsimplex_new();
cmd = "optimsimplex_size ( s1 , ''foo'' )";
assert_checkerror(cmd,"%s: Unknown simplex size method %s" , [],"optimsimplex_size","foo");
s1 = optimsimplex_destroy(s1);

