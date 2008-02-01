function [g1]=subgraph(v,ind,g)
// Copyright INRIA
[lhs,rhs]=argn(0)
if rhs<>3 then error(39), end
// check v
s=size(v)
if s(1)<>1 then
  error('First argument must be a row vector')
end
// check ind
if ((ind<>'nodes')&(ind<>'edges')) then
  error('Second argument must be ""nodes"" or ""edges""')
end
// check g
check_graph(g)
// subgraph given by edges or nodes
n=g('node_number'); 
a=g('tail');b=g('head');
ne=prod(size(g('tail')));noe=[1:ne];
if ind=='edges' then
  // case of edges
  if (min(v)<1|max(v)>ne) then
    error('A number in first argument is not an edge number')
  end
  noe(v)=[];
  adel=a(noe);bdel=b(noe);ndel=[adel bdel];
  vv=-sort(-ndel);
  w=vv(2:$)-vv(1:($-1));
  [ir,ic]=find(w==0);
  vv(ic)=[];
  n1=size(a(v),2);
  tri=[a(v) b(v)];[atri,indic]=sort(tri);nn=size(atri,2);
  atri1=atri-[0 atri(1:nn-1)];ij1=find(atri1<>0);ij2=find(atri1==0);
  uu=[1:nn];nij2=size(ij2,2);vv2=[1:nij2];uu(ij2)=uu(ij2)-vv2;
  nij1=size(ij1,2);vv1=[1:nij1];uu(ij1)=vv1;
  w=0*ones(1:nn);w(indic)=uu;
  ta=w(1:(n1));he=w((n1)+1:nn);
  g1=make_graph('foo',g('directed'),nij1,ta,he);
  dd=[1:nij1];dd(ta)=a(v);dd(he)=b(v);
else
  // case of nodes
  [atri,indic]=sort(v);nn=size(atri,2);
  if (v(1)<1|v(nn)>n) then
     error('A number in first argument is not a node number')
   end
  atri1=atri-[0 atri(1:nn-1)];ij2=find(atri1==0);
  if(ij2 <> []) then
    error('Duplicated node number in first argument')
  end
  nnew=size(v,2);
  v=atri($:-1:1);
  vglo=zeros(1,n);vglo(v)=[1:nnew];
  indic=[]
  for i=1:nnew, 
    ij1=find(a==v(i));
    indic=[indic ij1];
  end
  hhe=b(indic);
  indic1=[]
  for i=1:nnew, 
    ij1=find(hhe==v(i));
    indic1=[indic1 indic(ij1)];
  end
  dd=v;v=indic1;
  ta=a(indic1);he=b(indic1);
  ta=vglo(ta);he=vglo(he);
  g1=make_graph('foo',g('directed'),nnew,ta,he);
end
//
a=g('node_name');if a<>[] then g1('node_name')=a(dd);end
a=g('node_type');if a<>[] then g1('node_type')=a(dd);end
a=g('node_x');if a<>[] then g1('node_x')=a(dd);end
a=g('node_y');if a<>[] then g1('node_y')=a(dd);end
a=g('node_color');if a<>[] then g1('node_color')=a(dd);end
a=g('node_diam');if a<>[] then g1('node_diam')=a(dd);end
a=g('node_border');if a<>[] then g1('node_border')=a(dd);end
a=g('node_font_size');if a<>[] then g1('node_font_size')=a(dd);end
a=g('node_demand');if a<>[] then g1('node_demand')=a(dd);end
a=g('node_label');if a<>[] then g1('node_label')=a(dd);end
//
a=g('edge_name');if a<>[] then g1('edge_name')=a(v);end
a=g('edge_color');if a<>[] then g1('edge_color')=a(v);end
a=g('edge_width');if a<>[] then g1('edge_width')=a(v);end
a=g('edge_hi_width');if a<>[] then g1('edge_hi_width')=a(v);end
a=g('edge_font_size');if a<>[] then g1('edge_font_size')=a(v);end
a=g('edge_length');if a<>[] then g1('edge_length')=a(v);end
a=g('edge_cost');if a<>[] then g1('edge_cost')=a(v);end
a=g('edge_min_cap');if a<>[] then g1('edge_min_cap')=a(v);end
a=g('edge_max_cap');if a<>[] then g1('edge_max_cap')=a(v);end
a=g('edge_q_weight');if a<>[] then g1('edge_q_weight')=a(v);end
a=g('edge_q_orig');if a<>[] then g1('edge_q_orig')=a(v);end
a=g('edge_weight');if a<>[] then g1('edge_weight')=a(v);end
a=g('edge_label');if a<>[] then g1('edge_label')=a(v);end
endfunction
