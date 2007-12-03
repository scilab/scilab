// <-- Non-regression test for bug 1971 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1971
//
// <-- Short Description -->
//    the mtlb_fprintf does not work. It should be re-written to follow the Matlab doc
//    of fprintf

// Copyright INRIA
// Scilab Project - F. Belahcene

MFILECONTENTS=[
"fprintf(''qsdfsdcvwqghcv'')";
]

MFILE=TMPDIR+"/bug1971.m"
SCIFILE=TMPDIR+"/bug1971.sci"

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
"// L.1: No simple equivalent, so mtlb_fprintf() is called";
"mtlb_fprintf(""qsdfsdcvwqghcv"")";
]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end

instr = 'mtlb_fprintf(""qsdfsdcvwqghcv"")';
ierr  = execstr(instr,'errcatch');
if ierr<>0  then pause,end
