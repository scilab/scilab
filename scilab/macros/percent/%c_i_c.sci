function s=%c_i_c(i,j,s1,s2)
// see %sip.sci
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==3 then 
  if type(i)==4 then s=s1;s(find(i))=j,return,end
  if size(i)==[-1 -1] then s=[],return,end
  s=s1
  mn=prod(size(s))
  i1=[]
  for ii=1:mn
    if ~or(ii==i) then  i1=[i1 ii] ,end
  end
  s=s(i1)
else
  if type(i)==4|type(j)==4 then
    if type(i)==4 then i=find(i),end
    if type(j)==4 then j=find(j),end
    s=s2
    s(i,j)=s1
    return
  end
  row=%f
  col=%f
  s=s2
  [m,n]=size(s)
  if and(size(i)==[-1 -1]) then
    row=%t
  else
    if and(i(:)==(1:m)') then row=%t,end
  end
  if and(size(j)==[-1 -1]) then
    col=%t
  else
    if and(j(:)==(1:n)') then col=%t,end
  end
  if ~row&~col then error('inserting [] in submatrix --> forbidden'),end
  if row&col then s=[],return,end
  if row then
    j1=[]
    for jj=1:n
      if ~or(jj==j) then  j1=[j1 jj] ,end
    end
    s=s(:,j1)
  else
    i1=[]
    for ii=1:m
      if ~or(ii==i) then  i1=[i1 ii] ,end
    end
    s=s(i1,:)
  end
end
