// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010-2011 - DIGITEO - Michael Baudin
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

function flag = MY_assert_equal ( computed , expected )
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

function checkassert ( flag , errmsg , ctype )
  if ( ctype == "success" ) then
    MY_assert_equal ( (flag==%t) & (errmsg==""), %t )
  else
    MY_assert_equal ( (flag==%f) & (errmsg<>""), %t )
  end
endfunction
    

format("v",10);

// Check error message when number of arguments is false
instr = "assert_checkalmostequal ( )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkalmostequal ( 1 )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "[o1,o2,o3]=assert_checkalmostequal ( 1 , 1 , %eps )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 999 );
//
//////////////////////////////////////////
// Check error message when type of arguments is false
instr = "assert_checkalmostequal ( ""a"" , 2 , %eps )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkalmostequal ( 1 , ""b"" , %eps )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkalmostequal ( 1 , 2 , ""c"" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkalmostequal ( 1 , 2 , %eps , ""c"" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkalmostequal ( 1 , 2 , %eps , 0 , %t )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//////////////////////////////////////////
// Check error message when size of arguments is false
instr = "assert_checkalmostequal ( 1 , 2 , [1 1] )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkalmostequal ( 1 , 2 , %eps , [1 1] )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkalmostequal ( 1 , 2 , %eps , 0 , [""a"" ""b""] )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkalmostequal ( 1 , [2 3] , %eps )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );

//////////////////////////////////////////
//
// Check that the error message is correctly handled.
instr = "assert_checkalmostequal ( 1 , 2 )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
errmsg = lasterror();
refmsg = msprintf(_("%s: Assertion failed: expected = %s while computed = %s"), "assert_checkalmostequal", "2", "1");
MY_assert_equal ( errmsg , refmsg );
//
// Check that the error message is correctly handled.
instr = "assert_checkalmostequal ( 1 , 2 , %eps )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
errmsg = lasterror();
refmsg = msprintf(_("%s: Assertion failed: expected = %s while computed = %s"), "assert_checkalmostequal", "2", "1");
MY_assert_equal ( errmsg , refmsg );
//
// Check that the error message is correctly handled.
instr = "assert_checkalmostequal ( 1 , 1 , %eps )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 0 );
errmsg = lasterror();
MY_assert_equal ( errmsg , [] );
//
// Obvious failure : Check the error message
instr = "assert_checkalmostequal ( zeros(10,1)+1.e-4 , zeros(10,1) , 1.e-5 )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
errmsg = lasterror();
refmsg = msprintf(_("%s: Assertion failed: expected = %s while computed = %s"), "assert_checkalmostequal", "[0 ...]", "[0.0001 ...]");
MY_assert_equal ( errmsg , refmsg );

//////////////////////////////////////////
//
// Test empty matrix
//
// Obvious success
[flag,errmsg] = assert_checkalmostequal ( [] , [] );
checkassert ( flag , errmsg , "success" );
//
// Obvious success
[flag,errmsg] = assert_checkalmostequal ( [] , [] , [], [], "matrix");
checkassert ( flag , errmsg , "success" );
//
// Obvious success
[flag,errmsg] = assert_checkalmostequal ( [] , [] , [], [], "element");
checkassert ( flag , errmsg , "success" );

