function [fe,che,fn,chn]=chain_struct(lp,la,ln)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs==1 then
  // g
  check_graph(lp)
  [lp,la,ln]=adj_lists(lp)
elseif rhs<>3 then error(39), 
end
// lp
s=size(lp)
if s(1)<>1 then
  error('First argument must be a row vector')
end
// la
s=size(la)
if s(1)<>1 then
  error('Second argument must be a row vector')
end
// ln
s=size(ln)
if s(1)<>1 then
  error('Third argument must be a row vector')
end
// from lp,ln,la to chained structure of edges and nodes
n=size(lp,2);lpm=lp(1:(n-1));
m=size(la,2);la1=[la 0];ln1=[ln 0];
mp1=m+1;lp1=lp;lpM=mp1*ones(lpm);
ii=find((lp(2:n)-lp(1:(n-1)))==0);
fe=la1(lpm);la2=la1;fe(ii)=zeros(ii);fe1=fe;fe1(ii)=mp1*ones(ii);
fn=ln1(lpm);ln2=ln1;fn(ii)=zeros(ii);fn1=fn;fn1(ii)=mp1*ones(ii);
la2(lp1)=zeros(lp1);ln2(lp1)=zeros(lp1);
che=zeros(1,mp1);chn=zeros(1,mp1);
lp2=min(lpm+1,lpM);
u=la2(lp2);un=ln2(lp2);
la2(lp2)=zeros(lp2);ln2(lp2)=zeros(lp2);
che(fe1)=u;chn(fe1)=un;
//loop
uumem=u;
i=2; 
while i<>m 
  lpm2=min(lpm+i,lpM);
  uu=la2(lpm2);uun=ln2(lpm2);
  la2(lpm2)=zeros(lpm2);ln2(lpm2)=zeros(lpm2);
  ii=find(uu<>0);if ii==[] then i=m;else
  che(uumem(ii))=uu(ii);chn(uumem(ii))=uun(ii);
  uumem=uu;i=i+1;end;
end
che=che(1:m);chn=chn(1:m);
endfunction
