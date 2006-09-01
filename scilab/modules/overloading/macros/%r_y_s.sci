function r=%r_y_s(a,b)
// a./.b
// Copyright INRIA
[ma,na]=size(a)
[mb,nb]=size(b)
r=zeros(ma*mb,na*nb)
k=0
for j=1:na
  l=0
  for i=1:ma
    r(l+(1:mb),k+(1:nb))=a(i,j)./b
    l=l+mb
  end
  k=k+nb
end
endfunction
