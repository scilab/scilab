function [iperm,mrepi,profil,ierr]=bandwr(a,b,c,d)
//case 1 : a is a sparse matrix and b is (option) iopt
//case 2 : a is lp, b is ln, c is n the dimension and d is (option) iopt
[lhs,rhs]=argn(0)
if (rhs>4|rhs<1) then error(39), end;
if (rhs<3) then
  if (rhs == 1) then iopt=0;end;
  if (rhs == 2) then iopt=b;end;
  [ij,v,mn]=spget(a);
  aa=sparse(ij,ones(v),mn);
  if (sum(aa-aa') <> 0) then   
    error('The matrix ""a"" must to be symmetric')
  end
  n=mn(1);
  if (size(diag(a),1) < n) then   
    error('There are null terms on the diagonal')
  end
//
amem=a;
aa=a-speye(a);aa=aa+aa';
mn=size(aa);
u=sum(aa,'r');rownul=find(u==0);
if(rownul <> []) then
ini=rownul;
if(max(rownul) == mn(1)) then ini($)=ini($)-1;end;
pea=sparse([ini' ini'+1], ones(ini'),mn);
a=a+pea+pea';a=triu(a);
[ij,v,mn]=spget(a);
end
// 
  [lp,la,ln]=m6ta2lpd(ij(:,1)',ij(:,2)',n+1,n);
else
  lp=a;ln=b;n=c;
  if (rhs == 3) then iopt=0;end;
  if (rhs == 4) then iopt=d;end;
  v=ones(ln)';n=size(lp,2)-1;
  [ta,he]=m6lp2tad(lp,la,ln,n);
  amem=sparse([ta' he'],v,[n,n]);
end;
if(n < 3) then 
  error('Size too small')
end
if(iopt <> 0) then iopt=1;end;
nz=size(v,'*');
liwork=2*n+2+2*nz+6*n+3+3*n;
iwork=zeros(1,liwork);
lrwork=n*n+1;
// max for lrwork; can be chosen smaller : k*nz with k=10 e.g.;
rwork=zeros(1,lrwork);
iwork(1:(n+1))=lp;
iwork((2*n+2):(2*n+2+nz-1))=ln;
rwork(1:nz)=v';
[iperm,mrepi,profil,ierr]=m6bandred(n,nz,liwork,iwork,lrwork,rwork,iopt);
if(ierr == 0) then
if(iopt == 0) then
[ij,v,mn]=spget(amem(mrepi,mrepi));
pr=max(abs(ij(:,1)-ij(:,2)));
profil(1)=pr;end;
if(iopt==1) then
[ij,v,mn]=spget(amem(mrepi,mrepi));
idia=find(ij(:,1) == ij(:,2));
ind1=idia(1:($-1))+1;
qq=ij(ind1,1)-ij(ind1,2);qq=[0;qq];
profil=[1:mn(1)]'-qq;
end;
return;
else
//
// 3 random sorts
for iisort=1:3;
b=[];a=[1:n];
for i=1:n,k=n+1-i;
ii=int((rand()+1./k)*k);
b=[b a(ii)];a(ii)=[];
end;
[ss,sk]=sort(b);
back=sk($:-1:1);
a=amem+amem';
a=a(b,b);a=triu(a);
aa=a-speye(a);aa=aa+aa';
mn=size(aa);
u=sum(aa,'r');rownul=find(u==0);
[ij,v,mn]=spget(a);
if(rownul <> []) then
ini=rownul;
if(max(rownul) == mn(1)) then ini($)=ini($)-1;end;
pea=sparse([ini' ini'+1], ones(ini'),mn);
a=a+pea+pea';a=triu(a);
[ij,v,mn]=spget(a);
end;
[lp,la,ln]=m6ta2lpd(ij(:,1)',ij(:,2)',n+1,n);
v=ones(ln)';nz=size(v,'*');
liwork=2*n+2+2*nz+6*n+3+3*n;
iwork=zeros(1,liwork);
lrwork=n*n+1;
rwork=zeros(1,lrwork);
iwork(1:(n+1))=lp;
iwork((2*n+2):(2*n+2+nz-1))=ln;
rwork(1:nz)=v';
[iperm,mrepi,profil,ierr]=m6bandred(n,nz,liwork,iwork,lrwork,rwork,iopt);
if(ierr == 0) then 
mrepi=b(mrepi);
iperm=iperm(back);
if(iopt == 0) then
[ij,v,mn]=spget(amem(mrepi,mrepi));
pr=max(abs(ij(:,1)-ij(:,2)));
profil(1)=pr;end;
if(iopt==1) then
[ij,v,mn]=spget(amem(mrepi,mrepi));
idia=find(ij(:,1) == ij(:,2));
ind1=idia(1:($-1))+1;
qq=ij(ind1,1)-ij(ind1,2);qq=[0;qq];
profil=[1:mn(1)]'-qq;
end;
return;end;
end;
if(ierr == 64000) then
    error('program terminated by error');
return;end;
end;
endfunction
