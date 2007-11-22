// <-- Non-regression test for bug 2341 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2341
//
// <-- Short Description -->
//    unhandled matlab syntax for lhs (Matlab accepts lhs calling sequence without commas


// Non-regression test file for bug 2341
// Copyright INRIA
// Scilab Project - Vincent COUVERT

//mode(-1);
clear;

MFILE=TMPDIR+"/bug2341.m"
SCIFILE=TMPDIR+"/bug2341.sci"

correct=%T;

// TEST1: no commas
MFILECONTENTS=["function [a b c d]=bug2341";
"a=1;b=2;c=3;d=4"]

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
correct=correct&ierr==0;

// TEST2: commas OK without spaces
MFILECONTENTS=["function [a,b,c,d]=bug2341";
"a=1;b=2;c=3;d=4"]

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
correct=correct&ierr==0;

// TEST3: commas OK with spaces before
MFILECONTENTS=["function [a  ,b ,c   ,d]=bug2341";
"a=1;b=2;c=3;d=4"]

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
correct=correct&ierr==0;

// TEST4: commas OK with spaces after
MFILECONTENTS=["function [a, b,  c,  d]=bug2341";
"a=1;b=2;c=3;d=4"]

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
correct=correct&ierr==0;

// TEST5: commas OK with spaces after and before
MFILECONTENTS=["function [a  , b,  c ,d]=bug2341";
"a=1;b=2;c=3;d=4"]

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
correct=correct&ierr==0;

// TEST6: commas OK or KO
MFILECONTENTS=["function [a   b,  c   d]=bug2341";
"a=1;b=2;c=3;d=4"]

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
correct=correct&ierr==0;

// TEST7: one output
MFILECONTENTS=["function [a]=bug2341";
"a=1;"]

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
correct=correct&ierr==0;

// TEST8: one output with blanks
MFILECONTENTS=["function [ a ]=bug2341";
"a=1;"]

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
correct=correct&ierr==0;

affich_result(correct,2341);

clear


