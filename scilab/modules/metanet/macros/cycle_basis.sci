function [spc]=cycle_basis(g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>1 then error(39), end
// finds a cycle basis in a simple connected undirected graph
if (g('directed') == 1) then
  error('The graph must be undirected')
end
ii=is_connex(g);
if (ii <> 1) then
  error('The graph must be connected')
end
n=g('node_number');m=prod(size(g('tail')));
if ( n < 3) then
  error('Not enough nodes in the graph to have a cycle')
end
nu=m-n+1;
ta=g('tail');he=g('head');
spt=sparse([ta' he'],[1:m],[n n]);
spt=spt+spt';
t=min_weight_tree(g);
tat=ta(t);het=he(t);
prev=1000000*ones(1,n);
tag=[tat het];heg=[het tat];
//
ta1=ta;he1=he;
ta1(t)=[];he1(t)=[];
if (ta1 == []) then
  error('No cycle in the graph')
end 
bac=[];Dir=[];
spc=sparse([],[],[nu m]);
t=[0 t];
for i=1:nu,
  cycle=[];
  i1=ta1(i);i2=he1(i);
  bac=[];Dir=full(spt(i1,i2));
  while ((i1)<>1)
    iedge=t(i1);
    bac=[iedge bac];i1=ta(iedge)+he(iedge)-i1;
  end
  while ((i2)<>1)
    iedge=t(i2);
    Dir=[iedge Dir];i2=ta(iedge)+he(iedge)-i2;
  end
  itron=[];jmax=min(size(bac,2),size(Dir,2));
  for j=1:jmax,
    if(bac(j)==Dir(j)), itron=[itron j];end;
  end;
  bac(itron)=[];Dir(itron)=[];
  cycle=[Dir bac($:-1:1)];
  ncy=size(cycle,2);
  spc(i,1:ncy)=cycle;
end
endfunction
