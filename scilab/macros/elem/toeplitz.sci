function a=toeplitz(c,r)
//a=toeplitz(c,r)  returns the Toepliz matrix whose first row
//is r and first column is c .( r(1) = c(1) is assumed).
//
//r and  c can be constant, polynomial or character string matrices.
//!
// Copyright INRIA
[lhs,rhs]=argn(0),
if rhs==1 then r=c(:);end
nr=size(r,'*');r=matrix(r,1,nr)
nc=size(c,'*');c=matrix(c,nc,1)
if nr*nc==0 then a=[],return,end
if r(1)<>c(1) then error('conflicting  r(1) and c(1)'),end
a(nc,nr)=r(1);k=mini([nr,nc]);
for l=1:k,a(l,l:nr)=r(1:nr-l+1),a(l:nc,l)=c(1:nc-l+1),end
endfunction
