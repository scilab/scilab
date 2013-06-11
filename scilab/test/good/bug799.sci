// Non-regression test file for bug 799
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2005
// Date : 6 fevrier 2005

mode(-1);
clear

stacksize(19e6);
m=10593;
n=696;
a=rand(m,n,"u");
fprintfMat(TMPDIR+"/Mat",a,"%5.2f");
a1=fscanfMat(TMPDIR+"/Mat");

if( a1(1,1) == 0.21 ) then
    affich_result(%T,799);
else
    affich_result(%F,799);
end

clear