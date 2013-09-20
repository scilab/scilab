// Copyright (C) 2009 - 2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt


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

instr = "assert_generror ( ""oups"" )";
ierr=execstr(instr,"errcatch");
lerr = lasterror();
MY_assert_equal ( ierr , 10000 );
MY_assert_equal ( lerr , "oups" );
//
instr = "assert_generror ( ""oups"" , 123456789 )";
ierr=execstr(instr,"errcatch");
lerr = lasterror();
MY_assert_equal ( ierr , 123456789 );
MY_assert_equal ( lerr , "oups" );
//
instr = "assert_generror ( 12 )";
ierr=execstr(instr,"errcatch");
lerr = lasterror();
MY_assert_equal ( ierr , 10000 );
refmsg = msprintf( gettext( "%s: Wrong type for input argument #%d: Matrix of strings expected.\n" ) , "assert_generror" , 1 );
MY_assert_equal ( lerr , refmsg );
//
instr = "assert_generror ( )";
ierr=execstr(instr,"errcatch");
lerr = lasterror();
MY_assert_equal ( ierr , 10000 );
refmsg = msprintf( gettext( "%s: Wrong number of input arguments: %d to %d expected." ) , "assert_generror" , 1 , 2 );
MY_assert_equal ( lerr , refmsg );

