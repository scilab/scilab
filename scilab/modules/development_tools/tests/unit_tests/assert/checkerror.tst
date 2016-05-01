// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - 2011 - DIGITEO - Michael Baudin
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

function flag = MY_assert_equal ( computed , expected )
    if computed==expected then
        flag = 1;
    else
        flag = 0;
    end
    if flag <> 1 then pause,end
endfunction

// These tests are rather abstract.
// This is because we test the assert_checkerror function,
// which generates errors.
// To check that assert_checkerror performs well, we use
// execstr.
// We obviously do not want to use assert_checkerror, since a
// bug in assert_checkerror would make the current test fail in a
// weird way.
// Moreover, the current tests are localized, so that
// a Scilab in English or in French equally pass the test.

function y = f(x)
    [lhs,rhs]=argn()
    if ( rhs <> 1 ) then
        lstr=gettext("%s: Wrong number of input argument: %d expected.\n")
        errmsg = sprintf ( lstr , "f" , 1 )
        error(errmsg)
    end
    if ( typeof(x) <> "constant" ) then
        lstr=gettext("%s: Wrong type for input argument #%d: Matrix expected.\n")
        errmsg = sprintf ( lstr , "f" , 1 )
        error(errmsg,123456789)
    end
    y = x
endfunction

///////////////////////////////////////////////
// 1. Check our test-function f (the old shool way),
// i.e. check that the function function is correctly written.
// The test of the assert_checkerror function starts at step 2.
MY_assert_equal ( f(2) , 2 );
//
instr = "f()";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
ferrmsg = msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"f",1);
MY_assert_equal ( lerr , ferrmsg );
//
instr = "f(""aa"")";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 123456789 );
lerr = lasterror();
ferrmsg = msprintf(gettext("%s: Wrong type for input argument #%d: Matrix expected.\n"),"f",1);
MY_assert_equal ( lerr , ferrmsg );
//
///////////////////////////////////////////////
// 2. Check the error messages produced by assert_checkerror
// in case of wrong use of assert_checkerror
//
// Check error message when number of input arguments is false
instr = "assert_checkerror ( )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
asserterrmsg = sprintf ( gettext ( "%s: Wrong number of input argument: At least %d expected.\n") , "assert_checkerror" , 2 );
MY_assert_equal ( lerr , asserterrmsg );
//
// Check when number of output arguments is false
//
instr = "[o1,o2,o3]=assert_checkerror ( ""y=f(1)"" , """" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 999 );
//
// Check error message when type of 1st argument of assert_checkerror is false
instr = "assert_checkerror ( 1 , """" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix of strings expected.\n") , "assert_checkerror" , 1 ) );
//
// Check error message when type of 2nd argument of assert_checkerror is false
instr = "assert_checkerror ( """" , 1 )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix of strings expected.\n") , "assert_checkerror" , 2 ) );
//
// Check error message when type of 3d argument of assert_checkerror is false
instr = "assert_checkerror ( """" , """" , """" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , sprintf ( gettext ( "%s: Wrong type for input argument #%d: Matrix expected.\n") , "assert_checkerror" , 3 ) );
//
// Check error message when size of 1st argument of assert_checkerror is false
instr = "assert_checkerror ( ["""" """"], """" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_checkerror" , 1 , 1, 1) );
//
// Check error message when size of 2nd argument of assert_checkerror is true
//
instr = "assert_checkerror ( """" , ["""" """"] )";
ierr = execstr(instr, "errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , sprintf( gettext ( "%s: No error was produced while evaluating ""%s"".") , "assert_checkerror" , "") );
//
// Check error message when size of 3d argument of assert_checkerror is false
instr = "assert_checkerror ( """" , """", [1 2] )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , sprintf ( gettext ( "%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n") , "assert_checkerror" , 3 , 1, 1) );
//
// Check error message when value of 3d argument of assert_checkerror is false
instr = "assert_checkerror ( """" , """", -12 )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
MY_assert_equal ( lerr , sprintf ( gettext ( "%s: Wrong value for input argument #%d: Non-negative integers expected.\n"  ) , "assert_checkerror" , 3 ) );
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
MY_assert_equal ( lerr , sprintf( gettext ( "%s: No error was produced while evaluating ""%s"".") , "assert_checkerror" , "y=f(1)" ) );
//
// Check error message when formatting the error message generates an error (!).
instr = "assert_checkerror ( ""y=f()"" , ""foo%d%d"" , [] , 2 )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
msprerrmsg = sprintf(gettext("%s: Wrong number of input arguments: data doesn''t fit with format.\n"),"msprintf",0);
MY_assert_equal ( lerr , sprintf( gettext ( "%s: Error while formatting the error message: ""%s""") , "assert_checkerror" , msprerrmsg ) );
//
///////////////////////////////////////////////
//
// Typical use-cases : the test pass
//
msg1=msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"f",1);
msg2=msprintf(gettext("%s: Wrong type for input argument #%d: Matrix expected.\n"),"f",1);
assert_checkerror ( "y=f()"      , msg1 );
assert_checkerror ( "y=f(""a"")" , msg2 );
assert_checkerror ( "y=f()"      , msg1                  , 10000 );
assert_checkerror ( "y=f(""a"")" , msg2 , 123456789 );
// Check error message when the good error is produced by f (and errmsg is not given)
flag = assert_checkerror ( "y=f()" , msg1 );
MY_assert_equal ( flag , %t );
//
flag = assert_checkerror ( "y=f(""a"")" , msg2 );
MY_assert_equal ( flag , %t );
//
// Check error message and error number
flag = assert_checkerror ( "y=f()" , msg1 , 10000 );
MY_assert_equal ( flag , %t );
//
flag = assert_checkerror ( "y=f(""a"")" , msg2 , 123456789 );
MY_assert_equal ( flag , %t );
//
// Check error message when the good error is produced by f (and errmsg is an output argument)
[flag,errmsg] = assert_checkerror ( "y=f()" , msg1 );
MY_assert_equal ( flag , %t );
MY_assert_equal ( errmsg , "" );
//
// Check error message and error number (and errmsg is given is an output argument)
[flag,errmsg] = assert_checkerror ( "y=f()" , msg1 , 10000 );
MY_assert_equal ( flag , %t );
MY_assert_equal ( errmsg , "" );
//
// Let assert_checkerror localize the message
lcl1="%s: Wrong number of input argument: %d expected.\n";
lcl2="%s: Wrong type for input argument #%d: Matrix expected.\n";
assert_checkerror ( "y=f()"      , lcl1 , [] , "f" , 1);
assert_checkerror ( "y=f(""a"")" , lcl2 , [] , "f" , 1);
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
localstr = gettext("%s: Assertion failed: expected error message = ""%s"" while computed error message = ""%s"".");
asserterrmsg = msprintf(localstr,"assert_checkerror","oups",msg1);
MY_assert_equal ( lerr , asserterrmsg );
//
instr = "assert_checkerror ( ""y=f(""""a"""")"" , ""oups"" )";
ierr = execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
localstr = gettext("%s: Assertion failed: expected error message = ""%s"" while computed error message = ""%s"".");
asserterrmsg = msprintf(localstr,"assert_checkerror","oups",msg2);
MY_assert_equal ( lerr , asserterrmsg );
//
// Check when errmsg output argument is given
//
[flag,errmsg]=assert_checkerror ( "y=f()" , "oups" );
MY_assert_equal ( flag , %f );
localstr = gettext("%s: Assertion failed: expected error message = ""%s"" while computed error message = ""%s"".");
asserterrmsg = msprintf(localstr,"assert_checkerror","oups",msg1);
MY_assert_equal ( errmsg , asserterrmsg );
//
[flag,errmsg]=assert_checkerror ( "y=f(""a"")" , "oups" );
MY_assert_equal ( flag , %f );
localstr = gettext("%s: Assertion failed: expected error message = ""%s"" while computed error message = ""%s"".");
asserterrmsg = msprintf(localstr,"assert_checkerror","oups",msg2);
MY_assert_equal ( errmsg , asserterrmsg );

///////////////////////////////////////////////
//
// Typical use-cases : the test fails because the error number is no the same
//
instr = "assert_checkerror ( ""y=f()"" , "+sci2exp(msg1)+" , 12 )";
ierr = execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
lerr = lasterror();
refmsg = msprintf( gettext( "%s: Assertion failed: expected error number = %d while computed error number = %d." ) , "assert_checkerror" , 12 , 10000 );
MY_assert_equal ( lerr , refmsg );
//
[flag,errmsg]=assert_checkerror ( "y=f()" , msg1 , 12 );
MY_assert_equal ( flag , %f );
MY_assert_equal ( errmsg , refmsg );

