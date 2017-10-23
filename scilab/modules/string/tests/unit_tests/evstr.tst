// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// =============================================================================
// unit tests evstr
// =============================================================================
a = 1;
b = 2;
Z = ["a","b"];
res = evstr(Z);
assert_checkequal(res(1), a);
assert_checkequal(res(2), b);
// =============================================================================
a = 1;
b = 2;
Z = list(["%(1)","%(1)-%(2)"], ["a+1","b+1"]);
res = evstr(Z);
assert_checkequal(res, [2 -1]);
// =============================================================================
assert_checktrue(isnan(evstr(%nan)));
assert_checktrue(isinf(evstr(%inf)));
assert_checktrue(isnan(evstr("%nan")));
assert_checktrue(isinf(evstr("%inf")));
assert_checktrue(isnan(evstr("Nan")));
assert_checktrue(isinf(evstr("Inf")));
assert_checktrue(isnan(evstr("NaN")));
assert_checktrue(isinf(evstr(-%inf)));
assert_checktrue(isinf(evstr("-%inf")));
assert_checktrue(isinf(evstr("-Inf")));
// =============================================================================
Scilab_value = 3;
assert_checkequal(Scilab_value, evstr("Scilab_value"));
// =============================================================================
clear toto;
assert_checkfalse(execstr("evstr(''toto'')", "errcatch") == 0);
assert_checkfalse(execstr("evstr(toto)", "errcatch") == 0);
// =============================================================================
s = rand(3,3,3);
assert_checkequal(execstr("evstr(s)", "errcatch"), 0);
// =============================================================================
function foo(), endfunction
assert_checkequal(execstr("evstr(''foo'')", "errcatch"), 0);
// =============================================================================
assert_checkequal(evstr(""), []);
// =============================================================================
// With some concatenations:
assert_checkequal(evstr("1 2 3"), 1:3);
assert_checkequal(evstr(["1 2" "3" "4 5"]), 1:5);
assert_checkequal(evstr("1 ; 2 ; 3"), [1;2;3]);
assert_checkequal(evstr(["1;2" ; "3" ; "4;5"]), (1:5)');

assert_checkequal(evstr(["1 2" ; "3 4"]), [1 2 ; 3 4]);
assert_checkequal(evstr(["1;2" "3;4"]), [1 3 ; 2 4]);

// =============================================================================
// With some comments or "//" substrings
//  Scalar expression
assert_checkequal(evstr("// test"), []);
assert_checkequal(evstr("1 // test"), 1);
assert_checkequal(evstr("1 2 // test"), [1 2]);
assert_checkequal(evstr("[1 2] // test"), [1 2]);
assert_checkequal(evstr("1 ; 2 // test"), [1 ; 2]);
assert_checkequal(evstr("[1;2] // test"), [1 ; 2]);
assert_checkequal(evstr("""//"" // A''b"), "//");
assert_checkequal(evstr(" ""//"" // A''b"), "//");
assert_checkequal(evstr(" "" //"" // A''b // c""d "), " //");

//  Column of expressions:
assert_checkequal(evstr(["// test" ; "// test line 2"]), []);
assert_checkequal(evstr(["1 2"; "// test" ; "3 4"]), [1 2; 3 4]);
assert_checkequal(evstr(["1 // abc"; "// no result" ; "3 // def"]), [1 ; 3]);
assert_checkequal(evstr(["1 // abc"; " // no result" ; "3 // def"]), [1 ; 3]);
assert_checkequal(evstr(["1 // abc"; "[] // no result" ; "3 // def"]), [1 ; 3]);
res = ["abc"; "http://a.b" ; "def"];
assert_checkequal(evstr(["""abc"""; """http://a.b""" ; """def"""]), res);
assert_checkequal(evstr(["""abc"""; """http://a.b"" // test" ; """def"""]), res);
assert_checkequal(evstr(["""abc"""; """http://a.b""// test" ; """def"""]), res);
assert_checkequal(evstr(["""abc"""; "''http://a.b'' // test" ; """def"""]), res);

res = ["abc"; "//a.b" ; "def"];
assert_checkequal(evstr(["""abc"""; """//a.b""" ; """def"""]), res);
assert_checkequal(evstr(["""abc"" // test"; """//a.b""" ; """def"""]), res);
assert_checkequal(evstr(["""abc // test"""]), "abc // test");
assert_checkequal(evstr(["""abc // test"" // comment"]), "abc // test");

//  Row of expressions:
assert_checkequal(evstr(["// test", "// test line 2"]), []);
assert_checkequal(evstr(["1 2", "// test", "3 4"]), [1 2 3 4]);
assert_checkequal(evstr(["1 // abc", "// no result", "3 // def"]), [1 3]);
assert_checkequal(evstr(["1 // abc", " // no result", "3 // def"]), [1 3]);
assert_checkequal(evstr(["1 // abc", "[] // no result", "3 // def"]), [1 3]);

ab = """ab""";
ht = """http://a.b""";
res = ["ab", "http://a.b", "ab // cd"];
assert_checkequal(evstr([ab, ht, """ab // cd"""]), res);
assert_checkequal(evstr([ab, ht+" // test", "''ab // cd''"]), res);
assert_checkequal(evstr([ab, ht+"// test", """ab // cd"""]), res);
assert_checkequal(evstr([ab, "''http://a.b'' // test", "''ab // cd''"]), res);
assert_checkequal(evstr(["// ab", """3"" // http://a.b", """ab // cd"""]), ["3" "ab // cd"]);
assert_checkequal(evstr(["// ab", """3"" ""//"" ""http://a.b""", """ab // cd"""]), ..
  ["3" "//" "http://a.b" "ab // cd"]);
assert_checkequal(evstr([ab, ht, """ab // cd"""]), res);

//  Matrix of expressions:
assert_checkequal(evstr(["1;2 // A", "3;4 // B //"]), [1 3 ; 2 4]);
assert_checkequal(evstr(["1 2 // A"; "3 4 // B //"]), [1 2 ; 3 4]);
assert_checkequal(evstr(["1;2 // A", "3;4 // B //"; "5// C" "6// D"]), [1 3 ; 2 4 ; 5 6]);
assert_checkequal(evstr(["1 // un", "2 // deux"; "3 // trois", "4 // quatre"]), [1 2 ; 3 4]);

// Can't be easily fixed (but very permissive):
in = [" ""a""; ""b"" // A''b", " ""c""; ""d"" // E''F"];
assert_checkfalse(evstr(in)==["a" "c";"b" "d"]);
assert_checkequal(evstr(in), ["a" ; "b"]);
