// Non-regression test file for bug 1662
// Copyright INRIA
// Scilab Project - F. Belahcene


mode(-1);
clear;

MFILECONTENTS=["function  y=bug1662(x)";
"switch x % comment1";
"% comment2";
"case x>0 then y=''positive''";
"case x<0 then y=''negative''";
"else y=''null''";
"end"]

MFILE=TMPDIR+"/bug1662.m"
SCIFILE=TMPDIR+"/bug1662.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR,%f,%t);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["function [y] = bug1662(x)";
""
"// Ouput variables initialisation (not found in input variables)";
"y=[];";
""
"// Display mode";
"mode(0);";
""
"// Display warning for floating point exception";
"ieee(1);";
""
"select x // comment1";
" // comment2";
"  case mtlb_logic(x,"">"",0) then y = ""positive""";
"  case mtlb_logic(x,""<"",0) then y = ""negative""";
"  else y = ""null""";
"end;";
"endfunction"]


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
    affich_result(%F,1662);
else
    affich_result(%T,1662);
end