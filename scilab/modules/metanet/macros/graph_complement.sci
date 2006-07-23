function [g1]=graph_complement(g,gmax)
// Copyright INRIA
[lhs,rhs]=argn(0);
if rhs==1 then
  ind=0;
elseif rhs==2 then
  check_graph(gmax);
  ind=1;
else
  error(39)
end
check_graph(g);
n=g('node_number');m=prod(size(g('tail')));
ta=g('tail');he=g('head');
spg=sparse([ta' he'],ones(m,1),[n n]);
if ind==1 then
  // gmax given
  nma=gmax('node_number');mma=prod(size(gmax('tail')));
  if (nma <> n) then
    error('The two graphs must have the same number of nodes')
  end
  ama=gmax('tail');bma=gmax('head');
  spgma=sparse([ama' bma'],ones(mma,1),[nma nma]);
  spgma=spgma+spgma';
  spdif=spgma-spg-spg';
  [ir ic]=find(spdif=-1);
  if((ir <> [])|(ic <> [])) then
    error('First graph must be a subgraph of second graph')
  end
  spdif=tril(spdif,-1);
  [ij,v,mn]=spget(spdif);
else
  // gmax not given
  taa=[1:n].*.ones(n,1);hee=taa';
  taa=taa(:);hee=hee(:);
  ij=[taa hee];v=ones(n*n,1);
  spcomp=sparse(ij,v,[n n]);
  spdif=spcomp-spg-spg';
  spdif=tril(spdif,-1);
  [ij,v,mn]=spget(spdif);
end
//
g1=make_graph('foo',0,n,ij(:,1)',ij(:,2)');
g1('node_type')=g('node_type');
g1('node_x')=g('node_x');
g1('node_y')=g('node_y');
g1('node_color')=g('node_color');
g1('node_diam')=g('node_diam');
g1('node_border')=g('node_border');
g1('node_font_size')=g('node_font_size');
g1('node_demand')=g('node_demand');
endfunction
