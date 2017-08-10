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
// <-- ENGLISH IMPOSED -->


function [ y , index ] = myquad ( x , index )
  y = x(1)^2 + x(2)^2
endfunction


//
// myoutputcmd --
//  This command is called back by the Nelder-Mead
//  algorithm.
// Arguments
//  state : the current state of the algorithm
//    "init", "iter", "done"
//  data : the data at the current state
//    This is a tlist with the following entries:
//    * x : the optimal vector of parameters
//    * fval : the minimum function value
//    * simplex : the simplex, as a simplex object
//    * iteration : the number of iterations performed
//    * funccount : the number of function evaluations
//    * step : the type of step in the previous iteration
// stop: set to true to stop algorithm
//
function stop = myoutputcmd ( state , data )
  global _OUTPUCMDFLAG_
  // Unload the array, just to make sure that the minimum is there
  iter = data.iteration
  fc = data.funccount
  fval = data.fval
  x = data.x
  simplex = data.simplex
  step = data.step
  // Simplex is a data structure, which can be managed
  // by the simplex class.
  v = optimsimplex_dirmat ( simplex )
  condv = cond ( v )
  _OUTPUCMDFLAG_ = 1
  stop = %f
endfunction


//
// Test the variable algorithm
//
global _OUTPUCMDFLAG_;
_OUTPUCMDFLAG_ = 0;

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",myquad);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-outputcommand",myoutputcmd);
nm = neldermead_search(nm, "off");
iter = neldermead_get(nm,"-iterations");
assert_checkequal ( iter , 10 );
// We are here, that means that the output command has been correctly
// called
assert_checkequal ( _OUTPUCMDFLAG_ , 1 );
nm = neldermead_destroy(nm);


//
// Test the fixed algorithm
//
global _OUTPUCMDFLAG_;
_OUTPUCMDFLAG_ = 0;

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",myquad);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_configure(nm,"-method","fixed");
nm = neldermead_configure(nm,"-outputcommand",myoutputcmd);
nm = neldermead_search(nm, "off");
iter = neldermead_get(nm,"-iterations");
assert_checkequal ( iter , 10 );
// We are here, that means that the output command has been correctly
// called
assert_checkequal ( _OUTPUCMDFLAG_ , 1 );
nm = neldermead_destroy(nm);


//
// Test the Box algorithm
//
global _OUTPUCMDFLAG_;
_OUTPUCMDFLAG_ = 0;

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",myquad);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_configure(nm,"-method","box");
nm = neldermead_configure(nm,"-boundsmin",[-10.0 -10.0]);
nm = neldermead_configure(nm,"-boundsmax",[10.0 10.0]);
nm = neldermead_configure(nm,"-outputcommand",myoutputcmd);
nm = neldermead_search(nm, "off");
iter = neldermead_get(nm,"-iterations");
assert_checkequal ( iter , 10 );
// We are here, that means that the output command has been correctly
// called
assert_checkequal ( _OUTPUCMDFLAG_ , 1 );
nm = neldermead_destroy(nm);


function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// myoutputcmd2 --
//  This command is called back by the Nelder-Mead
//  algorithm.
// Arguments
//  state : the current state of the algorithm
//    "init", "iter", "done"
//  data : the data at the current state
//    This is a tlist with the following entries:
//    * x : the optimal vector of parameters
//    * fval : the minimum function value
//    * simplex : the simplex, as a simplex object
//    * iteration : the number of iterations performed
//    * funccount : the number of function evaluations
//  myobj : a user-defined data structure
// stop: set to true to stop algorithm
//
function stop = myoutputcmd2 ( state , data , myobj )
  global _OUTPUCMDFLAG_
  // Unload the array, just to make sure that the minimum is there
  iter = data.iteration
  fc = data.funccount
  fval = data.fval
  x = data.x
  simplex = data.simplex
  // Simplex is a data structure, which can be managed
  // by the simplex class.
  v = optimsimplex_dirmat ( simplex )
  condv = cond ( v )
  _OUTPUCMDFLAG_ = myobj.myarg
  stop = %f
endfunction

global _OUTPUCMDFLAG_;
_OUTPUCMDFLAG_ = 0;

myobj = tlist(["T_MYSTUFF","myarg"]);
myobj.myarg = 12;

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-outputcommand",list(myoutputcmd2,myobj));
nm = neldermead_search(nm, "off");
iter = neldermead_get(nm,"-iterations");
assert_checkequal ( iter , 10 );
assert_checkequal ( _OUTPUCMDFLAG_ , 12 );
nm = neldermead_destroy(nm);



