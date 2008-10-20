function [p,C,b,ci,cs,mi]=mps2linpro(mps)
[lhs,rhs]=argn(0)
if type(mps)==10 then //a file name
  mps=readmps(mps,[-1000 1000])
end
m=size(mps('rownames'),1)
n=size(mps('colnames'),2)
kobj=mps('irobj')
rowstat=mps('rowstat')

keq=find(rowstat==1)
kge=find(rowstat==2)
kle=find(rowstat==3)
C=full(adj2sp(mps('colpnts'),mps('rownmbs'),mps('acoeff'),[m,n]))
p=C(kobj,:)'

C(kge,:)=-C(kge,:)
C=C([keq,kle,kge],:) // modif [keq,kle,kge] in place of [keq;kle;kge]
                     // this is because find return row index vector
b=mps('rhs');
b(kge,:)=-b(kge,:)
b=b([keq,kle,kge],:) // same modif

ci=mps('bounds')(:,1)
cs=mps('bounds')(:,2)
mi=size(keq,'*')
if lhs==1 then
  p=tlist(['linpro','p','C','b','ci','cs','mi'],p,C,b,ci,cs,mi)
end
endfunction
