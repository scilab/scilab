// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004      - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1003 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1003
//
// <-- Short Description -->
//    Matlab fgets translation is wrong
//
//    The Matlab function fgets is close to mgetl but not
//    identical.
//    fgets(fd) should be translated into mgetl(fd,1).
//    The current CVS translates fgets(fd) into mgetl(fd) but
//    this is wrong since fgets(fd) in Matlab reads one line of
//    the file while mgetl(fd) in Scilab reads the full file (if
//    I understand the Matlab and Scilab docs correctly).
//
//    The solution I propose to correct this consists simply in
//    adding:
//      tree.rhs(2)=Cste(1)
//    in the else part of sci_fgets.sci
//
//
//    Example of wrong conversion: Try to convert the following
//    file ct1.m:
//
//    function ct1()
// ...

MFILECONTENTS=["% Test with fgets result stored in a variable";
"fp = fopen(''testfile.txt'',''r'');";
"tempstr = '' '';";
"while ( tempstr ~= -1)";
"  tempstr = fgets(fp); % -1 if eof";
"  disp(tempstr);";
"end";
"fclose(fp);"
"% Test with fgets result not-stored in a variable";
"fp = fopen(''testfile.txt'',''r'');";
"disp(fgets(fp)); % -1 if eof";
"fclose(fp);"];

MFILE=TMPDIR+"/bug1003.m";
SCIFILE=TMPDIR+"/bug1003.sci";

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
"// Test with fgets result stored in a variable";
"fp = mtlb_fopen(""testfile.txt"",""r"");";
"tempstr = "" "";";
"while tempstr<>-1";
"  tempstr = mgetl(fp,1);  if isempty(tempstr) then tempstr = -1;end;  // -1 if eof";
"  disp(tempstr);";
"end;";
"mclose(fp);";
"// Test with fgets result not-stored in a variable";
"fp = mtlb_fopen(""testfile.txt"",""r"");";
"%v0 = mgetl(fp,1);if isempty(%v0) then %v0 = -1;end;disp(%v0);// -1 if eof";
"mclose(fp);"];

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
