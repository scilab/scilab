// Non-regression test file for bug 1812
// Copyright INRIA
// Scilab Project - F. Belahcene

mode(-1);
clear;

MFILECONTENTS=[
"m=3; a=5; b=2; c=3; d=6";
" ";
"if a==0 && b==2"; 
"m=2";
"end";
" ";
"if a==0 && b==2 && c==3 && d==4";
"m=2";
"end";
" ";
"if a==0 && b==2"; 
"m=2";
"end";
" ";
"if a==0 || b==2 || c==3 || d==4";
"m=2";
"end"
]

MFILE=TMPDIR+"/bug1812.m"
SCIFILE=TMPDIR+"/bug1812.sci"

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

mfile2sci(MFILE,TMPDIR);

fd=mopen(SCIFILE,"r");
SCIFILECONTENTS=mgetl(fd,-1);
mclose(fd);

SCIFILECONTENTSREF=["";
"// Display mode";
"mode(0);";
"";
"// Display warning for floating point exception";
"ieee(1);";
"";
"m = 3;a = 5;b = 2;c = 3;d = 6";
"";
"%v02 = bool2s(%f);if bool2s(a==0) then %v02 = bool2s(b==2);end;";
"if %v02 then";
"  m = 2";
"end;";
"";
"%v02 = bool2s(%f);if bool2s(a==0) then %v02 = bool2s(b==2);end;%v12 = bool2s(%f);if %v02 then %v12 = bool2s(c==3);end;";
"if %v12&bool2s(d==4) then";
"  m = 2";
"end;";
"";
"%v02 = bool2s(%f);if bool2s(a==0) then %v02 = bool2s(b==2);end;";
"if %v02 then";
"  m = 2";
"end;";
"";
"%v02 = bool2s(%t);if ~bool2s(a==0) then %v02 = bool2s(b==2);end;%v12 = bool2s(%t);if ~%v02 then %v12 = bool2s(c==3);end;";
"if %v12|bool2s(d==4) then";
"  m = 2";
"end;";
]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,1812);

clear

