// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
// <-- CLI SHELL MODE -->


// This test is designed to produce a warning:
// this warning is localized.
// This is why we do not check the ref.
// Checking the ref file under Linux fails, because
// <-- ENGLISH IMPOSED --> has no effect there.
// See : http://bugzilla.scilab.org/show_bug.cgi?id=9284

function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction



//
// Ensure backward compatibility.
// Check that it works also without a stop output variable.
//
function outfun4 ( x , optimValues , state )
endfunction
opt = optimset ( "OutputFcn" , outfun4);
opt = optimset ( opt , "MaxIter" , 10 );
[x fval] = fminsearch ( rosenbrock , [-1.2 1] , opt );

