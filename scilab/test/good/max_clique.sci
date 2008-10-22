function [nsize,nodes]=max_clique(g,ind)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs>2|rhs<1 then 
  error(39)
elseif rhs==1 then
  ind=0
end;
// check g
check_graph(g)
// check ind
if (ind<>0&ind<>1) then
  error('The second argument must be 0 or 1')
end
m=prod(size(g('tail')))
n=g('node_number')
head=g('head'); tail=g('tail');
if (ind==1) then
  tta=[tail head];hhe=[head tail];
  xm=sparse([tta' hhe'],ones(tta)',[n,n]);
  [ij,v,mn]=spget(xm);
  [lp,la,ln]=adj_lists(1,n,ij(:,1)',ij(:,2)');
  m2=2*m;np1=n+1;nwk=10*m;
  [wcl]=m6clique1(n,m,m2,np1,nwk,lp,ln);
  ii=find(wcl == 0);
  if (ii<>[]) then wcl(ii)=[]; end;
  nodes=wcl;nsize=size(wcl,2);
else
  xm=sparse([tail' head'],ones(tail'),[n,n]);
  xadj=full(xm+xm');
  mn=m*n;
  [clmax,clnod,bestn]=m6clique(n,m,mn,xadj);
  nsize=clmax;
  ii=find(bestn==0);
  if (ii<>[]) then bestn(ii)=[]; end;
  nodes=clnod(bestn);
end;
endfunction
