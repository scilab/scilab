// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Create a new simplex by coordinates and compute the 
// norm of the matrix of directions and its oriented length
coords = [
0.0 0.0
1.0 0.5
1.0 2.0
];
si = optimsimplex_new ( coords );
D = optimsimplex_dirmat ( si )
for i=1:2
  nd = norm(D(1:2,i),2);
  mprintf( "||d_%d||=%f\n",i,nd)
end
ss = optimsimplex_size ( si , "sigmaplus" );
mprintf( "sigma_+(S)=%f\n",ss);
normmatrix = norm(D);
mprintf( "||D||=%f\n",normmatrix);
optimsimplex_destroy(si);

