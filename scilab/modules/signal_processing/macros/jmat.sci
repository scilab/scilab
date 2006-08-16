function [j]=jmat(n,m)
//j=jmat(n,m)
//This macro permutes block rows or block columns of a matrix
//
//   n   : number of block rows or block columns of the matrix
//   m   : size of the (square) blocks
//!
// Copyright INRIA
j=[];
for k=1:n,j(k,n-k+1)=1;end;
j=j.*.eye(m,m);
endfunction
