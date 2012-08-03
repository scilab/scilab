// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

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
// Check backward-compatibility #1:
// without stop output argument.
//

function myoutputcmd3 ( state , data , mystuff )
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
  _OUTPUCMDFLAG_ = mystuff.myarg
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
nm = neldermead_configure(nm,"-outputcommand",list(myoutputcmd3,myobj));
nm = neldermead_search(nm);
iter = neldermead_get(nm,"-iterations");
assert_checkequal ( iter , 10 );
assert_checkequal ( _OUTPUCMDFLAG_ , 12 );
nm = neldermead_destroy(nm);

//
// Check backward-compatibility #2:
// obsolete use of -outputcommandarg option.
//

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
nm = neldermead_configure(nm,"-outputcommand",myoutputcmd2);
nm = neldermead_configure(nm,"-outputcommandarg",myobj);
nm = neldermead_search(nm);
iter = neldermead_get(nm,"-iterations");
assert_checkequal ( iter , 10 );
assert_checkequal ( _OUTPUCMDFLAG_ , 12 );
nm = neldermead_destroy(nm);

