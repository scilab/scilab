// Non-regression test file for bug 924
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear all;

MFILECONTENTS=["function bug924";
"a=''milou'';";
"a=''milou'',";
"a=''milou''";
"a;";
"a,";
"a";
"[c,d]=svd(1);";
"[c,d]=svd(1),";
"[c,d]=svd(1)";
"b=a;";
"b=a,";
"b=a"]

MFILE=TMPDIR+"/bug924.m"
SCIFILE=TMPDIR+"/bug924.sci"

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

mfile2sci(MFILE,TMPDIR);

fd=mopen(SCIFILE,"r");
SCIFILECONTENTS=mgetl(fd,-1);
mclose(fd);

SCIFILECONTENTSREF=["function [] = bug924()";
"";
"// Display mode";
"mode(0);";
"";
"// Display warning for floating point exception";
"ieee(1);";
"";
"a = ""milou"";";
"a = ""milou"",";
"a = ""milou""";
"a;";
"a,";
"a";
"[c,d] = svd(1);";
"[c,d] = svd(1),";
"[c,d] = svd(1)";
"b = a;";
"b = a,";
"b = a";
"endfunction"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,924);

clear all
