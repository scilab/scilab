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



//
// The _MYDATA_ variable name is chosen so that 
// no name conflict can possibly occur.
//
function [ y , index ] = rosenbrock ( x , index )
  a = _MYDATA_.a
  y = 100*(x(2)-x(1)^2)^2 + ( a - x(1))^2;
  _MYDATA_.nb = _MYDATA_.nb + 1
endfunction

x0 = [11.0 140.0]';

//
// Test with an additional argument
//
_MYDATA_ = tlist(["T_MYSTUFF","a","nb"]);
_MYDATA_.a = 12.0;
_MYDATA_.nb = 0;

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",x0);
nm = neldermead_configure(nm,"-maxfunevals",%inf);
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_search(nm, "off");
iter = neldermead_get(nm,"-iterations");
assert_checkequal ( iter , 10 );
// Check _MYDATA_.nb
// The variable is just read, not written and the nb field
// is not updated.
assert_checkequal ( _MYDATA_.nb , 0 );
// Cleanup
nm = neldermead_destroy(nm);

//
// In this case, the mydata variable is passed
// explicitely by the neldermead class.
// So the actual name "mydata" does not matter
// and whatever variable name can be used.
//
function [ y , index ] = rosenbrock2 ( x , index , mydata )
  a = mydata.a
  y = 100*(x(2)-x(1)^2)^2 + ( a - x(1))^2;
endfunction

//
// Test with an additional argument
//
mystuff = tlist(["T_MYSTUFF","a"]);
mystuff.a = 12.0;

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",list(rosenbrock2,mystuff));
nm = neldermead_configure(nm,"-x0",x0);
nm = neldermead_configure(nm,"-maxfunevals",%inf);
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_search(nm, "off");
iter = neldermead_get(nm,"-iterations");
assert_checkequal ( iter , 10 );
nm = neldermead_destroy(nm);

//
// Use a global variable.
//
function [ y , index ] = rosenbrock3 ( x , index )
  global _MYDATA_
  a = _MYDATA_.a
  y = 100*(x(2)-x(1)^2)^2 + ( a - x(1))^2;
  _MYDATA_.nb = _MYDATA_.nb + 1
endfunction

//
// Test with an additional argument
//
global _MYDATA_
_MYDATA_ = tlist(["T_MYSTUFF","a","nb"]);
_MYDATA_.a = 12.0;
_MYDATA_.nb = 0;

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock3);
nm = neldermead_configure(nm,"-x0",x0);
nm = neldermead_configure(nm,"-maxfunevals",%inf);
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_search(nm, "off");
iter = neldermead_get(nm,"-iterations");
assert_checkequal ( iter , 10 );
// Check _MYDATA_.nb
// The variable is not just read, it is also written and the nb field
// IS updated.
assert_checkequal ( _MYDATA_.nb > 10 , %T );
nm = neldermead_destroy(nm);




