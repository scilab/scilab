function W=pertrans(A)
//W=pertranspose(A)
// Copyright INRIA
[n,m]=size(A);
A=A';
W=A(m:-1:1,n:-1:1);
endfunction
