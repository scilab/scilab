// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->
// <-- NO CHECK REF -->

//===============================
// unit tests strcat
//===============================
msg = sprintf(_("%s: Wrong number of input arguments: %d or %d expected.\n"), "strcat", 1, 3);
assert_checkerror("strcat()", msg);

msg = sprintf(_("%s: Wrong type for input argument #%d: String expected.\n"), "strcat", 1);
assert_checkerror("strcat(%f)", msg);

msg = sprintf(_("%s: Wrong type for input argument #%d: ''%s'' or ''%s'' expected.\n"), "strcat", 3, "c", "r");
assert_checkerror("strcat(""test"", """", ""f"")", msg);

msg = sprintf(_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "strcat", 2);
assert_checkerror("strcat([],3)", msg);

assert_checkequal(strcat(["abc" "abd","aa" "bxe"]), "abcabdaabxe");
assert_checkequal(strcat(["abc";"abd";"aa";"bxe"]), "abcabdaabxe");
assert_checkequal(strcat(["abc" "abd";"aa" "bxe"]), "abcaaabdbxe");

assert_checkequal(strcat(["abc","abd","aa","bxe"],","), "abc,abd,aa,bxe");
assert_checkequal(strcat("abc"), "abc");

assert_checkequal(strcat("abc","sd"), "abc");
assert_checkequal(strcat(""), "");
assert_checkequal(strcat(["" ""]), "");
assert_checkequal(strcat(["";""]), "");

assert_checkequal(strcat([]), []);
assert_checkequal(strcat([], ""), []);
assert_checkequal(strcat([], "3"), []);

assert_checkequal(strcat(["abc","abd","aa","bxe"],"--"), "abc--abd--aa--bxe");

assert_checkequal(strcat(["abc" "abd";"aa" "bxe"], '-', 'r'), ["abc-aa" "abd-bxe"]);
assert_checkequal(strcat(["abc" "abd";"aa" "bxe"], '-', 'c'), ["abc-abd";"aa-bxe"]);

assert_checkequal(strcat(["" "A" "" "B" ""], ","), [",A,,B,"]);
assert_checkequal(strcat(["";"A";"";"B";""], ","), [",A,,B,"]);

assert_checkequal(strcat(["" "A" "";"B" "" "C"], ",", "r"), [",B" "A," ",C"]);
assert_checkequal(strcat(["" "A" "";"B" "" "C"], ",", "c"), [",A,";"B,,C"]);
