// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

//===============================
// unit tests part
//===============================
EMPTY_STR = "";
BLANK = " ";
//===============================
s = part(EMPTY_STR ,[]);
if (s <> EMPTY_STR ) then pause,end
//===============================
// compatibility 4.x
N = 50;
s = part(EMPTY_STR ,N);
if (s <> BLANK) then pause,end
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
if part("abc",5)<>BLANK then pause,end
if part("abc",5:6)<>"  " then pause,end
if or(part(["abc";"x"],1)<>["a";"x"]) then pause,end
if or(part(["abc";"x"],[1,1])<>["aa";"xx"]) then pause,end
if or(part(["abc";"x"],[1,2])<>["ab";"x "]) then pause,end
if or(part(["abc";"x"],[1;1])<>["aa";"xx"]) then pause,end
if or(part(["abc";"x"],[1;2])<>["ab";"x "]) then pause,end
//===============================



a=part(["a","abc","abcd"],[1]);
assert_checkequal(a,["a","a","a"]);

a=part(["a","abc","abcd"],[1,1]);
assert_checkequal(a,["aa","aa","aa"]);

a=part(["a","abc","abcd"],[1,1,2]);
assert_checkequal(a,["aa ","aab","aab"]);

N = 10; a=part("-",ones(1:N));
assert_checkequal(a,"----------");

N = 6; pat = "- ";
a=part(pat, ones(1:N).*.(1:length(pat)));
assert_checkequal(a,"- - - - - - ");

a=part(["a string"  "another longer one"], $-2:$ );
assert_checkequal(a,["ing","one"]);

a=part("Hello world", $:-1:1);
assert_checkequal(a,"dlrow olleH");
a=part("Hello world", [ $ 5:6 ]);
assert_checkequal(a,"do ");

