// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
// Copyright (C) 2017 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 9838 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/9838
//
// <-- Short Description -->
// evstr() poorly supported comments in expressions

assert_checkequal(evstr('//xxx'),[]);
t=["1";
   "2";
   "//xxx"];
assert_checkequal(evstr(t),[1;2]);

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
