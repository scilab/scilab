function [n,nc,u,sl,v]=st_ility(sl,tol)
//stabilizability test
// Copyright INRIA
  [lhs,rhs]=argn(0)
if type(sl)==1 then
//[n,nc,u,A,B]=st_ility(A,B,tol)
if rhs==2 
sl=syslin('c',sl,tol,[]);rhs=1;
end
end
  [a,b,c,d,x0,dom]=sl(2:7);
  if dom==[] then 
    dom='c';warning('st_ility: time domain not given => sl assumed continuous!');
  end
  typ='c';if dom<>'c' then typ='d',end
  [na,na]=size(a);
  [nw,nb]=size(b);
  if nb==0 then
  b=zeros(na,1);
  end
  // controllable part
  if rhs==1 then 
    [n,u,ind,V,a,b]=contr(a,b);
  else 
    [n,u,ind,V,a,b]=contr(a,b,tol);
  end;
  if nb==0 then
  b=[];
  end;
  n=sum(n);nc=n;
  if lhs==4 then c=c*u;x0=u'*x0;end
  if n<>na then
    //order evals uncont. part
    nn=n+1:na;
    [v,n1]=schur(a(nn,nn),part(typ,1))
    n=n+n1
    //new realization
    if lhs>2 then
      u(:,nn)=u(:,nn)*v
      if lhs==4 then
	a(:,nn)=a(:,nn)*v;a(nn,nn)=v'*a(nn,nn)
	b(nn,:)=v'*b(nn,:)
	c(:,nn)=c(:,nn)*v
	x0(nn)=v'*x0(nn)
      end;
    end;
  end;
  //
  if lhs==4 then sl=syslin(dom,a,b,c,d,x0),end
  if lhs==5 then v=sl.B;sl=sl.A;end 
endfunction
