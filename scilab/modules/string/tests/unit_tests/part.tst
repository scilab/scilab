// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2013, 2019 - Samuel GOUGEON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- NO CHECK REF -->

//===============================
// unit tests part
//===============================

assert_checkequal(part("",[]), "");

// compatibility 4.x
assert_checkequal(part("", 50), " ");
//===============================
s = part(["a","abc","abcd"],[1,1,2]);
if (s <> ["aa","aab","aab"]) then pause,end
//===============================
s1 = part(["a","abc","abcd"],[2,4,6]);
s2 = part(["a","abc","abcd"],[2,4,5]);
if (s1 <> s2)  then pause,end
if (s1 <> ["   ","b","bd"]) then pause,end
//===============================
N = 20;
s = part(" ",ones(1,N));
if (blanks(N) <> s) then pause,end
//===============================
if part("abc",1)<>"a" then pause,end
if part("abc",[1 1])<>"aa" then pause,end
if part("abc",[1;1])<>"aa" then pause,end
if part("abc",[])<>emptystr() then pause,end
if part("abc",5)<>" " then pause,end
if part("abc",5:6)<>"  " then pause,end
if or(part(["abc";"x"],1)<>["a";"x"]) then pause,end
if or(part(["abc";"x"],[1,1])<>["aa";"xx"]) then pause,end
if or(part(["abc";"x"],[1,2])<>["ab";"x "]) then pause,end
if or(part(["abc";"x"],[1;1])<>["aa";"xx"]) then pause,end
if or(part(["abc";"x"],[1;2])<>["ab";"x "]) then pause,end
//===============================


t = ["a","abc","abcd"];
assert_checkequal(part(t,[]),   ["" "" ""]);
assert_checkequal(part(t, 1),   ["a","a","a"]);
assert_checkequal(part(t,[1 1]),["aa","aa","aa"]);
assert_checkequal(part(t,[1 1 2]),["aa ","aab","aab"]);

N = 10; a=part("-",ones(1:N));
assert_checkequal(a,"----------");

N = 6; pat = "- ";
a=part(pat, ones(1:N).*.(1:length(pat)));
assert_checkequal(a,"- - - - - - ");

// With $ indices
// --------------
assert_checkequal(part("", $), "");
assert_checkequal(part("", $-1), "");
assert_checkequal(part("", $-1:$), "");
assert_checkequal(part("", $-1:$+1), " ");
assert_checkequal(part("", $-1:$+2), "  ");

a = part(["a string"  "another longer one"], $-2:$ );
assert_checkequal(a,["ing","one"]);

a = part("Hello world", $:-1:1);
assert_checkequal(a,"dlrow olleH");
a = part("Hello world", [ $ 5:6 ]);
assert_checkequal(a,"do ");
