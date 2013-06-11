// Non-regression test file for bug 1620
// Copyright INRIA
// Scilab Project - Pierre MARECHAL
// Copyright INRIA 2006
// Date : 9 janvier 2006

mode(-1);
clear

a=rand(500,148)
fprintfMat(TMPDIR+"/bug1620.txt",a);
b=fscanfMat(TMPDIR+"/bug1620.txt");

if or(size(b)<>[500 148]) then
    affich_result(%F,1620);
else
    affich_result(%T,1620);
end

clear
