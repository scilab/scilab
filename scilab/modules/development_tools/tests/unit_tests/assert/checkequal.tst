// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// --------------------------------------------------------------------------
// <-- NO CHECK REF -->
// <-- ENGLISH IMPOSED -->

// ====================================
// Unitary tests of assert_checkequal()
// ====================================

function flag = MY_assert_equal ( computed , expected )
    if ( and ( computed==expected ) ) then
        flag = 1;
    else
        flag = 0;
    end
    if flag <> 1 then whereami(), pause,end
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
MY_assert_equal ( ierr , 999 );

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
refmsg = _("%s: Assertion failed: %s  while %s (mean diff = %s)");
refmsg = msprintf(refmsg, "assert_checkequal", ..
                          msprintf(_("expected(%d)= "),1)+"3", ..
                          msprintf(_("computed(%d)= "),1)+"1", "-2");
MY_assert_equal ( errmsg , refmsg );
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
//
////////////////////////////////////////////////////////
// Check various types
////////////////////////////////////////////////////////
//
// Lists
// -----
assert_checkequal(list(), list());
assert_checkequal(list([]), list([]));
assert_checkequal(list(list()), list(list()));
assert_checkequal(list(%nan), list(%nan));
assert_checkequal(list(1,,3), list(1,,3));
assert_checkequal(list(1,,%nan+%i), list(1,,%nan+%i));
s = list(1,,list(2,,4));
assert_checkequal(s, s);
s = list("foo",2);
assert_checkequal(s, s);
ierr = execstr("assert_checkequal(list(2,,7), list(2,%F,8))","errcatch");
MY_assert_equal(ierr, 10000);
errmsg = lasterror();
refmsg = _("%s: Assertion failed: %s  while %s");
refmsg = msprintf(refmsg, "assert_checkequal", ..
    msprintf(_("expected(%d)= "),2) + "F", ..
    msprintf(_("computed(%d)= "),2) + "(void)");
MY_assert_equal( errmsg , refmsg );

// Mlist
// -----
s = mlist(["V","name","value"],["a","b";"c" "d"],[1 2; 3 4]);
assert_checkequal(s, s);
//
// Tlist
// -----
s = tlist(["V","name","value"],["a","b";"c" "d"],[1 2; 3 4]);
assert_checkequal(s, s);
//
// Polynomial
// ----------
s = poly(0,"s");
assert_checkequal(s, s);
//
// Sparse
// ------
s = spzeros(3,5);
assert_checkequal(s, s);
assert_checkequal(sparse([]), sparse([]));
assert_checkequal(sparse(%nan), sparse(%nan));
assert_checkequal(sparse([0 %nan 2]), sparse([0 %nan 2]));
t = s;
s(1)=12;
instr="assert_checkequal(s, t)";
ierr=execstr(instr,"errcatch");
MY_assert_equal(ierr, 10000);

//
// Boolean
// -------
s=(ones(3,5)==ones(3,5));
t=s;
assert_checkequal(s, t);
//
// Sparse Boolean
// --------------
s=(spzeros(3,5)==spzeros(3,5));
t=s;
assert_checkequal(s, t);
s(1)=%f;
instr="assert_checkequal(s, t)";
ierr=execstr(instr,"errcatch");
MY_assert_equal(ierr, 10000);

//
// Integer 8
// ---------
s=int8(3);
t=s;
assert_checkequal(s, t);
//
// String
// ------
assert_checkequal("", "");
s = ["foo" "FOO"];
assert_checkequal(s,s);
//
// Hypermatrix
// -----------
// - Double
s = rand(2,2,2);
t = s;
assert_checkequal(s,t);
// - int8
s = int8(rand(2,2,2));
t = s;
assert_checkequal(s,t);
// - wrong type
s = rand(2,2,2);
t = int8(s);
instr="assert_checkequal(s, t)";
ierr=execstr(instr,"errcatch");
MY_assert_equal(ierr, 10000);

// Implicit lists
// --------------
assert_checkequal(1:$, 1:$);
assert_checkequal(3:2:$-1, 3:2:$-1);
ierr = execstr("assert_checkequal(1:$,2:$)","errcatch");
MY_assert_equal(ierr, 10000);
errmsg = lasterror();
refmsg = _("%s: Assertion failed: expected= %s  while computed= %s");
refmsg = msprintf(refmsg, "assert_checkequal", "2:1:$", "1:1:$");
MY_assert_equal ( errmsg , refmsg );

ierr = execstr("assert_checkequal(1:$,1:$-1)","errcatch");
MY_assert_equal(ierr, 10000);

// Primitives
// ----------
assert_checkequal(sin, sin);
sine = sin;
assert_checkequal(sin, sine);
ierr = execstr("assert_checkequal(sin, cos)","errcatch");
MY_assert_equal(ierr, 10000);
errmsg = lasterror();
refmsg = _("%s: Assertion failed: expected and computed are two distinct built-in functions.");
refmsg = msprintf(refmsg, "assert_checkequal");
MY_assert_equal ( errmsg , refmsg );

ierr = execstr("assert_checkequal(sin, sind)","errcatch");
MY_assert_equal(ierr, 10000);

// Scilab functions
// ----------------
assert_checkequal(sind, sind);
sined = sind;
assert_checkequal(sind, sined);

ierr = execstr("assert_checkequal(sind, cosd)","errcatch");
MY_assert_equal(ierr, 10000);
errmsg = lasterror();
refmsg = _("%s: Assertion failed: expected= %s  while computed= %s");
refmsg = msprintf(refmsg, "assert_checkequal", "cosd()", "sind()");
MY_assert_equal ( errmsg , refmsg );

ierr = execstr("assert_checkequal(sind, sin)","errcatch");
MY_assert_equal(ierr, 10000);

// Scilab libraries
// ----------------
assert_checkequal(iolib, iolib);
ierr = execstr("assert_checkequal(corelib, iolib)","errcatch");
MY_assert_equal(ierr, 10000);
errmsg = lasterror();
refmsg = _("%s: Assertion failed: expected= %s  while computed= %s");
refmsg = msprintf(refmsg, "assert_checkequal", ..
                   "lib@" + string(iolib)(1), "lib@" + string(corelib)(1));
MY_assert_equal( errmsg , refmsg );

// Graphical handles
// -----------------
assert_checkequal(gdf(), gdf());
assert_checkequal(gda(), gda());
assert_checkequal([gdf(),gda()], [gdf(),gda()]);
ierr = execstr("assert_checkequal(gdf(), gda())","errcatch");
MY_assert_equal(ierr, 10000);
errmsg = lasterror();
refmsg = _("%s: Assertion failed: expected= %s  while computed= %s");
refmsg = msprintf(refmsg, "assert_checkequal", ..
                   msprintf("Axes(uid:%d)",gda().uid), ..
                   msprintf("Figure(uid:%d)",gdf().uid));
MY_assert_equal( errmsg , refmsg );
assert_checkequal([gdf(),gda()], [gdf(),gda()]);

ierr = execstr("assert_checkequal([gdf(),gda()],[gdf(),gdf()])","errcatch");
MY_assert_equal(ierr, 10000);
errmsg = lasterror();
refmsg = _("%s: Assertion failed: %s  while %s");
refmsg = msprintf(refmsg, "assert_checkequal", ..
    msprintf(_("expected(%d)= "),2) + msprintf("Figure(uid:%d)",gdf().uid), ..
    msprintf(_("computed(%d)= "),2) + msprintf("Axes(uid:%d)",gda().uid));
MY_assert_equal( errmsg , refmsg );

