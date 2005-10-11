function [w,rk]=rowcomp(A,flag,tol)
//Row compression of A <--> comput. of im(A)
//flag and tol are optional parameters
//flag='qr' or 'svd' (default 'svd')
//tol tolerance parameter (of order %eps as default value)
//the rk first (top) rows of w span the row range of a
//the rk first columns of w' span the image of a
//F.D. (1987)
//!
// Copyright INRIA
  [ma,na]=size(A)
  [lhs,rhs]=argn(0)
  if A==[] then w=[];rk=0;return;end
  if norm(A,1) < sqrt(%eps)/10 then rk=0,w=eye(ma,ma),return;end
  if rhs ==2 then tol=sqrt(%eps)*norm(A,1),end
  if rhs==1 then flag='svd',tol=sqrt(%eps)*norm(A,1);end
  select flag
  case 'qr' then [q,r,rk,e]=qr(A,tol);w=q';
  case 'svd' then [u,s,v,rk]=svd(A,tol);w=u' ;
  end
endfunction


