function a=sprand(m,n,density,typ)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<4 then typ='uniform',end

if type(typ)<>10 then
  error('typ argument of sprand must be ''uniform'' or ''normal''')
end
if part(typ,1)<>'u'&part(typ,1)<>'n' then
  error('typ argument of sprand must be ''uniform'' or ''normal''')
end 
if m*n==0 then a=[],end
density=maxi(mini(density,1),0)
// compute the index of non zeros entries
if density==0 then a=sparse([],[],[m,n]),return,end
nel=m*n*density
mdist=1/density

ij=int(1+2*(mdist-1)*rand(1,1.2*nel,'uniform'))
// sum the increments
ij=rtitr(1,poly([-1 1],'z','c'),ij);
nel1=size(ij,'*')
if nel1<=1 then a=sparse([],[],[m,n]),return,end
k=find(ij>m*n)
if k<>[] then nel1=k(1)-1,end
if nel1<=1 then a=sparse([],[],[m,n]),return,end
ij=matrix(ij(2:nel1),nel1-1,1);
j=int((ij-1)/m);
i=ij-j*m;
j=j+1;
a=sparse([i j],rand(nel1-1,1,typ),[m,n]);

