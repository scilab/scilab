// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005 - INRIA - Farid BELAHCENE
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- ENGLISH IMPOSED -->

// <-- Non-regression test for bug 1149 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1149
//
// <-- Short Description -->
//    m2sci doesn't cope with constructs like a=[1,2,3,] or
//    a=[,1,2,3,] (superfluous leading/trailing comma), which
//    matlab lives well with. Awful matlab quirk, I agree, but
//    some automatic m-file generator does it carelessly.

MFILECONTENTS="a=[,1,2,3,];"

MFILE=TMPDIR+"/bug1149.m"
SCIFILE=TMPDIR+"/bug1149.sci"

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
"a = [1,2,3];"]

correct=%T
if or(SCIFILECONTENTSREF<>SCIFILECONTENTS)  then pause,end
