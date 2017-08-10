// Copyright (C) 2009 - 2010 - DIGITEO - Michael Baudin
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

