function [g1]=delete_arcs(ij,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
// checking ij
if (ij==[]) then g1=g; return end;
if (type(ij)<>1|size(ij,2) <> 2) then
  error('First argument must be a n x 2 scalar vector')
end;
// deleting the edges from nodes i to nodes j
g1=g;ma=prod(size(g('tail')));n=g('node_number');
ta=g('tail');he=g('head');
ic=[];
//
if(g('directed') == 1) then
  for jj=1:size(ij,1),
    i=ij(jj,1);j=ij(jj,2);
    if ((i < 1)|(i > n)) then
      error(string(i)+' is not a node number')
    end;
    if ((j < 1)|(j > n)) then
      error(string(j)+' is not a node number')
    end;
    ic1=find((ta==i)&(he==j));ic=[ic ic1];
  end;
else
  for jj=1:size(ij,1),
    i=ij(jj,1);j=ij(jj,2);
    if ((i < 1)|(i > n)) then
      error(string(i)+' is not a node number')
    end;
    if ((j < 1)|(j > n)) then
      error(string(j)+' is not a node number')
    end;
    ic1=find((ta==i)&(he==j));ic=[ic ic1];
    ic1=find((ta==j)&(he==i));ic=[ic ic1];    
  end;
end;
//
ii=[1:ma];ii(ic)=[];
ta(ic)=[];
if ta==[] then
  error('Cannot delete, a graph must have at least one edge')
end;
g1('tail')=ta;
a=g('head');g1('head')=a(ii);
a=g('edge_name');g1('edge_name')=a(ii);
a=g('edge_color');g1('edge_color')=a(ii);
a=g('edge_width');g1('edge_width')=a(ii);
a=g('edge_hi_width');g1('edge_hi_width')=a(ii);
a=g('edge_font_size');g1('edge_font_size')=a(ii);
a=g('edge_length');g1('edge_length')=a(ii);
a=g('edge_cost');g1('edge_cost')=a(ii);
a=g('edge_min_cap');g1('edge_min_cap')=a(ii);
a=g('edge_max_cap');g1('edge_max_cap')=a(ii);
a=g('edge_q_weight');g1('edge_q_weight')=a(ii);
a=g('edge_q_orig');g1('edge_q_orig')=a(ii);
a=g('edge_weight');g1('edge_weight')=a(ii);
a=g('edge_label');g1('edge_label')=a(ii);
