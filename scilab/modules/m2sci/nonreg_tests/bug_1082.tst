// <-- Non-regression test for bug 1082 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=1082
//
// <-- Short Description -->
//    Error during conversion of a very simple file. Though the
//    syntax is the same in both languages, the converter fails to
//    convert the rounding of the field of a structure: "round(a.b)"

// Copyright INRIA
// Scilab Project - F.Belahcene

MFILECONTENTS=["structure.field = 2;" 
""
"data = round(structure.field);"]

MFILE=TMPDIR+"/bug1082.m"
SCIFILE=TMPDIR+"/bug1082.sci"

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
"structure.field = 2;";
"";
"data = round(mtlb_double(structure.field));"]

if or(SCIFILECONTENTSREF<>SCIFILECONTENTS) then pause,end
