// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

deff("[x] = myplus(y,z)","x=y+z");
assert_checktrue(isdef("myplus"));
assert_checkequal(myplus(3,2), 5);
clear myplus;
assert_checkfalse(isdef("myplus"));

deff("[x] = mymacro(y,z)",["a=3*y+1"; "x=a*z+y"]);
assert_checktrue(isdef("mymacro"));
assert_checkequal(mymacro(5,2), 37);
clear mymacro;
assert_checkfalse(isdef("mymacro"));

deff("[a,b,c] = mymacro(varargin)",["a = varargin(1); b = varargin(2) * a; c = varargin(3) * b"]);
assert_checktrue(isdef("mymacro"));
[a,b,c] = mymacro(3,4,5);
assert_checkequal(a, 3);
assert_checkequal(b, 12);
assert_checkequal(c, 60);
clear mymacro;
assert_checkfalse(isdef("mymacro"));

deff("[varargout] = mymacro(varargin)",["for i = 1 : size(varargin)"; "    varargout(i) = varargin(i) * 2;";"end"]);
assert_checktrue(isdef("mymacro"));
[a,b,c] = mymacro(3,4,5);
assert_checkequal(a, 6);
assert_checkequal(b, 8);
assert_checkequal(c, 10);
clear mymacro;
assert_checkfalse(isdef("mymacro"));

deff("[varargout] = mymacro(varargin)", ["deff(""varargout = mymacro2(varargin)"", [""for i = 1 : size(varargin)""; ""    varargout(i) = varargin(i) * 2;"";""end""]);";"[a,b,c] = mymacro2(varargin(:));";"varargout(1) = a*2;";"varargout(2) = b*2;";"varargout(3) = c*2;"]);
assert_checktrue(isdef("mymacro"));
assert_checkfalse(isdef("mymacro2"));
[a,b,c] = mymacro(3,4,5);
assert_checkequal(a, 12);
assert_checkequal(b, 16);
assert_checkequal(c, 20);
assert_checkfalse(isdef("mymacro2"));
clear mymacro;
assert_checkfalse(isdef("mymacro"));

