// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


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
function y = rosenbrock (x)
  y = 100*(x(2)-x(1)^2)^2 + (1-x(1))^2;
endfunction

//
// Test basic verbose
//
opt = optimbase_new ();
computed = optimbase_cget(opt,"-verbose");
assert_equal ( computed , 0 );
opt = optimbase_configure(opt,"-verbose",1);
computed = optimbase_cget(opt,"-verbose");
assert_equal ( computed , 1 );
optimbase_log ( opt , "My interesting message" );
opt = optimbase_configure(opt,"-verbose",0);
// Log a message relative to the stopping rule
computed = optimbase_cget(opt,"-verbosetermination");
assert_equal ( computed , 0 );
opt = optimbase_configure(opt,"-verbosetermination",1);
computed = optimbase_cget(opt,"-verbosetermination");
assert_equal ( computed , 1 );
opt = optimbase_stoplog ( opt , "My interesting stop message" );
opt = optimbase_configure(opt,"-verbosetermination",0);
opt = optimbase_destroy(opt);

//
// Test logging into a file
//
opt = optimbase_new ();
opt = optimbase_configure(opt,"-verbose",1);
computed = optimbase_cget(opt,"-logfile");
assert_equal ( computed , "" );
opt = optimbase_configure(opt,"-logfile","mylogfile.txt");
computed = optimbase_cget(opt,"-logfile");
assert_equal ( computed , "mylogfile.txt" );
opt = optimbase_log ( opt , "My interesting message" );
opt = optimbase_configure(opt,"-verbose",0);
opt = optimbase_log ( opt , "My NOT interesting message" );
opt = optimbase_destroy(opt);
computed = fileinfo ( "mylogfile.txt" );
assert_equal ( ( computed <> [] ) , %t );
// Check content
fd = mopen( "mylogfile.txt" , "r" );
computed = mgetl ( fd , 1 ); // The date is not checked
computed = mgetl ( fd , 1 );
assert_equal ( computed , "My interesting message" );
computed = meof ( fd );
assert_equal ( computed , 0 );
mclose ( fd );
computed = deletefile ( "mylogfile.txt" );
assert_equal ( computed , %t );


