function [g]=make_graph(name,directed,n,tail,head)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>5 then error(39), end
// name
if type(name)<>10|prod(size(name))<>1 then
  error('""name"" must be a string')
end
// directed
if directed<>1&directed<>0 then
  error('""directed"" must be 0 or 1')
end
// node_number

if prod(size(n))<>1|n<0
  error('""n"" must be positive')
end
// tail
s=size(tail)
if s(1)<>1 then
  error('""tail"" must be a row vector')
end
ma=s(2)
// head
s=size(head)
if s(1)<>1 then
  error('""head"" must be a row vector')
end
if s(2)<>ma then
  error('""tail"" and ""head"" must have identical sizes')
end
if n<>0 then
  // tail and head
  if min(min(tail),min(head))<1|max(max(tail),max(head))>n then
    error('""tail"" and ""head"" do not represent a graph')
  end
  g=glist(name,directed,n,tail,head,string(1:n),..
      [],[],[],[],[],[],[],[],[],[],..
      [],[],[],[],[],[],[],[],[],[],..
      [],[],[],[],[],[],[])
else
  ij=[tail' head'];
  n2=size(ij,1);
  tri=[ij(:,1)' ij(:,2)'];[atri,indic]=sort(-tri);
  atri=-atri;
  n=size(atri,2);
  atri1=atri-[0 atri(1:n-1)];ij1=find(atri1<>0);ij2=find(atri1==0);
  uu=[1:n];nij2=size(ij2,2);vv2=[1:nij2];uu(ij2)=uu(ij2)-vv2;
  nij1=size(ij1,2);vv1=[1:nij1];uu(ij1)=vv1;
  w=0*ones(1:n);w(indic)=uu;
  ta=w(1:(n2));he=w((n2)+1:n);
  g=glist(name,directed,nij1,ta,he,string(atri(ij1)),..
      [],[],[],[],[],[],[],[],[],[],..
      [],[],[],[],[],[],[],[],[],[],..
      [],[],[],[],[],[],[])
end
endfunction
