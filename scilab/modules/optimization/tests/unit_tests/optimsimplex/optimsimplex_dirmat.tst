// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->

// Test optimsimplex_dirmat

// Test with 3 vertices
s1 = optimsimplex_new ();
simplex = [
1. -2.0 1.0
4. -1.0 3.0
7. -3.0 2.0
];
s1 = optimsimplex_setall ( s1 , simplex );
m = optimsimplex_dirmat ( s1 );
assert_checkalmostequal ( m , [1. -1.;2. 1.], %eps );
s1 = optimsimplex_destroy ( s1 );

// Test with 5 vertices
s1 = optimsimplex_new ();
simplex = [
1. -2.0 1.0
4. -1.0 3.0
7. -3.0 2.0
9. -5.0 3.0
11. -7.0 4.0
];
s1 = optimsimplex_setall ( s1 , simplex );
m = optimsimplex_dirmat ( s1 );
assert_checkalmostequal ( m , [
1. -1. -3. -5.0 
2. 1. 2.0 3.0
], %eps );
s1 = optimsimplex_destroy ( s1 );

