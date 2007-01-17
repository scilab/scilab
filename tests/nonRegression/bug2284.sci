// Non-regression test file for bug 2284
// Copyright INRIA
// Scilab Project - S. Steer
// Copyright INRIA
// Date : 17 Janv 2007

mode(-1);
clear;
s1=-9.999999999;
s2=0.999999999999;

print(TMPDIR+'/bug2284',s1,s2)
txt=mgetl(TMPDIR+'/bug2284')
r=and(txt== [" s2  =";" ";"    1.  ";" s1  =";" ";"  -10.  "])
mdelete(TMPDIR+'/bug2284')

affich_result(r,2284);

clear;
