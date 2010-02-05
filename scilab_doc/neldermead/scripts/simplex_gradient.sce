// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Create a new axis-by-axis simplex and compute the 
// simplex gradient
function y = myfunction ( x )
  y = x(1)^2 + x(2)^2
endfunction
x0 = [1.0 1.0];
len = 1.e-3;
si = optimsimplex_new ( "axes" , x0 , myfunction , len );
sg = optimsimplex_gradientfv ( si );
mprintf ( "Simplex Gradient=(%f %f)^T\n",sg(1),sg(2));
eg = [2 * x0(1) 2 * x0(2)].';
mprintf ( "Exact Gradient=(%f %f)^T\n",eg(1),eg(2));
err = norm(sg-eg)/norm(eg);
mprintf ( "Relative Error = %e\n",err);
err = norm(sg-eg);
mprintf ( "Absolute Error = %e\n",err);
D = optimsimplex_dirmat ( si );
k = cond(D);
mprintf ( "k(D)=%f\n",k);
ss = optimsimplex_size ( si );
mprintf ( "sigma_+(D)=%e\n",ss);
optimsimplex_destroy(si);

// Create a flat simplex and compute the simplex gradient.
R = 0.5e-3
coords = [
  1.0 1.0
  1.0+1.e-3 1.0
];
for theta = [90.0 10.0 1.0 0.1 0.01 0.001]
  C(1,1) = 1.0 + R * cos(theta*%pi/180);
  C(1,2) = 1.0 + R * sin(theta*%pi/180);
  coords(3,1:2) = C;
  si = optimsimplex_new ( coords , myfunction );
  sg = optimsimplex_gradientfv ( si );
  eg = [2 * x0(1) 2 * x0(2)].';
  err = norm(sg-eg);
  D = optimsimplex_dirmat ( si );
  k = cond(D);
  ss = optimsimplex_size ( si );
  mprintf ( "%f & %e & %e & %e \\\\\n",theta , ss , err , k);
  optimsimplex_destroy(si);
end