//////////////////////////////////////////
//
//
// Obvious success
[flag,errmsg] = assert_checkalmostequal ( 1 , 1 );
checkassert ( flag , errmsg , "success" );
// Obvious success
[flag,errmsg] = assert_checkalmostequal ( 1 , 1 , %eps );
checkassert ( flag , errmsg , "success" );
//
// Obvious success
[flag,errmsg] = assert_checkalmostequal ( ones(10,1) , ones(10,1) , %eps );
checkassert ( flag , errmsg , "success" );
//
// Obvious success
[flag,errmsg] = assert_checkalmostequal ( ones(10,1)+%eps , ones(10,1) , %eps );
checkassert ( flag , errmsg , "success" );
//
// Obvious success
[flag,errmsg] = assert_checkalmostequal ( zeros(10,1) , zeros(10,1) , 1.e-5 );
checkassert ( flag , errmsg , "success" );
//
// Obvious failure
[flag,errmsg] = assert_checkalmostequal ( 1 , 2 , %eps );
checkassert ( flag , errmsg , "failure" );
//
// Obvious failure
[flag,errmsg] = assert_checkalmostequal ( zeros(10,1)+1.e-4 , zeros(10,1) , 1.e-5 );
checkassert ( flag , errmsg , "failure" );
//
// Success: not obvious!
// The two values are equal, very small, but nonzero.
// The relative tolerance must be used here.
// If, instead, a bug in the assert function is so that the 
// absolute tolerance is used as 10^-16, then the output 
// of this comparison is wrong.
[flag,errmsg] = assert_checkalmostequal ( 1.23456789123456789e-30 , 1.23456789123456789e-30 , %eps );
checkassert ( flag , errmsg , "success" );
//
// Failure : not obvious!
// There is only one significant digit here and we require the 
// maximum precision.
// The test must fail because the relative tolerance must be used here.
// If, instead, there is a bug in the comparison and 
// the absolute tolerance is used and set to
// 10^-16, the output of this test is wrong.
[flag,errmsg] = assert_checkalmostequal ( 1.23456789123456789e-30 , 1.3e-30 , %eps );
checkassert ( flag , errmsg , "failure" );
//
// Success : not obvious!
// The expected result is zero, so that the absolute tolerance is used.
[flag,errmsg] = assert_checkalmostequal ( 1.23456789123456789e-30 , 0 , 0 , 1.e-10 );
checkassert ( flag , errmsg , "success" );
//
// Check that nans are correctly handled
[flag,errmsg] = assert_checkalmostequal ( [1 %nan], [1 %nan] , %eps );
checkassert ( flag , errmsg , "success" );
//
// Check that nans are correctly handled
[flag,errmsg] = assert_checkalmostequal ( [1 %nan 1 %nan] , [1 %nan 1 %nan] , %eps );
checkassert ( flag , errmsg , "success" );
//
[flag,errmsg] = assert_checkalmostequal ( [%nan 1], [1 %nan] , %eps );
checkassert ( flag , errmsg , "failure" );
//
// Check that slightly different values cannot pass basic test
// Simple sequence with default settings
[flag,errmsg] = assert_checkalmostequal ( 1 + 5 * %eps , 1 , %eps );
checkassert ( flag , errmsg , "failure" );
//
// Customize the relative precision so that a test can pass
// Simple sequence with default settings
[flag,errmsg] = assert_checkalmostequal ( 1 + 5 * %eps , 1 , 10*%eps );
checkassert ( flag , errmsg , "success" );
//
// Customize the absolute precision so that a test can pass
// Simple sequence with default settings
[flag,errmsg] = assert_checkalmostequal ( 0 + 5 * %eps , 0 , 0 , 10*%eps );
checkassert ( flag , errmsg , "success" );
//
// An example where the relative error is used,
// with a customized tolerance.
[flag,errmsg] = assert_checkalmostequal ( 1.23456 , 1.23457 , 1.e11*%eps );
checkassert ( flag , errmsg , "success" );
//
// Put all IEEE values
[flag,errmsg] = assert_checkalmostequal ( [1.2345 %inf -%inf %nan] , [1.2346 %inf -%inf %nan] , 1.e-4 );
checkassert ( flag , errmsg , "success" );

///////////////////////////////////////////////////////////////////////////////
// Test elementwise algo

// If "matrix" type is used, this test pass.
[flag,errmsg] = assert_checkalmostequal ( [1 1.e5] , [2 1.e5] , 1.e-3 , [], "element" );
checkassert ( flag , errmsg , "failure" );
//
// Force the test to pass
[flag,errmsg] = assert_checkalmostequal ( [1 1.e5] , [1 1.e5] , 1.e-3 , [], "element" );
checkassert ( flag , errmsg , "success" );
//
[flag,errmsg] = assert_checkalmostequal ( [1 %nan] , [2 %nan] , 1.e-3 , [], "element" );
checkassert ( flag , errmsg , "failure" );
//
[flag,errmsg] = assert_checkalmostequal ( [1 %inf] , [2 %inf] , 1.e-3 , [], "element" );
checkassert ( flag , errmsg , "failure" );
//
[flag,errmsg] = assert_checkalmostequal ( [1 %inf] , [1 %inf] , 1.e-3 , [], "element" );
checkassert ( flag , errmsg , "success" );
//
[flag,errmsg] = assert_checkalmostequal ( [1 %inf -%inf %nan] , [1 %inf -%inf %nan] , 1.e-3 , [], "element" );
checkassert ( flag , errmsg , "success" );
//
[flag,errmsg] = assert_checkalmostequal ( [1 %inf -%inf %nan] , [1 -%inf %inf %nan] , 1.e-3 , [], "element" );
checkassert ( flag , errmsg , "failure" );

///////////////////////////////////////////////////////////////////////////////
// Test complex elementwise algo
//
[flag,errmsg] = assert_checkalmostequal ( 1+%i , 1+(1+1.e-4)*%i , 1.e-3 , [], "element" );
checkassert ( flag , errmsg , "success" );
[flag,errmsg] = assert_checkalmostequal ( 1+%i , 1+(1+1.e-4)*%i , 1.e-5 , [], "element" );
checkassert ( flag , errmsg , "failure" );
//
// Use absolute tolerance
[flag,errmsg] = assert_checkalmostequal ( 1+%i , 0 , [], 1.e-3 , "element" );
checkassert ( flag , errmsg , "failure" );
//

