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


function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Check behaviour with default settings.
//
nm = nmplot_new ();
nm = nmplot_configure(nm,"-numberofvariables",2);
nm = nmplot_configure(nm,"-function",rosenbrock);
nm = nmplot_configure(nm,"-x0",[-1.2 1.0].');
nm = nmplot_configure(nm,"-maxiter",10);
nm = nmplot_configure(nm,"-maxfunevals",300);
nm = nmplot_configure(nm,"-tolfunrelative",10*%eps);
nm = nmplot_configure(nm,"-tolxrelative",10*%eps);
nm = nmplot_configure(nm,"-simplex0method","axes");
nm = nmplot_configure(nm,"-simplex0length",1.0);
nm = nmplot_configure(nm,"-method","variable");
nm = nmplot_configure(nm,"-simplexfn",fullfile(TMPDIR,"rosenbrock.history.simplex.txt"));
nm = nmplot_configure(nm,"-fbarfn",fullfile(TMPDIR,"rosenbrock.history.fbar.txt"));
nm = nmplot_configure(nm,"-foptfn",fullfile(TMPDIR,"rosenbrock.history.fopt.txt"));
nm = nmplot_configure(nm,"-sigmafn",fullfile(TMPDIR,"rosenbrock.history.sigma.txt"));
sfn = nmplot_cget ( nm , "-simplexfn" );
assert_checkequal ( sfn , fullfile(TMPDIR,"rosenbrock.history.simplex.txt") );
sfn = nmplot_cget ( nm , "-fbarfn" );
assert_checkequal ( sfn , fullfile(TMPDIR,"rosenbrock.history.fbar.txt") );
sfn = nmplot_cget ( nm , "-foptfn" );
assert_checkequal ( sfn , fullfile(TMPDIR,"rosenbrock.history.fopt.txt") );
sfn = nmplot_cget ( nm , "-sigmafn" );
assert_checkequal ( sfn , fullfile(TMPDIR,"rosenbrock.history.sigma.txt") );
// Check the display
disp(nm);
nm = nmplot_destroy(nm);

