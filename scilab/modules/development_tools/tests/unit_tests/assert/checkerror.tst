// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// <-- JVM NOT MANDATORY -->
// <-- ENGLISH IMPOSED -->

function flag = MY_assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

// Which error number to generate ?
// 201 "%s: Wrong type for argument %d: Real or complex matrix expected."
// 209 "%s: Wrong type for argument %d: Matrix expected."
// 53 "Wrong type for input argument %d: Real or complex matrix expected."
// 81 "%s: Wrong type for argument %d: Real or complex matrix expected."

function y = f(x)
    [lhs,rhs]=argn()
    if ( rhs <> 1 ) then
        errmsg = sprintf ( gettext ( "%s: Unexpected number of input arguments : %d provided while %d to %d are expected.") , "f" , rhs , 1 , 1 )
        error(errmsg)
    end
    if ( typeof(x) <> "constant" ) then
        localstr = gettext ( "%s: Unexpected type of input argument #%d : variable %s has type %s while %s is expected.")
        errmsg = sprintf ( localstr , "f" , 1 , "x" , typeof(x) , "constant" )
        error(errmsg,123456789)
    end
    y = x
endfunction

///////////////////////////////////////////////
// Check our test-function f (the old shool way)
MY_assert_equal ( f(2) , 2 );
//
instr = "f()";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , "f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected." );
//
instr = "f(""aa"")";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 123456789 );
lerr = lasterror();
MY_assert_equal ( lerr , "f: Unexpected type of input argument #1 : variable x has type string while constant is expected." );
//
///////////////////////////////////////////////
// Check the error messages produced by assert_checkerror in case of wrong use of assert_checkerror
//
// Check error message when number of input arguments is false
instr = "assert_checkerror ( )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , "assert_checkerror: Wrong number of input arguments: 2 to 3 expected." );
//
// Check when number of output arguments is false
//
instr = "[o1,o2,o3]=assert_checkerror ( ""y=f(1)"" , """" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 59 );
//
// Check error message when type of arguments of assert_checkerror is false
instr = "assert_checkerror ( 1 , """" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , "assert_checkerror: Wrong type for argument 1: Matrix of strings expected." );
//
// Check error message when no error is produced by f
instr = "assert_checkerror ( ""y=f(1)"" , ""foo"" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , "assert_checkerror: No error was produced while evaluating ""y=f(1)""." );
//
// Check error message when no error is produced by f (and error number is given)
instr = "assert_checkerror ( ""y=f(1)"" , ""foo"" , 12 )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , "assert_checkerror: No error was produced while evaluating ""y=f(1)""." );
//
///////////////////////////////////////////////
//
// Typical use-cases : the test pass
//
assert_checkerror ( "y=f()"      , "f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected." );
assert_checkerror ( "y=f(""a"")" , "f: Unexpected type of input argument #1 : variable x has type string while constant is expected." );
assert_checkerror ( "y=f()"      , "f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected."                  , 10000 );
assert_checkerror ( "y=f(""a"")" , "f: Unexpected type of input argument #1 : variable x has type string while constant is expected." , 123456789 );
// Check error message when the good error is produced by f (and errmsg is not given)
flag = assert_checkerror ( "y=f()" , "f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected." );
MY_assert_equal ( flag , %t );
// 
flag = assert_checkerror ( "y=f(""a"")" , "f: Unexpected type of input argument #1 : variable x has type string while constant is expected." );
MY_assert_equal ( flag , %t );
// 
// Check error message and error number
flag = assert_checkerror ( "y=f()" , "f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected." , 10000 );
MY_assert_equal ( flag , %t );
// 
flag = assert_checkerror ( "y=f(""a"")" , "f: Unexpected type of input argument #1 : variable x has type string while constant is expected." , 123456789 );
MY_assert_equal ( flag , %t );
//
// Check error message when the good error is produced by f (and errmsg is an output argument)
[flag,errmsg] = assert_checkerror ( "y=f()" , "f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected." );
MY_assert_equal ( flag , %t );
MY_assert_equal ( errmsg , "" );
// 
// Check error message and error number (and errmsg is given is an output argument)
[flag,errmsg] = assert_checkerror ( "y=f()" , "f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected." , 10000 );
MY_assert_equal ( flag , %t );
MY_assert_equal ( errmsg , "" );
//
///////////////////////////////////////////////
//
// Typical use-cases : the test fails because the message is no the same
//
// Check error message when the wrong error is produced by f (and errmsg is not given)
instr = "assert_checkerror ( ""y=f()"" , ""oups"" )";
ierr = execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , "assert_checkerror: Assertion failed: expected error message = ""oups"" while computed error message = ""f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected.""." );
//
instr = "assert_checkerror ( ""y=f(""""a"""")"" , ""oups"" )";
ierr = execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , "assert_checkerror: Assertion failed: expected error message = ""oups"" while computed error message = ""f: Unexpected type of input argument #1 : variable x has type string while constant is expected.""." );
//
// Check when errmsg is given
//
[flag,errmsg]=assert_checkerror ( "y=f()" , "oups" );
MY_assert_equal ( flag , %f );
MY_assert_equal ( errmsg , "assert_checkerror: Assertion failed: expected error message = ""oups"" while computed error message = ""f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected.""." );
//
[flag,errmsg]=assert_checkerror ( "y=f(""a"")" , "oups" );
MY_assert_equal ( flag , %f );
MY_assert_equal ( errmsg , "assert_checkerror: Assertion failed: expected error message = ""oups"" while computed error message = ""f: Unexpected type of input argument #1 : variable x has type string while constant is expected.""." );
///////////////////////////////////////////////
//
// Typical use-cases : the test fails because the error number is no the same
//
instr = "assert_checkerror ( ""y=f()"" , ""f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected."" , 12 )";
ierr = execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , "assert_checkerror: Assertion failed: expected error number = 12 while computed error number = 10000." );
//
[flag,errmsg]=assert_checkerror ( "y=f()" , "f: Unexpected number of input arguments : 0 provided while 1 to 1 are expected." , 12 );
MY_assert_equal ( flag , %f );
MY_assert_equal ( errmsg , "assert_checkerror: Assertion failed: expected error number = 12 while computed error number = 10000." );

