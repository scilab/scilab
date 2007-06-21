// Non-regression test file for bug 2428
// Copyright INRIA
// Scilab Project - Vincent COUVERT

//mode(-1);
clear;

MFILE=TMPDIR+"/bug2428.m"
SCIFILE=TMPDIR+"/bug2428.sci"

correct=%T;

MFILECONTENTS=["function bug2428";
    "global gui_settings";
    "i = 1;";
    "a = gui_settings.pn_mem{i}"]

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr=execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch")
correct=correct&ierr==0;

//affich_result(correct,2428);
disp(correct);
clear


