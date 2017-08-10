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

//
// Check behaviour with configured settings.
//


function [ y , index ] = rosenbrock ( x , index )
    y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Check maxfunevals with "variable" method
//
nm = neldermead_new ();
//
nm = neldermead_configure(nm,"-numberofvariables",2);
numvar = neldermead_cget(nm,"-numberofvariables");
assert_checkequal ( numvar , 2 );
//
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
x0 = neldermead_cget(nm,"-x0");
assert_checkequal ( x0 , [1.1 1.1]' );
// Check default -simplex0method
simplex0method = neldermead_cget(nm,"-simplex0method");
assert_checkequal ( simplex0method , "axes" );
//
nm = neldermead_configure(nm,"-simplex0method","spendley");
simplex0method = neldermead_cget(nm,"-simplex0method");
assert_checkequal ( simplex0method , "spendley" );
// Check default -simplex0length
simplex0length = neldermead_cget(nm,"-simplex0length");
assert_checkequal ( simplex0length , 1 );
//
nm = neldermead_configure(nm,"-simplex0length",0.1);
simplex0length = neldermead_cget(nm,"-simplex0length");
assert_checkequal ( simplex0length , 0.1 );
//
// Check default -method
method = neldermead_cget(nm,"-method");
assert_checkequal ( method , "variable" );
//
nm = neldermead_configure(nm,"-method","fixed");
method = neldermead_cget(nm,"-method");
assert_checkequal ( method , "fixed" );
//
nm = neldermead_configure(nm,"-method","variable");
method = neldermead_cget(nm,"-method");
assert_checkequal ( method , "variable" );
//
nm = neldermead_configure(nm,"-function",rosenbrock);
//
nm = neldermead_configure(nm,"-maxfunevals",10);
maxfunevals = neldermead_cget(nm,"-maxfunevals");
assert_checkequal ( maxfunevals , 10 );
//
nm = neldermead_search(nm, "off");
funevals = neldermead_get(nm,"-funevals");
// Let's be not strict
assert_checkequal ( funevals < 15 , %T );
// Cleanup
nm = neldermead_destroy(nm);

//
// Check maxiter with "variable" method
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",0.1);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_search(nm, "off");
iterations = neldermead_get(nm,"-iterations");
assert_checkequal ( iterations , 10 );
// Cleanup
nm = neldermead_destroy(nm);

// Wrong -method flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-method'',''foo'')";
alloptions = """fixed"" or ""variable"" or ""box"" or ""mine""";
assert_checkerror(cmd,"%s: Expected value [%s] for input argument %s at input #%d, but got ""%s"" instead.",[],..
"neldermead_configure",alloptions,"value",3,"foo");
nm = neldermead_destroy(nm);

// Wrong -simplex0method flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-simplex0method'',''foo'')";
alloptions = """given"" or ""axes"" or ""spendley"" or ""pfeffer"" or ""randbounds""";
assert_checkerror(cmd,"%s: Expected value [%s] for input argument %s at input #%d, but got ""%s"" instead.",[],..
"neldermead_configure",alloptions,"value",3,"foo");
nm = neldermead_destroy(nm);

// Wrong -tolsimplexizemethod flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-tolsimplexizemethod'',''foo'')";
assert_checkerror(cmd,"%s: Expected boolean but for variable %s at input #%d, got %s instead.",[],"nelmead_typeboolean","value",3,"string");
nm = neldermead_destroy(nm);

// Wrong -tolssizedeltafvmethod flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-tolssizedeltafvmethod'',''foo'')";
assert_checkerror(cmd,"%s: Expected boolean but for variable %s at input #%d, got %s instead.",[],"nelmead_typeboolean","value",3,"string");
nm = neldermead_destroy(nm);

//
// Check wrong key for get method
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",0.1);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-maxfunevals",2);
nm = neldermead_search(nm, "off");
cmd = "funevals = neldermead_get(nm,''-foo'')";
assert_checkerror(cmd,"%s: Unknown key %s",[],"optimbase_get","-foo");
nm = neldermead_destroy(nm);

//
// Check that x0 is forced to be a vector
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
cmd = "nm = neldermead_configure(nm,''-x0'',[-1.2 1.0]);";
nm = neldermead_destroy(nm);

nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
cmd = "nm = neldermead_configure(nm,''-x0'',[-1.2 1.0; 1.0 -1.2]);";
assert_checkerror(cmd,"%s: Wrong size for x0 argument: A vector expected.", [], "optimbase_configure");
nm = neldermead_destroy(nm);

//
// Check -restartstep
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
cmd = "nm = neldermead_configure(nm,''-restartstep'',[1 2 3]);";
assert_checkerror(cmd,"%s: The restartstep vector is expected to have %d x %d shape, but current shape is %d x %d",[],"neldermead_configure",2,1,1,3);
cmd = "nm = neldermead_configure(nm,''-restartstep'',[-1 2]'');";
assert_checkerror(cmd,"%s: Expected that all entries of input argument %s at input #%d are greater or equal than %s, but entry #%d is equal to %s.",[],..
"neldermead_configure","value",3,string(number_properties("tiny")),1,"-1");
nm = neldermead_destroy(nm);

//
// Check -restarteps
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
cmd = "nm = neldermead_configure(nm,''-restarteps'',[1 2]);";
assert_checkerror(cmd,"%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n",[], ..
"neldermead_configure",3,1,1);
cmd = "nm = neldermead_configure(nm,''-restarteps'',-1);";
assert_checkerror(cmd,"%s: Expected that all entries of input argument %s at input #%d are greater or equal than %s, but entry #%d is equal to %s.",[],..
"neldermead_configure","value",3,string(number_properties("tiny")),1,"-1");
nm = neldermead_destroy(nm);

//
// Check the display system
nm = neldermead_new ();
nm
nm = neldermead_destroy(nm);

