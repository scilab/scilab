function [h,num,den]=ss2tf(sl,rmax)
// State-space to transfer function.
// Syntax:
//   h=ss2tf(sl) 
//   h=ss2tf(sl,'b')
//   h=ss2tf(sl,rmax)
//
//   sl   : linear system (syslin list)
//   h    : transfer matrix 
//   rmax : optional parameter controlling the conditioning in
//          block diagonalization method is used (If 'b' is entered
//          a default value is used)
//   Method: By default, one uses characteristic polynomial and 
//   det(A+Eij)=det(A)+C(i,j) where C is the adjugate matrix of A
//   Other method used : block-diagonalization (generally 
//   this gives a less accurate result).
//
//!
// Copyright INRIA
if type(sl)==1|type(sl)==2 then
  h=sl
  return
end
flag=sl(1);
if (type(sl)<>16)|flag(1)<>'lss' then
  error('First argument must be in state-space form')
end
if sl(3)==[] then h=sl(5);num=sl(5);den=eye(sl(5));return;end
if sl(4)==[] then h=sl(5);num=sl(5);den=eye(sl(5));return;end
if size(sl(2),'*')==0 then
  h=sl(5)
  return
end 
//1
domaine=sl(7)
if type(domaine)==1 then var='z';end
if domaine=='c' then var='s';end;
if domaine=='d' then var='z';end;
if domaine==[] then
   var='s'; 
   if type(sl(5))==2 then var=varn(sl(5));end
end
//
[lhs,rhs]=argn(0);
meth='p'
if rhs==2 then
  if type(rmax)==10 then  
    meth=part(rmax,1),
  else 
    meth='b'
  end
end
//
select meth
case 'b' // 
a=sl(2)
[n1,n1]=size(a)
z=poly(0,var);
if rhs==1 then
  [a,x,bs]=bdiag(a)
else
  [a,x,bs]=bdiag(a,rmax)
end
k=1;m=[];v=ones(1,n1);den=1;
for n=bs';k1=k:k-1+n;
// leverrier
             h=z*eye(n,n)-a(k1,k1)
             f=eye(n,n)
             for kl=1:n-1,
                b=h*f,
                d=-sum(diag(b))/kl,
                f=b+eye()*d,
             end;
             d=sum(diag(h*f))/n
//
          den=den*d;
          l=[1:k-1,k+n:n1] ,
          if l<>[] then v(l)=v(l)*d,end
          m=[m,x(:,k1)*f];
          k=k+n;
end;
if lhs==3 then h=sl(5),num=sl(4)*m*diag(v)*(x\sl(3));return;end
m=sl(4)*m*diag(v)*(x \ sl(3))+sl(5)*den;
[m1,n1]=size(m);[m,den]=simp(m,den*ones(m1,n1))
h=syslin(domaine,m,den)
case 'p' then
  Den=real(poly(sl(2),var))
  na=degree(Den);den=[];
  [m,n]=size(sl(5))
  c=sl(4)
  for l=1:m
    [m,i]=maxi(abs(c(l,:)));
    if m<>0 then
      ci=c(l,i)
      t=eye(na,na)*ci;t(i,:)=[-c(l,1:i-1), 1, -c(l,i+1:na)]
      al=sl(2)*t;
      t=eye(na,na)/ci;t(i,:)=[c(l,1:i-1)/ci, 1, c(l,i+1:na)/ci]
      al=t*al;ai=al(:,i),
      b=t*sl(3)
      for k=1:n
        al(:,i)=ai+b(:,k);
	[nlk,dlk]=simp(real(poly(al,var)),Den)
	den(l,k)=dlk;
        num(l,k)=-(nlk-dlk)*ci
      end
    else
      num(l,1:n)=0*ones(1,n);
      den(l,1:n)=ones(1,n);
    end
  end
  if lhs==3 then h=sl(5);return;end
 w=num./den+sl(5);
 if type(w)==1 then h=w;return;end   //degenerate case
 h=syslin(domaine,w);
end
