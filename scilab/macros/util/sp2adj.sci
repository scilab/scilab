function [lp,ls,v]=sp2adj(A)
// A =  n x m sparse matrix
// lp, ls, v = adjacency representation of A i.e:
// lp(j+1)-lp(j) = # of non zero entries in row j
// ls = column index of the non zeros entries 
// in row 1, row 2,..., row n.
// v = values of non zero entries 
// in row 1, row 2,..., row n.
// lp is a (column) vector of size n+1
// ls is an integer (column) vector of size nnz(A).
// v is a real vector of size nnz(A).
[ij,v,n]=spget(A');
N=n(1);
if ij == [] then,
lp=ones(n(2)+1,1);ls=[];v=[];
else,
[lp,la,ls]=m6ta2lpd(ij(:,1)',ij(:,2)',N+1,N)
lp=lp(:);ls=ls(:);
end;
