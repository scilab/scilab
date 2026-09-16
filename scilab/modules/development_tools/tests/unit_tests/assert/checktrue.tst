// Copyright (C) 2008 - INRIA - Michael Baudin
// Copyright (C) 2010 - DIGITEO - Michael Baudin
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->
// <-- NO ASSERT FAILURE -->

function flag = MY_assert_equal (computed , expected)
  if computed==expected then
    flag = 1;
  else
    flag = 0;
  end
  if flag <> 1 then pause,end
endfunction

function checkassert (flag , errmsg , ctype)
  if (ctype == "success") then
    MY_assert_equal ((flag==%t) & (errmsg==""), %t)
  else
    MY_assert_equal ((flag==%f) & (errmsg<>""), %t)
  end
endfunction

format("v",10);

// Check error message when number of arguments is false
instr = "assert_checktrue ()";
ierr=execstr(instr,"errcatch");
MY_assert_equal (ierr , 10000);
//
instr = "[o1,o2,o3]=assert_checktrue (%t)";
ierr=execstr(instr,"errcatch");
MY_assert_equal (ierr , 999);
//////////////////////////////////////////
// Check error message when type of arguments is false
instr = "assert_checktrue (""a"")";
ierr=execstr(instr,"errcatch");
MY_assert_equal (ierr , 10000);
//
//
// Check that the error message is correctly handled.
instr = "assert_checktrue ([%f %t])";
ierr=execstr(instr,"errcatch");
MY_assert_equal (ierr , 10000);
errmsg = lasterror();
msg = gettext("%s: Assertion failed: Entry %%F found in condition(%d).\n");
refmsg = msprintf(msg, "assert_checktrue", 1);
MY_assert_equal (errmsg , refmsg);
//
for o = list(%t, [%t %t], sparse(%t), sparse([%t %t]))
    [flag,errmsg] = assert_checktrue(o);
    checkassert (flag , errmsg , "success");
end
//
for o = list(%f, [%f %f], [%t %f], sparse(%f), sparse([%f %f]), sparse([%t %f]))
    [flag,errmsg] = assert_checktrue(o);
    checkassert (flag , errmsg , "failure");
end

//////////////////////////////////////////
// Check custom message parameter
//////////////////////////////////////////

// Test with custom message on failure
[flag, errmsg] = assert_checktrue([%f %t], "my custom message");
MY_assert_equal(flag, %f);
// Check that custom message is in the error
MY_assert_equal(~isempty(grep(errmsg, "my custom message")), %t);

// Test backward compatibility (no message)
[flag, errmsg] = assert_checktrue([%f %t]);
MY_assert_equal(flag, %f);
MY_assert_equal(~isempty(grep(errmsg, "Assertion failed")), %t);

// Test with empty message
[flag, errmsg] = assert_checktrue([%f %t], "");
MY_assert_equal(flag, %f);
MY_assert_equal(~isempty(grep(errmsg, "Assertion failed")), %t);

// Test success with message (should not error)
[flag, errmsg] = assert_checktrue(%t, "success message");
MY_assert_equal(flag, %t);
MY_assert_equal(errmsg, "");

// Test wrong type for message
[flag, errmsg] = assert_checktrue(%t, 123);
MY_assert_equal(flag, %f);
MY_assert_equal(~isempty(grep(errmsg, "Wrong type")), %t);

// Test wrong size for message (row vector)
[flag, errmsg] = assert_checktrue(%t, ["a" "b"]);
MY_assert_equal(flag, %f);
MY_assert_equal(~isempty(grep(errmsg, "Wrong size")), %t);

