// Non-regression test file for bug 853
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["dir_name=''path'';";
"dir_target=dir(dir_name);";
"for i=1:10";
"  RC(i)=[dir_name,''\'',dir_target(i).name];";
"end"]

MFILE=TMPDIR+"/bug853.m"
SCIFILE=TMPDIR+"/bug853.sci"

mputl(MFILECONTENTS,MFILE);
mfile2sci(MFILE,TMPDIR);
SCIFILECONTENTS=mgetl(SCIFILE);

SCIFILECONTENTSREF=["";
		"// Display mode";
		"mode(0);";
		"";
		"// Display warning for floating point exception";
		"ieee(1);";
		"";
		"dir_name = ""path"";";
		"dir_target = mtlb_dir(dir_name);";
		"for i = 1:10";
		"  RC(1,i) = dir_name+""\""+dir_target(i).name;";
		"end;"]


if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,853);
else
	affich_result(%T,853);
end

clear
