function c=mtlb_cell(n1,n2,n3,n4,n5,n6,n7,n8,n9,n10,n11,n12,n13,n14)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then
  c=tlist('cell',n1)
else
  n(1,rhs)=0
  c=tlist('cell',n)
  for k=1:rhs
    execstr('n(k)=n'+string(k))
  end
  c(2)=n
end
for k=1:prod(c(2))
  c($+1)=[]
end

