// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid BELAHCENE
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1812 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1812
//
// <-- Short Description -->
//    error in converting matlab || as && and other translator doubts

MFILECONTENTS=[
"m=3; a=5; b=2; c=3; d=6";
"";
"if a==0 & b==2"; 
"  m=2";
"end";
"";
"if a==0 && b==2"; 
"  m=2";
"end";
"";
"if a==0 && b==2 && c==3 && d==4";
"  m=2";
"end";
"";
"if a==0 | b==2"; 
"  m=2";
"end";
"";
"if a==0 || b==2"; 
"  m=2";
"end";
"";
"if a==0 || b==2 || c==3 || d==4";
"  m=2";
"end"
];

MFILE=TMPDIR+"/bug1812.m";
SCIFILE=TMPDIR+"/bug1812.sci";

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
"if a==0 & b==2 then";
"  m = 2";
"end;";
"";
"%v02 = %f;if a==0 then %v02 = b==2;end;";
"if %v02 then";
"  m = 2";
"end;";
"";
"%v02 = %f;if a==0 then %v02 = b==2;end;%v12 = %f;if %v02 then %v12 = c==3;end;";
"if %v12 & d==4 then";
"  m = 2";
"end;";
"";
"if a==0 | b==2 then";
"  m = 2";
"end;";
"";
"%v02 = %t;if ~a==0 then %v02 = b==2;end;";
"if %v02 then";
"  m = 2";
"end;";
"";
"%v02 = %t;if ~a==0 then %v02 = b==2;end;%v12 = %t;if ~%v02 then %v12 = c==3;end;";
"if %v12 | d==4 then";
"  m = 2";
"end;";
];

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
