// Scilab Project - Serge Steer
// Copyright INRIA 2007
// Date : Sept 2007

mode(-1) ;
clear ;
BugNumber=2467
plot(1:10)
a=gca();a.log_flags='lnn';
path=TMPDIR+'/bug2467.scg'
xsave(path)
clf();
T=execstr("xload(path)",'errcatch')==0

affich_result(T,BugNumber) ;
clear ;
