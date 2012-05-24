// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Vincent COUVERT <vincent.couvert@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 2341 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2341
//
// <-- Short Description -->
//    unhandled matlab syntax for lhs (Matlab accepts lhs calling sequence without commas

MFILE=TMPDIR+"/bug2341.m";
SCIFILE=TMPDIR+"/bug2341.sci";

// TEST1: no commas
MFILECONTENTS=["function [a b c d]=bug2341";
"a=1;b=2;c=3;d=4"];

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
if ierr<>0 then pause,end

// TEST2: commas OK without spaces
MFILECONTENTS=["function [a,b,c,d]=bug2341";
"a=1;b=2;c=3;d=4"];

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
if ierr<>0 then pause,end

// TEST3: commas OK with spaces before
MFILECONTENTS=["function [a  ,b ,c   ,d]=bug2341";
"a=1;b=2;c=3;d=4"];

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch");
if ierr<>0 then pause,end

// TEST4: commas OK with spaces after
MFILECONTENTS=["function [a, b,  c,  d]=bug2341";
"a=1;b=2;c=3;d=4"];

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch");
if ierr<>0 then pause,end

// TEST5: commas OK with spaces after and before
MFILECONTENTS=["function [a  , b,  c ,d]=bug2341";
"a=1;b=2;c=3;d=4"];

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch");
if ierr<>0 then pause,end

// TEST6: commas OK or KO
MFILECONTENTS=["function [a   b,  c   d]=bug2341";
"a=1;b=2;c=3;d=4"];

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch");
if ierr<>0 then pause,end

// TEST7: one output
MFILECONTENTS=["function [a]=bug2341";
"a=1;"];

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch");
if ierr<>0 then pause,end

// TEST8: one output with blanks
MFILECONTENTS=["function [ a ]=bug2341";
"a=1;"];

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch");
if ierr<>0 then pause,end
