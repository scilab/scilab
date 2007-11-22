// <-- Non-regression test for bug 2428 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=2428
//
// <-- Short Description -->
//    I would like to convert to SciLab the package TBMLab 3.1 by Smets 
//    (http://iridia.ulb.ac.be/~psmets/TBMLAB.zip). I tried both via the menu 
//    Applications -> m2sci and trying to convert recursively the directory 
//    of the package and via the SciPad menu File -> Import Matlab File. In 
//    the first case the conversion fails without errors, in the second case 
//    an error appears and a message suggests to report here the error.

// Copyright INRIA
// Scilab Project - Vincent COUVERT

MFILE         = TMPDIR+"/bug2428.m"
SCIFILE       = TMPDIR+"/bug2428.sci"
MFILECONTENTS = ["function bug2428";
    "global gui_settings";
    "i = 1;";
    "a = gui_settings.pn_mem{i}"]

fd=mopen(MFILE,"w");
mputl(MFILECONTENTS,fd);
mclose(fd);

ierr = execstr("mfile2sci("""+MFILE+""","""+TMPDIR+""")","errcatch");
if ierr<>0 then pause,end
