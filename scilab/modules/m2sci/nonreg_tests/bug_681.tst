// <-- Non-regression test for bug 681 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=681
//
// <-- Short Description -->
//    m2sci doesn't translate "error" function.


// Copyright INRIA
// Scilab Project - V. Couvert

MFILECONTENTS="error(''This is an error message'')"

MFILE=TMPDIR+"/bug681.m"
SCIFILE=TMPDIR+"/bug681.sci"

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
"error(""This is an error message"")"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
