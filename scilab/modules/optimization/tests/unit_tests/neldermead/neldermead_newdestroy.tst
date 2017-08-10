// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
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
// <-- ENGLISH IMPOSED -->




//
// Test basic new/destroy sequence
//
nm = neldermead_new ();
nm = neldermead_destroy(nm);

//
// Test printing system
//
nm = neldermead_new ();
str = string(nm)
assert_checkequal ( typeof(str) , "string" )
nm
nm = neldermead_destroy(nm);

//
// optimtestcase --
//   Non linear inequality constraints are positive.
//    
// Arguments
//   x: the point where to compute the function
//   index : the stuff to compute
//
function [ f , c , index ] = optimtestcase ( x , index )
  f = []
  c = []
  if ( index == 2 | index == 6 ) then
    f = x(1)^2 + x(2)^2 + 2.0 * x(3)^2 + x(4)^2 ...
      - 5.0 * x(1) - 5.0 * x(2) - 21.0 * x(3) + 7.0 * x(4)
  end
  if ( index == 5 | index == 6 ) then
    c1 = - x(1)^2 - x(2)^2 - x(3)^2 - x(4)^2 ...
              - x(1) + x(2) - x(3) + x(4) + 8
    c2 = - x(1)^2 - 2.0 * x(2)^2 - x(3)^2 - 2.0 * x(4)^2 ...
              + x(1) + x(4) + 10.0
    c3 = - 2.0 * x(1)^2 - x(2)^2 - x(3)^2 - 2.0 * x(1) ...
              + x(2) + x(4) + 5.0
    c = [c1 c2 c3]
  end
endfunction


//
// Test printing system with various options configured
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",4);
nm = neldermead_configure(nm,"-function",optimtestcase);
nm = neldermead_configure(nm,"-x0",[0.0 0.0 0.0 0.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",1000);
nm = neldermead_configure(nm,"-tolsimplexizerelative",1.e-4);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-nbineqconst",3);
nm = neldermead_configure(nm,"-simplex0length",20.0);
str = string(nm)
assert_checkequal ( typeof(str) , "string" )
nm
nm = neldermead_destroy(nm);

