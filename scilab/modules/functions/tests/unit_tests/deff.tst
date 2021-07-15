// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2020 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->


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

// Other syntaxes
clear Fun
deff(["r = Fun(x)" "r = x.^2 - 3"]);    // Only one deff argin, being a vector
assert_checktrue(isdef("Fun","l"));
assert_checkequal(Fun(4), 13);
clear Fun
deff "r = Fun(x) r = x.^2 - 3";         // Only one deff argin, being a scalar
assert_checktrue(isdef("Fun","l"));
assert_checkequal(Fun(4), 13);
clear Fun
deff("r = Fun(x) x.^2 - 3");    // Fun argout implicit assignment (missing in definition)
assert_checktrue(isdef("Fun","l"));
assert_checkequal(Fun(4), 13);

// output = deff(..)
// =================
clear Fun myFun
myFun = deff("res = Fun(a,b)",["res = a+b" "res = res.^2"]);
assert_checkequal(myFun(1,1), 4);
assert_checkfalse(isdef("Fun","l"));

clear myFun
e = execstr("myFun = deff(""res = Fun(a,b)"",[""res = a+"" ""res = res.^2""]);", "errcatch");
assert_checktrue(e <> 0);             // syntax error ^^^
assert_checkfalse(isdef("myFun","l"));
assert_checkfalse(isdef("Fun","l"));

in = list(["r = Fun(x)" "r = x.^2"], .. // Only one deff argin, being a vector
           "r = Fun(x) r = x.^2", ..    // Only one deff scalar argin
           "r = Fun(x) x.^2",..  // Fun argout implicit assignment (missing in definition):
           "[a,b] = Fun(x) a=x.^2; b=x-3"); // Only one deff scalar argin. 2 outputs
for argin = in
    clear myFun
    myFun = deff(argin);
    assert_checktrue(isdef("myFun","l"));
    assert_checkfalse(isdef("Fun","l"));
    if grep(argin(1), "[a,b]") <> []
        [u,v] = myFun(4);
        assert_checkequal([u v], [16 1]);
    else
        assert_checkequal(myFun(4), 16);
    end

    // Anonymous "@"
    clear myFun
    argin = strsubst(argin, "Fun", "@")
    myFun = deff(argin);
    assert_checktrue(isdef("myFun","l"));
    assert_checkequal(myFun(4), 16);
end
