// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
// Copyright (C) 2013 - Scilab Enterprises - Adeline CARNIS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// Check error
d=[1 10;254 9];

assert_checkfalse(execstr("sum(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "sum",2,"""*"",""r"",""c"",""m"",""native"",""double""");
assert_checkerror("sum(d, ""orient"")", refMsg);

assert_checkfalse(execstr("sum(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "sum", 2);
assert_checkerror("sum(d, [""r"", ""c""])", refMsg);

assert_checkfalse(execstr("sum(d, ""e"", ""double"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "sum",2,"""*"",""r"",""c"",""m""");
assert_checkerror("sum(d, ""e"", ""double"")", refMsg);

assert_checkfalse(execstr("sum(d, -1.5, ""double"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: A positive scalar expected.\n"), "sum", 2);
assert_checkerror("sum(d, -1.5, ""double"")", refMsg);

//==============================================================================
// matrices of integer
i = uint8(d);

assert_checkfalse(execstr("sum(i, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "sum",2,"""*"",""r"",""c"",""m"",""native"",""double""");
assert_checkerror("sum(i, ""orient"")", refMsg);

assert_checkfalse(execstr("sum(i, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "sum", 2);
assert_checkerror("sum(i, [""r"", ""c""])", refMsg);

assert_checkfalse(execstr("sum(i,""r"", ""nat"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: %s or %s expected.\n"),"sum", 3, """native""", """double""");
assert_checkerror("sum(i,""r"", ""nat"")", refMsg);

assert_checkfalse(execstr("sum(i,""r"", [""nat"" ""dble""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "sum", 3);
assert_checkerror("sum(i,""r"", [""nat"" ""dble""])", refMsg);

assert_checkfalse(execstr("sum(i,""orient"", ""t"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "sum",2,"""*"",""r"",""c"",""m""");
assert_checkerror("sum(i,""orient"", ""t"")", refMsg);

assert_checkfalse(execstr("sum(i,1,1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "sum", 3);
assert_checkerror("sum(i,1,1)", refMsg);

//==============================================================================
// sparse matrices
d = sparse(d);
assert_checkfalse(execstr("sum(d, ""orient"")","errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "sum",2,"""*"",""r"",""c"",""m""");
assert_checkerror("sum(d, ""orient"")", refMsg);

assert_checkfalse(execstr("sum(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"sum",2);
assert_checkerror("sum(d, [""r"", ""c""])", refMsg);

//==============================================================================
// boolean matrices
d = [%f %t;%t %f];
assert_checkfalse(execstr("sum(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"sum",2,"""*"",""r"",""c"",""m"",""native"",""double""");
assert_checkerror("sum(d, ""orient"")", refMsg);

assert_checkfalse(execstr("sum(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"),"sum",2);
assert_checkerror("sum(d, [""r"", ""c""])", refMsg);

assert_checkfalse(execstr("sum(d,""r"", ""nat"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: %s or %s expected.\n"),"sum", 3, """native""", """double""");
assert_checkerror("sum(d,""r"", ""nat"")", refMsg);

assert_checkfalse(execstr("sum(d,""r"", [""nat"" ""dble""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"),"sum",3);
assert_checkerror("sum(d,""r"", [""nat"" ""dble""])", refMsg);

assert_checkfalse(execstr("sum(d,""orient"", ""t"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
"sum",2,"""*"",""r"",""c"",""m""");
assert_checkerror("sum(d,""orient"", ""t"")", refMsg);

assert_checkfalse(execstr("sum(d,1,1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"sum",3);
assert_checkerror("sum(d,1,1)", refMsg);

//==============================================================================
// sparse boolean matrices
d = sparse(d);
assert_checkfalse(execstr("sum(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
"sum",2,"""*"",""r"",""c"",""m""");
assert_checkerror("sum(d, ""orient"")", refMsg);

assert_checkfalse(execstr("sum(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"sum",2);
assert_checkerror("sum(d, [""r"", ""c""])", refMsg);

assert_checkfalse(execstr("sum(d,""r"", ""nat"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: ""%s"" or ""%s"" expected.\n"),"sum", 3, "native", "double");
assert_checkerror("sum(d,""r"", ""nat"")", refMsg);

assert_checkfalse(execstr("sum(d,""r"", [""nat"" ""dble""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"sum",3);
assert_checkerror("sum(d,""r"", [""nat"" ""dble""])", refMsg);

assert_checkfalse(execstr("sum(d,""orient"", ""t"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
"sum",2,"""*"",""r"",""c"",""m""");
assert_checkerror("sum(d,""orient"", ""t"")", refMsg);

assert_checkfalse(execstr("sum(d,1,1)"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"),"sum",3);
assert_checkerror("sum(d,1,1)", refMsg);

//==============================================================================
// hypermatrices
d = rand(2,2,2);
assert_checkfalse(execstr("sum(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"), "sum",2,"""*"",""r"",""c"",""m"",""native"",""double""");
assert_checkerror("sum(d, ""orient"")", refMsg);

assert_checkfalse(execstr("sum(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"),"sum",2);
assert_checkerror("sum(d, [""r"", ""c""])", refMsg);

//==============================================================================
// polynome
s = poly(0, "s");
d = [s s^2;s*%i 1];
assert_checkfalse(execstr("sum(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),"sum",2,"""*"",""r"",""c"",""m"",""native"",""double""");
assert_checkerror("sum(d, ""orient"")", refMsg);

assert_checkfalse(execstr("sum(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: A scalar string expected.\n"),"sum",2);
assert_checkerror("sum(d, [""r"", ""c""])", refMsg);

//==============================================================================
// rational matrices
s = poly(0, "s");
d = [1/s 1/s^2; 1/s 1];
assert_checkfalse(execstr("sum(d, ""orient"")"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),..
"sum",2,"""*"",""r"",""c"",""m""");
assert_checkerror("sum(d, ""orient"")", refMsg);

assert_checkfalse(execstr("sum(d, [""r"", ""c""])"   ,"errcatch") == 0);
refMsg = msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"),"sum",2);
assert_checkerror("sum(d, [""r"", ""c""])", refMsg);

//empty matrices
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(sum([], typ(:)), 0);
    assert_checkequal(sum([], "*", typ(:)), 0);

    assert_checkequal(sum([], 1, typ(:)), []);
    assert_checkequal(sum([], 2, typ(:)), []);
    assert_checkequal(sum([], 3, typ(:)), []);
end
//=======================================================================
//float matrices
d=[1 10;254 9];
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(sum(d, typ(:)), 274);
    assert_checkequal(sum(d, "*", typ(:)), 274);
    assert_checkequal(sum(d, 1, typ(:)), [255 19]);
    assert_checkequal(sum(d, 2, typ(:)), [11; 263]);
    assert_checkequal(sum(d, 3, typ(:)), d);
end

//hypermatrices of floats
d(1,1,2)=1;
for typ=T
    assert_checkequal(sum(d, typ(:)), 275);
    assert_checkequal(sum(d, "*", typ(:)), 275);
    assert_checkequal(sum(d, 1, typ(:)), matrix([255;19;1;0], [1,2,2]));
    assert_checkequal(sum(d, 2, typ(:)), matrix([11;263;1;0], [2,1,2]));
    assert_checkequal(sum(d, 3, typ(:)), [2,10;254,9]);
    assert_checkequal(sum(d, 5, typ(:)), d);
end

//=======================================================================
//matrices of short integers
// Caution : uint8([11;263]) is equal to uint8([11;7])
i=uint8([1 10;254 9]);
T = list(list(),list("native"));
for typ=T
    assert_checkequal(sum(i, typ(:)), uint8(274));
    assert_checkequal(sum(i, "*", typ(:)), uint8(274));
    assert_checkequal(sum(i, 1, typ(:)), uint8([255 19]));
    assert_checkequal(sum(i, 2, typ(:)), uint8([11;263]));
    assert_checkequal(sum(i, 3, typ(:)), double(i));
end

assert_checkequal(sum(i, "double"), 274);
assert_checkequal(sum(i, "*", "double"), 274);
assert_checkequal(sum(i, 1, "double"), [255 19]);
assert_checkequal(sum(i, 2, "double"), [11; 263]);
assert_checkequal(sum(i, 3, "double"), double(i));

//with hypermatrices
i(1,1,2)=uint8(1);
T = list(list(),list("native"));
for typ=T
    assert_checkequal(sum(i, typ(:)), uint8(275));
    assert_checkequal(sum(i, "*", typ(:)), uint8(275));
    assert_checkequal(sum(i, 1, typ(:)), matrix(uint8([255;19;1;0]), [1,2,2]));
    assert_checkequal(sum(i, 2, typ(:)), matrix(uint8([11;263;1;0]), [2,1,2]));
    assert_checkequal(sum(i, 3, typ(:)), uint8([2,10;254,9]));
    assert_checkequal(sum(i, 5, typ(:)), double(i));
end

assert_checkequal(sum(i, "double"), 275);
assert_checkequal(sum(i, "*", "double"), 275);
assert_checkequal(sum(i, 1, "double"), matrix([255;19;1;0], [1,2,2]));
assert_checkequal(sum(i, 2, "double"), matrix([11;263;1;0], [2,1,2]));
assert_checkequal(sum(i, 3, "double"), [2,10;254,9]);

//=======================================================================
//Matrices of polynomials
s=%s;
p=[s s+1;s^2 0];
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(sum(p, typ(:)), 1+2*s+s^2);
    assert_checkequal(sum(p, "*", typ(:)), 1+2*s+s^2);
    assert_checkequal(sum(p, 1, typ(:)), [s + s^2, 1 + s]);
    assert_checkequal(sum(p, 2, typ(:)), [1+2*s;s^2]);
    assert_checkequal(sum(p, 3, typ(:)), p);
end

//with hypermatrices
p(1,1,2)=-1;
for typ=T
    assert_checkequal(sum(p, typ(:)), 2*s+s^2);
    assert_checkequal(sum(p, "*", typ(:)), 2*s+s^2);
    assert_checkequal(sum(p, 1, typ(:)), matrix([s+s^2;1+s;-1;0*s], [1,2,2]));
    assert_checkequal(sum(p, 2, typ(:)), matrix([1+2*s;s^2;-1;0*s], [2,1,2]));
    assert_checkequal(sum(p, 3, typ(:)), [-1+s,1+s;s^2,0]);
    assert_checkequal(sum(p, 5, typ(:)), p);
end

//=======================================================================
//Matrices of rationals
s=%s;
r=1.0 ./[s s+1;s^2 1];
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(sum(r, typ(:)), (1+2*s+3*s^2+s^3)./(s^2+s^3));
    assert_checkequal(sum(r, "*", typ(:)), (1+2*s+3*s^2+s^3)./(s^2+s^3));
    assert_checkequal(sum(r, 1, typ(:)), [1+s,2+s]./[s^2,1+s]);
    assert_checkequal(sum(r, 2, typ(:)), [1+2*s;1+s^2]./[s+s^2;s^2]);
    assert_checkequal(sum(r, 3, typ(:)), r);
end

//=======================================================================
///Matrices of boolean

b=[%t %t %f %t];
T = list(list(),list("double"));
for typ=T
    assert_checkequal(sum(b, typ(:)), 3);
    assert_checkequal(sum(b, "*", typ(:)), 3);
    assert_checkequal(sum(b, 1, typ(:)), [1 1 0 1]);
    assert_checkequal(sum(b, 2, typ(:)), 3);
    assert_checkequal(sum(b, 3, typ(:)), double(b));
end

assert_checkequal(sum(b, "native"), %t);
assert_checkequal(sum(b, "*", "native"), %t);
assert_checkequal(sum(b, 1, "native"), b);
assert_checkequal(sum(b, 2, "native"), %t);
assert_checkequal(sum(b, 3, "native"), b);

//with hypermatrices
b=[%t %t %f %t];b(1,1,2)=%f;
T = list(list(),list("double"));
for typ=T
    assert_checkequal(sum(b, typ(:)), 3);
    assert_checkequal(sum(b, "*", typ(:)), 3);
    assert_checkequal(sum(b, 1, typ(:)), matrix([1;1;0;1;0;0;0;0], [1,4,2]));
    assert_checkequal(sum(b, 2, typ(:)), matrix([3;0], [1,1,2]));
    assert_checkequal(sum(b, 3, typ(:)), [1 1 0 1]);
    assert_checkequal(sum(b, 5, typ(:)), double(b));
end

assert_checkequal(sum(b, "native"), %t);
assert_checkequal(sum(b, "*", "native"), %t);
assert_checkequal(sum(b, 1, "native"), matrix([%t;%t;%f;%t;%f;%f;%f;%f], [1,4,2]));
assert_checkequal(sum(b, 2, "native"), matrix([%t;%f], [1,1,2]));
assert_checkequal(sum(b, 3, "native"), [%t %t %f %t]);
assert_checkequal(sum(b, 5, "native"), b);

//=======================================================================
//sparse matrices of floats
s=sparse([1 10 0;0 254 9]);
T=list(list(),list("native"),list("double"));
for typ=T
    assert_checkequal(sum(s, typ(:)), 274);
    assert_checkequal(sum(s, "*", typ(:)), 274);
    assert_checkequal(sum(s, 1, typ(:)), sparse([1,264,9]));
    assert_checkequal(sum(s, 2, typ(:)), sparse([11;263]));
    assert_checkequal(sum(s, 3, typ(:)), s);
end

//=======================================================================
//sparse  matrices of boolean
bs=sparse([1 10 0;0 254 9])<>0;
T = list(list(),list("double"));
for typ=T
    assert_checkequal(sum(bs, typ(:)), 4);
    assert_checkequal(sum(bs, "*", typ(:)), 4);
    assert_checkequal(sum(bs, 1, typ(:)), sparse([1,2,1]));
    assert_checkequal(sum(bs, 2, typ(:)), sparse([2;2]));
    assert_checkequal(sum(bs, 3, typ(:)), bool2s(bs));
end

assert_checkequal(sum(bs, "native"), %t);
assert_checkequal(sum(bs, "*", "native"), %t);
assert_checkequal(sum(bs, 1, "native"), sparse([%t,%t,%t]));
assert_checkequal(sum(bs, 2, "native"), sparse([%t;%t]));
assert_checkequal(sum(bs, 3, "native"),bs);

// TODO : test the "m" option

