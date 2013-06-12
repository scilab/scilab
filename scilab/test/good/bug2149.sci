// Non-regression test file for bug 2149
// Copyright INRIA
// Scilab Project - Allan CORNET
// Copyright INRIA 2006

mode(-1);
clear

correctItem=0;

M=[1+%i;2+3*%i] ;
try
    fprintfMat("d:/pipo.txt",M,"%f",["comment un";"comment 2"])
    correct=%T;
catch
    correctItem=correctItem+1;
end

M=[1.2,2.3];
try
    fprintfMat("d:/pipo.txt",M,"%f",["comment un";"comment 2"])
catch
    correctItem=correctItem+1;
end

try
    fprintfMat("d:/pipo.txt","M","%f",["comment un";"comment 2"])
    correct=%T;
catch
    correctItem=correctItem+1;
end

if (correctItem == 2) then
    affich_result(%T,2149);
else
    affich_result(%F,2149);
end


clear
