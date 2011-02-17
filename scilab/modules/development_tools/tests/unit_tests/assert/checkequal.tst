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
  if ( and ( computed==expected ) ) then
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
instr = "assert_checkequal ( )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkequal ( 1 )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "[o1,o2,o3]=assert_checkequal ( 1 , 1 )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 59 );

//////////////////////////////////////////
// Check error message when type of arguments is false
instr = "assert_checkequal ( ""a"" , 2 )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
instr = "assert_checkequal ( 1 , ""b"" )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//////////////////////////////////////////
// Check error message when size of arguments are not equal
instr = "assert_checkequal ( 1 , [2 3] )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
//
// Check that the error message is correctly handled.
instr = "assert_checkequal ( [1 2], [3 4] )";
ierr=execstr(instr,"errcatch");
MY_assert_equal ( ierr , 10000 );
errmsg = lasterror();
MY_assert_equal ( errmsg , "assert_checkequal: Assertion failed: expected = [3 ...] while computed = [1 ...]" );
//
[flag , errmsg] = assert_checkequal ( %T , %T );
checkassert ( flag , errmsg , "success" );
//
[flag , errmsg] = assert_checkequal ( %F , %T );
checkassert ( flag , errmsg , "failure" );
//
[flag , errmsg] = assert_checkequal ( %nan , %nan );
checkassert ( flag , errmsg , "success" );
//
[flag , errmsg] = assert_checkequal ( list() , list() );
checkassert ( flag , errmsg , "success" );
//
[flag , errmsg] = assert_checkequal ( [%T %F], [%T %F] );
checkassert ( flag , errmsg , "success" );
//
// Test all IEEE values
[flag , errmsg] = assert_checkequal ( [1 %inf -%inf %nan] , [1 %inf -%inf %nan] );
checkassert ( flag , errmsg , "success" );
//
[flag , errmsg] = assert_checkequal ( [] , [] );
checkassert ( flag , errmsg , "success" );
////////////////////////////////////////////////////////
// Check complex entries.
[flag , errmsg] = assert_checkequal(complex(%nan,%nan),complex(%nan,0));
checkassert ( flag , errmsg , "failure" );
//
[flag , errmsg] = assert_checkequal(complex(%nan,%inf),complex(%nan,0));
checkassert ( flag , errmsg , "failure" );
//
[flag , errmsg] = assert_checkequal(complex(%nan,%nan),complex(0,%nan));
checkassert ( flag , errmsg , "failure" );
//
[flag , errmsg] = assert_checkequal(complex(%nan,%inf),complex(0,%inf));
checkassert ( flag , errmsg , "failure" );
//
[flag , errmsg] = assert_checkequal(complex(0,%inf),complex(0,%inf));
checkassert ( flag , errmsg , "success" );
//
[flag , errmsg] = assert_checkequal(complex(%nan,%nan),complex(%nan,%nan));
checkassert ( flag , errmsg , "success" );
//
[flag , errmsg] = assert_checkequal(complex(%inf,%nan),complex(%inf,%nan));
checkassert ( flag , errmsg , "success" );





