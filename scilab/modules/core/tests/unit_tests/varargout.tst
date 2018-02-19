// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// Return 3 values
//
function varargout=__test_function__()
  varargout(1) = 42;
  varargout(2) = "value";
  varargout(3) = list(51);
endfunction

// Simple call
__test_function__();

a = __test_function__();
if a <> 42 then pause, end;
clear a;

[a,b] = __test_function__();
if a <> 42 then pause, end;
if b <> "value" then pause, end;
clear a;
clear b;

[a,b,c] = __test_function__();
if a <> 42 then pause, end;
if b <> "value" then pause, end;
if c <> list(51) then pause, end;
clear a;
clear b;
clear c;

// Check call with too much output values.
ierr = execstr("[a,b,c,d] = __test_function__();", "errcatch");
assert_checkfalse(ierr == 0);

//
// Return a list of 3 elements with second missing.
//
function varargout=__test_function2__()
  varargout(1) = 42;
  varargout(3) = "value";
endfunction

// Simple call
__test_function2__();

a = __test_function2__();
if a <> 42 then pause, end;
clear a;

// varargout(2) is undefined
ierr = execstr("[a,b] = __test_function2__();", "errcatch");
assert_checkfalse(ierr == 0);

// empty list
function varargout=__test_function3__()
  varargout = list();
endfunction

msg = msprintf(_("%s: Wrong number of output argument(s): %d expected.\n"), "__test_function3__", 0);
assert_checkerror("a = __test_function3__()", msg);

// wrong varargout type
function varargout=__test_function4__()
  varargout = 42;
endfunction

msg = msprintf(_("%s: Wrong type for %s: A list expected.\n"), "__test_function4__", "Varargout");
assert_checkerror("__test_function4__()", msg);