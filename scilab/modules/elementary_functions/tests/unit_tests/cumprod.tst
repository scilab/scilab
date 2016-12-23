// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
// Copyright (C) 2012 - Scilab Enterprises - Cedric Delamarre
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// Check error
//==============================================================================
//float matrices
d=[1 10;254 9];

assert_checkfalse(execstr("cumprod(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "cumprod",2,"""*"",""r"",""c"",""m"",""native"",""double""");
assert_checkerror("cumprod(d, ""orient"")", refMsg);

assert_checkfalse(execstr("cumprod(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "cumprod", 2);
assert_checkerror("cumprod(d, [""r"", ""c""])", refMsg);

//==============================================================================
// matrices of integer
i = uint8(d);

assert_checkfalse(execstr("cumprod(i, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "cumprod",2,"""*"",""r"",""c"",""m"",""native"",""double""");
assert_checkerror("cumprod(i, ""orient"")", refMsg);

assert_checkfalse(execstr("cumprod(i, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "cumprod", 2);
assert_checkerror("cumprod(i, [""r"", ""c""])", refMsg);

assert_checkfalse(execstr("cumprod(i,""r"", ""nat"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: %s or %s expected.\n"),"cumprod", 3, """native""", """double""");
assert_checkerror("cumprod(i,""r"", ""nat"")", refMsg);

assert_checkfalse(execstr("cumprod(i,""r"", [""nat"" ""dble""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "cumprod", 3);
assert_checkerror("cumprod(i,""r"", [""nat"" ""dble""])", refMsg);

assert_checkfalse(execstr("cumprod(i,""orient"", ""t"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "cumprod",2,"""*"",""r"",""c"",""m""");
assert_checkerror("cumprod(i,""orient"", ""t"")", refMsg);

assert_checkfalse(execstr("cumprod(i,1,1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "cumprod", 3);
assert_checkerror("cumprod(i,1,1)", refMsg);

//==============================================================================
// sparse matrices
d = sparse(d);
assert_checkfalse(execstr("cumprod(d, ""orient"")","errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "cumprod",2,"""*"",""r"",""c"",""m""");
assert_checkerror("cumprod(d, ""orient"")", refMsg);

assert_checkfalse(execstr("cumprod(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"cumprod",2);
assert_checkerror("cumprod(d, [""r"", ""c""])", refMsg);

//==============================================================================
// boolean matrices
d = [%f %t;%t %f];
assert_checkfalse(execstr("cumprod(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"cumprod",2,"""*"",""r"",""c"",""m"",""native"",""double""");
assert_checkerror("cumprod(d, ""orient"")", refMsg);

assert_checkfalse(execstr("cumprod(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"),"cumprod",2);
assert_checkerror("cumprod(d, [""r"", ""c""])", refMsg);

assert_checkfalse(execstr("cumprod(d,""r"", ""nat"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: %s or %s expected.\n"),"cumprod", 3, """native""", """double""");
assert_checkerror("cumprod(d,""r"", ""nat"")", refMsg);

assert_checkfalse(execstr("cumprod(d,""r"", [""nat"" ""dble""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"),"cumprod",3);
assert_checkerror("cumprod(d,""r"", [""nat"" ""dble""])", refMsg);

assert_checkfalse(execstr("cumprod(d,""orient"", ""t"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
"cumprod",2,"""*"",""r"",""c"",""m""");
assert_checkerror("cumprod(d,""orient"", ""t"")", refMsg);

assert_checkfalse(execstr("cumprod(d,1,1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"cumprod",3);
assert_checkerror("cumprod(d,1,1)", refMsg);

//==============================================================================
// sparse boolean matrices
d = sparse(d);
assert_checkfalse(execstr("cumprod(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
"cumprod",2,"""*"",""r"",""c"",""m""");
assert_checkerror("cumprod(d, ""orient"")", refMsg);

assert_checkfalse(execstr("cumprod(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"cumprod",2);
assert_checkerror("cumprod(d, [""r"", ""c""])", refMsg);

assert_checkfalse(execstr("cumprod(d,""r"", ""nat"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"cumprod", 3, "native", "double");
assert_checkerror("cumprod(d,""r"", ""nat"")", refMsg);

assert_checkfalse(execstr("cumprod(d,""r"", [""nat"" ""dble""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"cumprod",3);
assert_checkerror("cumprod(d,""r"", [""nat"" ""dble""])", refMsg);

assert_checkfalse(execstr("cumprod(d,""orient"", ""t"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
"cumprod",2,"""*"",""r"",""c"",""m""");
assert_checkerror("cumprod(d,""orient"", ""t"")", refMsg);

assert_checkfalse(execstr("cumprod(d,1,1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"cumprod",3);
assert_checkerror("cumprod(d,1,1)", refMsg);

//==============================================================================
// hypermatrices
d = rand(2,2,2);
assert_checkfalse(execstr("cumprod(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "cumprod",2,"""*"",""r"",""c"",""m"",""native"",""double""");
assert_checkerror("cumprod(d, ""orient"")", refMsg);

assert_checkfalse(execstr("cumprod(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"),"cumprod",2);
assert_checkerror("cumprod(d, [""r"", ""c""])", refMsg);

//==============================================================================
// polynome
s = poly(0, "s");
d = [s s^2;s*%i 1];
assert_checkfalse(execstr("cumprod(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"cumprod",2,"""*"",""r"",""c"",""m"",""native"",""double""");
assert_checkerror("cumprod(d, ""orient"")", refMsg);

assert_checkfalse(execstr("cumprod(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"),"cumprod",2);
assert_checkerror("cumprod(d, [""r"", ""c""])", refMsg);

//==============================================================================
// rational matrices
s = poly(0, "s");
d = [1/s 1/s^2; 1/s 1];
assert_checkfalse(execstr("cumprod(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
"cumprod",2,"""*"",""r"",""c"",""m""");
assert_checkerror("cumprod(d, ""orient"")", refMsg);

assert_checkfalse(execstr("cumprod(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"cumprod",2);
assert_checkerror("cumprod(d, [""r"", ""c""])", refMsg);

//cumprod
//Empty matrices
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(cumprod([],typ(:)), []);
    assert_checkequal(cumprod([],"*",typ(:)), []);
    assert_checkequal(cumprod([],1,typ(:)), []);
    assert_checkequal(cumprod([],2,typ(:)), []);
    assert_checkequal(cumprod([],3,typ(:)), []);
end

//=======================================================================
//float matrices
d=[1 10;254 9];
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(cumprod(d,typ(:)), [1,2540;254,22860]);
    assert_checkequal(cumprod(d,"*",typ(:)), [1,2540;254,22860]);
    assert_checkequal(cumprod(d,1,typ(:)), [1,10;254,90]);
    assert_checkequal(cumprod(d,2,typ(:)), [1,10;254,2286]);
    assert_checkequal(cumprod(d,3,typ(:)), d);
end

//hypermatrices of floats
d=[1 10;254 9];d(1,1,2)=1;
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(cumprod(d,typ(:)), matrix([1;254;2540;22860;22860;0;0;0], [2,2,2]));
    assert_checkequal(cumprod(d,"*",typ(:)), matrix([1;254;2540;22860;22860;0;0;0], [2,2,2]));
    assert_checkequal(cumprod(d,1,typ(:)), matrix([1;254;10;90;1;0;0;0], [2,2,2]));
    assert_checkequal(cumprod(d,2,typ(:)), matrix([1;254;10;2286;1;0;0;0], [2,2,2]));
    assert_checkequal(cumprod(d,3,typ(:)), matrix([1;254;10;9;1;0;0;0], [2,2,2]));
    assert_checkequal(cumprod(d,5,typ(:)), d);
end

//=======================================================================
//Integers
i=uint8([1 10;254 9]);
for typ=list(list(),list("native"));
    assert_checkequal(cumprod(i,typ(:)), uint8([1,236;254,76]));
    assert_checkequal(cumprod(i,"*",typ(:)), uint8([1,236;254,76]));
    assert_checkequal(cumprod(i,1,typ(:)), uint8([1,10;254,90]));
    assert_checkequal(cumprod(i,2,typ(:)), uint8([1,10;254,238]));
    assert_checkequal(cumprod(i,3,typ(:)), double(i));
end

assert_checkequal(cumprod(i,"double"), [1,2540;254,22860]);
assert_checkequal(cumprod(i,"*","double"), [1,2540;254,22860]);
assert_checkequal(cumprod(i,1,"double"), [1,10;254,90]);
assert_checkequal(cumprod(i,2,"double"), [1,10;254,2286]);
assert_checkequal(cumprod(i,3,"double"), double(i));

//with hypermatrices
i=uint8([1 10;254 9]);i(1,1,2)=uint8(1);
for typ=list(list(),list("native"));
    assert_checkequal(cumprod(i,typ(:)), matrix(uint8([1;254;236;76;76;0;0;0]), [2,2,2]));
    assert_checkequal(cumprod(i,"*",typ(:)), matrix(uint8([1;254;236;76;76;0;0;0]), [2,2,2]));
    assert_checkequal(cumprod(i,1,typ(:)), matrix(uint8([1;254;10;90;1;0;0;0]), [2,2,2]));
    assert_checkequal(cumprod(i,2,typ(:)), matrix(uint8([1;254;10;238;1;0;0;0]), [2,2,2]));
    assert_checkequal(cumprod(i,3,typ(:)), matrix(uint8([1;254;10;9;1;0;0;0]), [2,2,2]));
    assert_checkequal(cumprod(i,5,typ(:)), double(i));
end


assert_checkequal(cumprod(i,"double"), matrix([1;254;2540;22860;22860;0;0;0], [2,2,2]));
assert_checkequal(cumprod(i,"*","double"), matrix([1;254;2540;22860;22860;0;0;0], [2,2,2]));
assert_checkequal(cumprod(i,1,"double"), matrix([1;254;10;90;1;0;0;0], [2,2,2]));
assert_checkequal(cumprod(i,2,"double"), matrix([1;254;10;2286;1;0;0;0], [2,2,2]));
assert_checkequal(cumprod(i,3,"double"), matrix([1;254;10;9;1;0;0;0], [2,2,2]));
assert_checkequal(cumprod(i,5,"double"), double(i));

//=======================================================================
//Matrices of Polynomials
s=%s;p=[s s+1;s^2 0];
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(cumprod(p,typ(:)), [s,s^3+s^4;s^3,0*s]);
    assert_checkequal(cumprod(p,"*",typ(:)), [s,s^3+s^4;s^3,0*s]);
    assert_checkequal(cumprod(p,1,typ(:)), [s,1+s;s^3,0*s]);
    assert_checkequal(cumprod(p,2,typ(:)), [s,s+s^2;s^2,0*s]);
    assert_checkequal(cumprod(p,3,typ(:)), p);
end

//with hypermatrices
s=%s;p=[s s+1;s^2 0];p(1,1,2)=-1;
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(cumprod(p,typ(:)), matrix([s;s^3;s^3+s^4;0*s;0*s;0*s;0*s;0*s], [2,2,2]));
    assert_checkequal(cumprod(p,"*",typ(:)), matrix([s;s^3;s^3+s^4;0*s;0*s;0*s;0*s;0*s], [2,2,2]));
    assert_checkequal(cumprod(p,1,typ(:)), matrix([s;s^3;1+s;0*s;-1;0*s;0*s;0*s], [2,2,2]));
    assert_checkequal(cumprod(p,2,typ(:)), matrix([s;s^2;s+s^2;0*s;-1;0*s;0*s;0*s], [2,2,2]));
    assert_checkequal(cumprod(p,3,typ(:)), matrix([s;s^2;1+s;0*s;-s;0*s;0*s;0*s], [2,2,2]));
    assert_checkequal(cumprod(p,5,typ(:)), p);
end
//=======================================================================
//Matrices of rationals
s=%s;r=1.0 ./[s s+1;s^2 1];
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(cumprod(r,typ(:)), [1,1;1,1]./[s,s^3+s^4;s^3,s^3+s^4]);
    assert_checkequal(cumprod(r,"*",typ(:)), [1,1;1,1]./[s,s^3+s^4;s^3,s^3+s^4]);
    assert_checkequal(cumprod(r,1,typ(:)), [1,1;1,1]./[s,1+s;s^3,1+s]);
    assert_checkequal(cumprod(r,2,typ(:)), [1,1;1,1]./[s,s+s^2;s^2,s^2]);
    assert_checkequal(cumprod(r,3,typ(:)), r);
end

//=======================================================================
///Matrices of boolean

b=[%t %t;%f %t];
for typ=list(list(),list("double"));
    assert_checkequal(cumprod(b,typ(:)), [1,0;0,0]);
    assert_checkequal(cumprod(b,"*",typ(:)), [1,0;0,0]);
    assert_checkequal(cumprod(b,1,typ(:)), [1,1;0,1]);
    assert_checkequal(cumprod(b,2,typ(:)), [1,1;0,0]);
    assert_checkequal(cumprod(b,3,typ(:)), double(b));
end
assert_checkequal(cumprod(b,"native"), [%t,%f;%f,%f]);
assert_checkequal(cumprod(b,"*","native"), [%t,%f;%f,%f]);
assert_checkequal(cumprod(b,1,"native"), [%t,%t;%f,%t]);
assert_checkequal(cumprod(b,2,"native"), [%t,%t;%f,%f]);
assert_checkequal(cumprod(b,3,"native"), b);

//with hypermatrices
b=[%t %t;%f %t];b(1,1,2)=%f;
for typ=list(list(),list("double"));
    assert_checkequal(cumprod(b,typ(:)), matrix([1;0;0;0;0;0;0;0], [2,2,2]));
    assert_checkequal(cumprod(b,"*",typ(:)), matrix([1;0;0;0;0;0;0;0], [2,2,2]));
    assert_checkequal(cumprod(b,1,typ(:)), matrix([1;0;1;1;0;0;0;0], [2,2,2]));
    assert_checkequal(cumprod(b,2,typ(:)), matrix([1;0;1;0;0;0;0;0], [2,2,2]));
    assert_checkequal(cumprod(b,3,typ(:)), matrix([1;0;1;1;0;0;0;0], [2,2,2]));
    assert_checkequal(cumprod(b,5,typ(:)), double(b));
end

assert_checkequal(cumprod(b,"native"), matrix([%t;%f;%f;%f;%f;%f;%f;%f], [2,2,2]));
assert_checkequal(cumprod(b,"*","native"), matrix([%t;%f;%f;%f;%f;%f;%f;%f], [2,2,2]));
assert_checkequal(cumprod(b,1,"native"), matrix([%t;%f;%t;%t;%f;%f;%f;%f], [2,2,2]));
assert_checkequal(cumprod(b,2,"native"), matrix([%t;%f;%t;%f;%f;%f;%f;%f], [2,2,2]));
assert_checkequal(cumprod(b,3,"native"), matrix([%t;%f;%t;%t;%f;%f;%f;%f], [2,2,2]));
assert_checkequal(cumprod(b,5,"native"), b);

//=======================================================================
//sparse matrices of floats
s=sparse([1 10 0;-1 0 9]);
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(cumprod(s,typ(:)), sparse([1,1;1,2;2,1],[1;-10;-1],[2,3]));
    assert_checkequal(cumprod(s,"*",typ(:)), sparse([1,1;1,2;2,1],[1;-10;-1],[2,3]));
    assert_checkequal(cumprod(s,1,typ(:)), sparse([1,1;1,2;2,1],[1;10;-1],[2,3]));
    assert_checkequal(cumprod(s,2,typ(:)), sparse([1,1;1,2;2,1],[1;10;-1],[2,3]));
    assert_checkequal(cumprod(s,3,typ(:)), s);
end

//=======================================================================
//sparse  matrices of boolean
bs=sparse([%t %t %f;%t %t %t]);
for typ=list(list(),list("double"));
    assert_checkequal(cumprod(bs,typ(:)), sparse([1,1;1,2;2,1;2,2],[1;1;1;1],[2,3]));
    assert_checkequal(cumprod(bs,"*",typ(:)), sparse([1,1;1,2;2,1;2,2],[1;1;1;1],[2,3]));
    assert_checkequal(cumprod(bs,1,typ(:)), sparse([1,1;1,2;2,1;2,2],[1;1;1;1],[2,3]));
    assert_checkequal(cumprod(bs,2,typ(:)), sparse([1,1;1,2;2,1;2,2;2,3],[1;1;1;1;1],[2,3]));
    assert_checkequal(cumprod(bs,3,typ(:)), bool2s(bs));
end

assert_checkequal(cumprod(bs,"native"), sparse([1,1;1,2;2,1;2,2],[%t;%t;%t;%t],[2,3]));
assert_checkequal(cumprod(bs,"*","native"), sparse([1,1;1,2;2,1;2,2],[%t;%t;%t;%t],[2,3]));

assert_checkequal(cumprod(bs,1,"native"), sparse([1,1;1,2;2,1;2,2],[%t;%t;%t;%t],[2,3]));

assert_checkequal(cumprod(bs,2,"native"), sparse([1,1;1,2;2,1;2,2;2,3],[%t;%t;%t;%t;%t],[2,3]));
assert_checkequal(cumprod(bs,3,"native"), bs);

// TODO : test the "m" option
