function [ac,bc,u,ind]=canon(a,b)
//[ac,bc,u,ind]=canon(a,b)  gives the canonical controllable form
//of the pair (a,b).
//
//ind    controllability indices,
//ac,bc  canonical form
//u      current basis i.e. ac=inv(u)*a*u,bc=inv(u)*b
//
//See also : obsv_mat, cont_mat, ctr_gram, contrss
//Author:F. D. (Inria)
//!
//1: block-hessenberg form
// Copyright INRIA
// Was : [ac,bc,u,ro]=contr(a,b,[1.d-10*norm([a,b],1),1.d-10]);
[n,u,ro,V,ac,bc]=contr(a,b,1.d-10*norm([a,b],1));
//2:zeroing what is to the right of under-diagonal blocks
[na,ni]=size(b);[l,k]=size(ro);k0=na+1;k1=k0-ro(k);
for kk=k:-1:2,
  k2=k1-ro(kk-1);rows=k1:k0-1;cols=k1:na;intsc=k2:k1-1;
  i=eye(na,na);i(intsc,cols)=-ac(rows,intsc)\ac(rows,cols);
  im1=2*eye()-i;ac=im1*ac*i,bc=im1*bc;u=u*i;k0=k1;k1=k2;
end;
//3: compression of under-the-diagonal blocks
i=eye(na,na);n=1;m=ro(1)+1;
for kk=1:k-1,
   c=n:n+ro(kk)-1;z=ac(m:m+ro(kk+1)-1,c);
   [x,s,v]=svd(z);i(c,c)=v;n=n+ro(kk);m=m+ro(kk+1);
end;
ac=i'*ac*i,bc=i'*bc;u=u*i;
//4. normalization of blocks
j=eye(na,na);i=eye(na,na);k0=na+1;k1=k0-ro(k);
for kk=k:-1:2,
   k2=k1-ro(kk-1);rows=k1:k0-1;long=k2:k2+k0-k1-1;
   i=eye(na,na);j=eye(na,na);
   z=ac(rows,long);j(long,long)=z;
   i(long,long)=inv(z);
   ac=j*ac*i,bc=j*bc;u=u*i;k0=k1;k1=k2;
end;
// controllability indices
ind=0*ones(1,ni);[xx,mi]=size(ro);
for k=1:ni,for kk=1:mi,
   if ro(kk)>=k then ind(k)=ind(k)+1;end;
end;end
//final permutation:
v=ones(1,na);
for k=1:ind(1)-1,
       index=sum(ro(1:k))+1;v(k+1)=index;
end;
k0=1;kmin=ind(1)+1;
for kk=2:ni,
   numb=ind(kk);
   kmax=kmin+numb-1;
   v(kmin:kmax)=v(k0:k0+numb-1)+ones(1,ind(kk));
   k0=kmin;kmin=kmax+1;
end;
ac=ac(v,v),bc=bc(v,:),u=u(:,v);
endfunction
