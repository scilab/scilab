function [w,rk]=rowcomp(A,flag,tol)
//Row compression of A <--> computation of im(A)
//flag and tol are optional parameters
//flag='qr' or 'svd' (default 'svd')
//tol tolerance parameter (sqrt(%eps)*norm(A,1) as default value)
//the rk first (top) rows of w span the row range of a
//the rk first columns of w' span the image of a
//F.D. (1987)
//!
// Copyright INRIA
  if A==[] then w=[];rk=0;return;end
  
  [ma,na]=size(A)
  rhs=argn(2)

  if norm(A,1) < sqrt(%eps)/10 then rk=0,w=eye(ma,ma),return;end
  
  select rhs
  case 1 then// default values for flag and tol
    flag='svd',tol=sqrt(%eps)*norm(A,1);
  case 2 then //default value for tol
    tol=sqrt(%eps)*norm(A,1)
  else
    if size(tol,'*')>1|~isreal(tol)|tol<0 then
      error('rowcomp: third argument should be a real non negative s"+...
	    " calar')
    end
  end
  select flag
  case 'qr' then 
    [q,r,rk,e]=qr(A,tol);w=q';
  case 'svd' then 
    [u,s,v,rk]=svd(A,tol);w=u' ;
  else
    error('rowcomp: second argument should be ''qr'' or ''svd''')
  end
endfunction


