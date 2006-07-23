function [g1]=delete_nodes(v,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>2 then error(39), end
// checking v
if v==[] then g=g1; return; end;
s=size(v)
if s(1)<>1 then
  error('First argument must be a row vector')
end
// deleting the nodes 
n=g('node_number'); 
g1=g;ma=prod(size(g('tail')));
ta=g('tail');he=g('head');
im=prod(size(v));
noe=[1:ma];ic1=[];
[atri,indic]=sort(v);nn=size(atri,2);
if (v(1)<1|v(nn)>n) then
  error('A number in first argument is not a node number')
end
atri1=atri-[0 atri(1:nn-1)];ij2=find(atri1==0);
if(ij2 <> []) then
  error('Duplicated node number in first argument')
end
for jj=1:im,
  i=v(jj);
  [ir,ic]=find((ta==i)|(he==i));
  ic1=[ic1 ic];
end;
ta(ic1)=[];he(ic1)=[];noe(ic1)=[];
if noe==[] then
  error('Cannot delete, a graph must have at least one edge')
end;
v=sort(v);
for jj=1:im,
  i=v(jj);
  [ir,ic]=find(ta>i);
  if ic <> [] then 
    ta(ic)=ta(ic)-1;
  end;
  [ir,ic]=find(he>i);
  if ic <> [] then 
    he(ic)=he(ic)-1;
  end;
end;
g1=make_graph('foo',g('directed'),(n-im),ta,he);
a=g('node_type');a(v)=[];g1('node_type')=a;
a=g('node_name');a(v)=[];g1('node_name')=a;
a=g('node_x');a(v)=[];g1('node_x')=a;
a=g('node_y');a(v)=[];g1('node_y')=a;
a=g('node_color');a(v)=[];g1('node_color')=a;
a=g('node_diam');a(v)=[];g1('node_diam')=a;
a=g('node_border');a(v)=[];g1('node_border')=a;
a=g('node_font_size');a(v)=[];g1('node_font_size')=a;
a=g('node_demand');a(v)=[];g1('node_demand')=a;
a=g('node_label');a(v)=[];g1('node_label')=a;
a=g('edge_name');if a<>[] then g1('edge_name')=a(noe);end
a=g('edge_color');if a<>[] then g1('edge_color')=a(noe);end
a=g('edge_width');if a<>[] then g1('edge_width')=a(noe);end
a=g('edge_hi_width');if a<>[] then g1('edge_hi_width')=a(noe);end
a=g('edge_font_size');if a<>[] then g1('edge_font_size')=a(noe);end
a=g('edge_length');if a<>[] then g1('edge_length')=a(noe);end
a=g('edge_cost');if a<>[] then g1('edge_cost')=a(noe);end
a=g('edge_min_cap');if a<>[] then g1('edge_min_cap')=a(noe);end
a=g('edge_max_cap');if a<>[] then g1('edge_max_cap')=a(noe);end
a=g('edge_q_weight');if a<>[] then g1('edge_q_weight')=a(noe);end
a=g('edge_q_orig');if a<>[] then g1('edge_q_orig')=a(noe);end
a=g('edge_weight');if a<>[] then g1('edge_weight')=a(noe);end
a=g('edge_label');if a<>[] then g1('edge_label')=a(noe);end
endfunction
