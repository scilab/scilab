// Non-regression test file for bug 853
// Copyright INRIA
// Scilab Project - V. Couvert

mode(-1);
clear;

MFILECONTENTS=["dir_name=''path'';";
"dir_target=dir(dir_name);";
"for i=1:10";
"  RC(i)=[dir_name,''\'',dir_target(i).name];";
"end"]

MFILE=TMPDIR+"/bug853.m"
SCIFILE=TMPDIR+"/bug853.sci"

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
"dir_name = ""path"";";
"dir_target = mtlb_dir(dir_name);";
"RC = [];";
"for i = 1:10";
"  RC(1,i) = dir_name+""\""+dir_target(i).name;";
"end"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,853);

clear
