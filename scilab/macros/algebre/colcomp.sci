function [w,rk]=colcomp(a,flag,tol)
//Syntaxes : [w,rk]=colcomp(a)
//           [w,rk]=colcomp(a,flag)
//           [w,rk]=colcomp(a,flag,tol)
//
//column compression of a i.e. comput. of ker(a)
//flag and tol are optional parameters
//flag='qr' or 'svd' (defaut 'svd')
//tol tolerance parameter (of order %eps as defaut value)
//the ma-rk first columns of w span the kernel of a when size(a)=(na,ma)
//F.D.
//!
// Copyright INRIA
  [ma,na]=size(a)
  [lhs,rhs]=argn(0)
  if a==[] then w=[];rk=0;return;end
  if norm(a,1) < sqrt(%eps)/10 then rk=0,w=eye(na,na),return,end
  if rhs ==2 then tol=sqrt(%eps)*norm(a,1)*maxi(ma,na),end
  if rhs==1 then flag='svd',tol=sqrt(%eps)*norm(a,1)*maxi(ma,na);end
  select flag
  case 'qr' then [q,r,rk,e]=qr(a',tol);
    //w=[q(:,rk+1:ma),q(:,1:rk)]; <-- le  ma me parait suspect je met na 
    w=q(:,na:-1:1)
  case 'svd' then [u,s,v,rk]=svd(a',tol);
    //w=[u(:,rk+1:na),u(:,1:rk)];
    w=u(:,na:-1:1)
  end
endfunction
