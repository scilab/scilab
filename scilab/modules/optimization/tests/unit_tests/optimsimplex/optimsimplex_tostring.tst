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
if getos() == 'Windows' then
expected = [
"Optim Simplex Object:"
"====================="
"Vertex #1/3 : fv=2.400000e+001, x=-2.000000e+000 1.000000e+000"
"Vertex #2/3 : fv=9.300000e+001, x=-1.000000e+000 3.000000e+000"
"Vertex #3/3 : fv=3.600000e+001, x=-3.000000e+000 2.000000e+000"
];
else
expected = [
"Optim Simplex Object:"
"====================="
"Vertex #1/3 : fv=2.400000e+01, x=-2.000000e+00 1.000000e+00"
"Vertex #2/3 : fv=9.300000e+01, x=-1.000000e+00 3.000000e+00"
"Vertex #3/3 : fv=3.600000e+01, x=-3.000000e+00 2.000000e+00"
];
end
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
if getos() == 'Windows' then
expected = [
"Optim Simplex Object:"
"====================="
"Vertex #1/4 : fv=2.400000e+001, x=-2.000000e+000 1.000000e+000" 
"Vertex #2/4 : fv=9.300000e+001, x=-1.000000e+000 3.000000e+000" 
"Vertex #3/4 : fv=3.600000e+001, x=-3.000000e+000 2.000000e+000" 
"Vertex #4/4 : fv=3.600000e+001, x=-3.000000e+000 2.000000e+000" 
];
else
expected = [
"Optim Simplex Object:"
"====================="
"Vertex #1/4 : fv=2.400000e+01, x=-2.000000e+00 1.000000e+00" 
"Vertex #2/4 : fv=9.300000e+01, x=-1.000000e+00 3.000000e+00" 
"Vertex #3/4 : fv=3.600000e+01, x=-3.000000e+00 2.000000e+00" 
"Vertex #4/4 : fv=3.600000e+01, x=-3.000000e+00 2.000000e+00" 
];
end
assert_checkequal ( str , expected );
s1 = optimsimplex_destroy ( s1 );

