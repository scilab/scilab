// Non-regression test file for bug 1003
// Copyright INRIA
// Scilab Project - V. Couvert

// Modified by Pierre MARECHAL
// Copyright INRIA
// Date : 18 Mar 2005

mode(-1);
clear;

MFILECONTENTS=["fp = fopen(''testfile.txt'',''r'');";
		"tempstr = '' '';";
		"while ( tempstr ~= -1)";
		"  tempstr = fgets(fp); % -1 if eof ";
		"  disp(tempstr);";
		"end";
		"fclose(fp);"]

MFILE=TMPDIR+"/bug1003.m"
SCIFILE=TMPDIR+"/bug1003.sci"

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
		"fp = mtlb_fopen(""testfile.txt"",""r"");";
		"tempstr = "" "";";
		"while asciimat(tempstr)~=(-1)";
		"  tempstr = mgetl(fp,1);  // -1 if eof ";
		"  disp(tempstr);";
		"end;";
		"mclose(fp);"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
	affich_result(%F,1003);
else
	affich_result(%T,1003);
end

clear
