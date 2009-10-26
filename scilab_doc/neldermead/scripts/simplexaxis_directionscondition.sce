// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Create a new axis-by-axis simplex and compute the 
// matrix of directions and its condition number
x0 = [0.0 0.0];
si = optimsimplex_new ( "axes" , x0 );
D = optimsimplex_dirmat ( si )
k = cond(D)
optimsimplex_destroy(si);

// Create a flat simplex and compute its condition number.
coords = [
0.0 0.0
1.0 0.0
0.5 1.e-10
];
si = optimsimplex_new ( coords );
D = optimsimplex_dirmat ( si )
k = cond(D)
optimsimplex_destroy(si);



