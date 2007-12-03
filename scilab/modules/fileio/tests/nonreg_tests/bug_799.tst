// <-- Non-regression test for bug 799 -->
//
// <-- Bugzilla URL -->
// http://www.scilab.org/cgi-bin/bugzilla_bug_II/show_bug.cgi?id=799
//
// <-- Short Description -->
//    Reload large matrix file from disk with "fscanfMat" command 
//    crashes Scilab.

// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 6 fevrier 2005

stacksize(19e6);

m = 10593;
n = 696;
a = rand(m,n,'u');

fprintfMat(TMPDIR+'/Mat',a,'%5.2f');
a1=fscanfMat(TMPDIR+'/Mat');

if a1(1,1) <> 0.21  then pause,end
