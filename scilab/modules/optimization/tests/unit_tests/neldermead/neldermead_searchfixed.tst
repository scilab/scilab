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

function [ y , index ] = squarefun ( x , index )
    y = x(1)^2+x(2)^2;
endfunction

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",0.5);
nm = neldermead_configure(nm,"-method","fixed");
nm = neldermead_configure(nm,"-function",squarefun);
nm = neldermead_search(nm, "off");
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0;0.0], 1e-6 );
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 0.0, 1e-6 );
nm = neldermead_destroy(nm);

//
// Interrupt the algorithm when done.

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-method","fixed");
nm = neldermead_configure(nm,"-function",squarefun);
// Set the relative size to zero: it should never stop...
nm = neldermead_configure(nm,"-tolsimplexizerelative",0.0);
nm = neldermead_search(nm, "off");
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0;0.0], 1e-6 );
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 0.0, 1e-6 );
nm = neldermead_destroy(nm);
//
// Check verbose mode on a few iterations.

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.0 1.0]');
nm = neldermead_configure(nm,"-method","fixed");
nm = neldermead_configure(nm,"-function",squarefun);
nm = neldermead_configure(nm,"-maxiter",5);
nm = neldermead_configure(nm,"-verbose",1);
// Set the relative size to zero: it should never stop...
nm = neldermead_configure(nm,"-tolsimplexizerelative",0.0);
nm = neldermead_search(nm, "off");
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0;0.0], 1e-6 );
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 0.0, 1e-6 );
nm = neldermead_destroy(nm);

function [ y , index ] = rosenbrock ( x , index )
    y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",200);
nm = neldermead_configure(nm,"-maxfunevals",200);
nm = neldermead_configure(nm,"-tolfunrelative",10*%eps);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-method","fixed");
nm = neldermead_search(nm, "off");
// With fixed-size simplices, one cannot lead the
// simplex to the optimum.
// Check optimum point
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [1.0;1.0], 1e1 );
// Check optimum point value
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 0.0 , 1e1 );
// Check status
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "maxfuneval" );
// Cleanup
nm = neldermead_destroy(nm);

// Check that the verbose mode is functional
// Few iterations are necessary to check this
// Many iterations costs a lot more in time.
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-x0",[-1.2 1.0]');
nm = neldermead_configure(nm,"-maxiter",5);
nm = neldermead_configure(nm,"-maxfunevals",200);
nm = neldermead_configure(nm,"-tolfunrelative",10*%eps);
nm = neldermead_configure(nm,"-tolxrelative",10*%eps);
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",1.0);
nm = neldermead_configure(nm,"-method","fixed");
nm = neldermead_configure(nm,"-verbose",1);
nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_search(nm, "off");
status = neldermead_get(nm,"-status");
assert_checkequal ( status , "maxiter" );
nm = neldermead_destroy(nm);

