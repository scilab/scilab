// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - Digiteo - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Basic use
format(25)
function y = banana (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction
[x , fval , exitflag , output] = fminsearch ( banana , [-1.2 1] )
output.message
// Print the result with 15 significant digits
mprintf ( "%.15e" , fval );
mprintf ( "%.15e %.15e" , x(1) , x(2) );
// Compare the result
xmb = [1.000022021783570   1.000042219751772 ];
err = norm(x - xmb) / norm(xmb);
mprintf ( "Relative Error on x : %e\n", err );
fmb = 8.177661197416674e-10;
err = abs(fval - fmb) / abs(fmb);
mprintf ( "Relative Error on f : %e\n", err );

// Now check each iteration
opt = optimset ( "Display" , "iter" );
[x , fval , exitflag , output] = fminsearch ( banana , [-1.2 1] , opt );

// Check that the output functions are OK
function outfun ( x , optimValues , state )
  plot( x(1),x(2),'.');
endfunction
opt = optimset ( "OutputFcn" , outfun );
[x fval] = fminsearch ( banana , [-1.2 1] , opt );


// Check that the optimplotfval functions is OK
opt = optimset ( "OutputFcn" , optimplotfval );
[x fval] = fminsearch ( banana , [-1.2 1] , opt );



