// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2011 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->

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
nm = neldermead_search(nm);
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
nm = neldermead_configure(nm,"-verbose",1);
// Set the relative size to zero: it should never stop...
nm = neldermead_configure(nm,"-tolsimplexizerelative",0.0);
nm = neldermead_search(nm);
xopt = neldermead_get(nm,"-xopt");
assert_checkalmostequal ( xopt , [0.0;0.0], 1e-6 );
fopt = neldermead_get(nm,"-fopt");
assert_checkalmostequal ( fopt , 0.0, 1e-6 );
nm = neldermead_destroy(nm);

