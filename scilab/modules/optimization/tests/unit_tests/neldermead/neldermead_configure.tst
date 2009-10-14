// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// Check behaviour with configured settings.
//

//
// assert_close --
//   Returns 1 if the two real matrices computed and expected are close,
//   i.e. if the relative distance between computed and expected is lesser than epsilon.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_close ( computed, expected, epsilon )
  if expected==0.0 then
    shift = norm(computed-expected);
  else
    shift = norm(computed-expected)/norm(expected);
  end
  if shift < epsilon then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
//
// assert_equal --
//   Returns 1 if the two real matrices computed and expected are equal.
// Arguments
//   computed, expected : the two matrices to compare
//   epsilon : a small number
//
function flag = assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction
function [ y , index ] = rosenbrock ( x , index )
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Check maxfunevals with "variable" method
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",0.1);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-maxfunevals",10);
nm = neldermead_search(nm);
funevals = neldermead_get(nm,"-funevals");
// Let's be not strict
assert_equal ( funevals < 15 , %T );
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
//nm = neldermead_configure(nm,"-verbose",0);
//nm = neldermead_configure(nm,"-verbosetermination",0);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-maxiter",10);
nm = neldermead_search(nm);
iterations = neldermead_get(nm,"-iterations");
assert_equal ( iterations , 10 );
// Cleanup
nm = neldermead_destroy(nm);

// Wrong -method flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-method'',''foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_configure: Unknown value foo for -method option";
assert_equal ( computed , expected );
nm = neldermead_destroy(nm);

// Wrong -simplex0method flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-simplex0method'',''foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_configure: Unknown value foo for -simplex0method option";
assert_equal ( computed , expected );
nm = neldermead_destroy(nm);

// Wrong -tolsimplexizemethod flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-tolsimplexizemethod'',''foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_configure: Unknown value foo for -tolsimplexizemethod option";
assert_equal ( computed , expected );
nm = neldermead_destroy(nm);

// Wrong -tolssizedeltafvmethod flag
nm = neldermead_new ();
cmd = "nm = neldermead_configure(nm,''-tolssizedeltafvmethod'',''foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "neldermead_configure: Unknown value foo for -tolssizedeltafvmethod option";
assert_equal ( computed , expected );
nm = neldermead_destroy(nm);

//
// Check wrong key for get method
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
nm = neldermead_configure(nm,"-x0",[1.1 1.1]');
nm = neldermead_configure(nm,"-simplex0method","axes");
nm = neldermead_configure(nm,"-simplex0length",0.1);
nm = neldermead_configure(nm,"-method","variable");
nm = neldermead_configure(nm,"-verbose",0);
nm = neldermead_configure(nm,"-function",rosenbrock);
nm = neldermead_configure(nm,"-maxfunevals",2);
nm = neldermead_search(nm);
cmd = "funevals = neldermead_get(nm,''-foo'')";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_get: Unknown key -foo";
assert_equal ( computed , expected );
nm = neldermead_destroy(nm);


//
// Check that x0 is forced to be a column vector
//
nm = neldermead_new ();
nm = neldermead_configure(nm,"-numberofvariables",2);
cmd = "nm = neldermead_configure(nm,''-x0'',[-1.2 1.0]);";
execstr(cmd,"errcatch");
computed = lasterror();
expected = "optimbase_configure: The x0 vector is expected to be a column matrix, but current shape is 1 x 2";
assert_equal ( computed , expected );
nm = neldermead_destroy(nm);


