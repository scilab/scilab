function [m,n]=checkdims(v)
// m=-1 row dim is formal
// m>=0 m is the matrix number of rows
// n=-1 col dim is formal
// n>=0 n is the matrix number of columns
r=0
[m,ierr]=evstr(v(3))
if ierr<>0 then m=-1,end
[n,ierr]=evstr(v(4))
if ierr<>0 then n=-1,end

