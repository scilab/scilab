// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- CLI SHELL MODE -->

//
// Test optimsimplex_deltafv
//

//
// Test with 3 vertices
//
s1 = optimsimplex_new ();
simplex = [
1. -2.0 1.0
4. -1.0 3.0
7. -3.0 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
df = optimsimplex_deltafv ( s1 );
assert_checkalmostequal ( df , [3.0 6.0]' , %eps );
s1 = optimsimplex_destroy ( s1 );
//
// Test with 5 vertices
//
s1 = optimsimplex_new ();
simplex = [
1. -2.0 1.0
4. -1.0 3.0
7. -3.0 2.0
9. -3.0 2.0
11. -3.0 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
df = optimsimplex_deltafv ( s1 );
assert_checkalmostequal ( df , [3.0 6.0 8.0 10.0]' , %eps );
s1 = optimsimplex_destroy ( s1 );

