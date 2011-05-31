// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->


// Test with 3 vertices
s1 = optimsimplex_new ();
simplex = [
24. -2.0 1.0
93. -1.0 3.0
36. -3.0 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
str = string ( s1 );
expected = [
"Optim Simplex Object:"
"====================="
"Vertex #1/3 : fv=24, x=-2 1"
"Vertex #2/3 : fv=93, x=-1 3"
"Vertex #3/3 : fv=36, x=-3 2"
];
assert_checkequal ( str , expected );
s1 = optimsimplex_destroy ( s1 );

// Test with 4 vertices
s1 = optimsimplex_new ();
simplex = [
24. -2.0 1.0
93. -1.0 3.0
36. -3.0 2.0
36. -3.0 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
str = string ( s1 );
expected = [
"Optim Simplex Object:"
"====================="
"Vertex #1/4 : fv=24, x=-2 1"
"Vertex #2/4 : fv=93, x=-1 3"
"Vertex #3/4 : fv=36, x=-3 2"
"Vertex #4/4 : fv=36, x=-3 2" 
];
assert_checkequal ( str , expected );
s1 = optimsimplex_destroy ( s1 );

