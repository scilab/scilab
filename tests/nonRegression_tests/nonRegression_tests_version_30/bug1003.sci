// Non-regression test file for bug 1003
// Copyright INRIA
// Scilab Project - V. Couvert

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
"fp = mtlb_fopen(""testfile.txt"",""r"");";
"tempstr = "" "";";
"while asciimat(tempstr)~=(-1)";
"  tempstr = mgetl(fp,1);  // -1 if eof ";
"  disp(tempstr);";
"end";
"mclose(fp);"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then
  correct=%F
end

affich_result(correct,1003);

clear
