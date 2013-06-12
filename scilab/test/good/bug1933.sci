// Non-regression test file for bug 1933
// A.C
// Copyright INRIA 2006

mode(-1);


A = [0 0 2 2 0 0];
A(find(A==0)) = %nan;
B=log(A);

if ( isnan(B(1)) & isnan(B(2)) & isnan(B(5)) & isnan(B(6)) )  then
    affich_result(%T,1933);
else
    affich_result(%F,1933);
end


