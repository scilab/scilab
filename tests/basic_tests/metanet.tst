// Copyright INRIA
// add_edge
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g=add_edge(1,7,g)

// add_node
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
n=g('node_number');
g1=add_node(g,[270 140])

// adj_lists
ta=[2 3 3 5 3 4 4 5 8];
he=[1 2 4 2 6 6 7 7 4];
g=make_graph('foo',1,8,ta,he);
[lp,la,ln]=adj_lists(1,g('node_number'),ta,he)

// arc_graph
ta=[1 1 2 4 4 5 6 7 2 3 5 1];
he=[2 6 3 6 7 8 8 8 4 7 3 5];
g=make_graph('foo',1,8,ta,he);
g1=arc_graph(g)

// arc_number
ta=[1 1 2 4 4 5 6 7 2 3 5 1];
he=[2 6 3 6 7 8 8 8 4 7 3 5];
g=make_graph('foo',1,8,ta,he);
arc_number(g)

// articul
ta=[2  1 3 2 2 4 4 5 6 7 8 8 9 10 10 10 10 11 12 13 14 15 16 17 17];
he=[1 10 2 5 7 3 2 4 5 8 6 9 7  7 11 13 15 12 13 14 11 16 17 14 15];
g=make_graph('foo',1,17,ta,he);
nart = articul(g)

// bandwr
ta=[2  1 3 2 2 4 4 5 6 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[1 10 2 5 7 3 2 4 5 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',0,17,ta,he);
n=g('node_number');
[lp,la,ln] = adj_lists(1,n,g('tail'),g('head'));
aa=graph_2_mat(g);
ij=[g('tail')' g('head')']; v=1*ones(g('tail')');
vv=sparse(ij,v,[n n]);
ww=tril(vv+vv')';ww=ww+eye();
ww1=full(ww);
[iperm,mrepi,profil,ierr]=bandwr(ww)
g2=g;g2('node_name')=string(iperm);
yy=0*ones(n,n);
for i=1:n, for j=1:n,
yy(i,j)=ww1(mrepi(i),mrepi(j));end;end;
[ij,v,mn]=spget(ww);
g1=make_graph('foo',0,n,ij(:,1)',ij(:,2)');
[lp,la,ln] = adj_lists(1,n,g1('tail'),g1('head'));
[iperm,mrepi,profil,ierr]=bandwr(lp,ln,n,0)

// best_match
ta=[27 27 3 12 11 12 27 26 26 25 25 24 23 23 21 22 21 20 19 18 18];
ta=[ta  16 15 15 14 12 9 10 6 9 17 8 17 10 20 11 23 23 12 18 28]; 
he=[ 1  2 2  4  5 11 13  1 25 22 24 22 22 19 13 13 14 16 16  9 16];
he=[he  10 10 11 12  2 6  5 5 7  8 7  9  6 11  4 18 13  3 28 17];
n=28;
g=make_graph('foo',0,n,ta,he);
[card,match] = best_match(g)

// chain_struct
ta=[1 1 2 3 5 4 6 7 7 3 3 8 8 5];
he=[2 3 5 4 6 6 7 4 3 2 8 1 7 4];
g=make_graph('foo',1,8,ta,he);
[lp,la,ln]=adj_lists(1,g('node_number'),ta,he);
[fe,che,fn,chn] = chain_struct(lp,la,ln)

// check_graph
ta=[1 1 2 3 5 4 6 7 7 3 3 8 8 5];
he=[2 3 5 4 6 6 7 4 3 2 8 1 7 4];
g=make_graph('foo',1,8,ta,he);
check_graph(g)

// circuit
ta=[1 1 2 3 5 4 6 7 7 3 3 8 8 5];
he=[2 3 5 4 6 6 7 4 3 2 8 1 7 4];
g=make_graph('foo',1,8,ta,he);
p=circuit(g)
g=make_graph('foo',1,4,[1 2 2 3],[2 3 4 4]);
[p,r]=circuit(g)

// con_nodes
ta=[1 1 2 2 2 3 4 4 5 7 7 9 10 12 12 13 13 14 15];
he=[2 6 3 4 5 1 3 5 1 8 9 8 11 10 11 11 15 13 14];
g=make_graph('foo',1,15,ta,he);
con_nodes(2,g)

// connex
ta=[1 1 2 2 2 3 4 4 5 6 7 7 7 8 9 10 12 12 13 13 14 15];
he=[2 6 3 4 5 1 3 5 1 7 5 8 9 5 8 11 10 11 11 15 13 14];
g=make_graph('foo',1,15,ta,he);
[nc,ncomp]=connex(g)

// contract_edge
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g1=contract_edge(10,13,g)

// convex_hull
xx=[46 120 207 286 366 453 543 544 473 387 300 206 136 250 346 408];
g('node_x')=[xx 527 443 306 326 196 139 264  55  58  46 118 513];
yy=[36  34  37  40  38  40  35 102 102  98  93  96 167 172 101 179];
g('node_y')=[yy 198 252 183 148 172 256 259 258 167 109 104 253];
xy=[g('node_x');g('node_y')];
[nhull,ind] = convex_hull(xy)

// cycle_basis
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
gt=make_graph('foo',1,17,ta,he);
g=graph_simp(gt);
spc=cycle_basis(g)

// delete_arcs
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
ij=[10 13;8 6;5 4;4 2];
gt=delete_arcs(ij,g)

// delete_nodes
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
v=[10 13 4];
gt=delete_nodes(v,g)

// edge_number
ta=[1 1 2 4 4 5 6 7 2 3 5 1];
he=[2 6 3 6 7 8 8 8 4 7 3 5];
g=make_graph('foo',1,8,ta,he);
edge_number(g)

// find_path
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
p=find_path(1,14,g)

// gen_net
v=[1,10,2,1,0,10,100,100,0,100,50,50];
g=gen_net('foo',1,v)

// girth
ta=[1 6 2 4 7 5 6 8 4 3 5 1];
he=[2 1 3 6 4 8 8 7 2 7 3 5];
g=make_graph('foo',1,8,ta,he);
d=girth(g)

// glist
ta=[1  1 2 7 8 9 10 10 10 10 11 12 13 13];
he=[2 10 7 8 9 7  7 11 13 13 12 13  9 10];
g=glist('foo',1,13,ta,he,[],[],[],[],[],[],[],[],[],[],[],..
      [],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[],[])

// graph_2_mat
g=load_graph(SCI+'/demos/metanet/colored');
a=graph_2_mat(g)

// graph_center
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',0,17,ta,he);
[no,rad] = graph_center(g)

// graph_complement
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 11 12 13 13 13 14 15 17 17 16 16];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 15 12 13 9 10 14 11 16 14 15 1 17];
g=make_graph('foo',1,17,ta,he);
g1=graph_complement(g)

// graph_diameter
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',0,17,ta,he);
[d,p] = graph_diameter(g)

// graph_power
ta=[1 1 2 4 4 5 6 7 2 3 5 1];
he=[2 6 3 6 7 8 8 8 4 7 3 5];
g=make_graph('foo',1,8,ta,he);
g1=graph_power(g,2)

// graph_simp
ta=[1 1  1 2 2 2 3 4 4 4 5 5 6 7 7 8 8 9 9 10 10 10 10 10 11 12 12 13 13 13 14 15 16 16 17 17];
he=[1 2 10 3 5 7 4 2 9 9 4 6 6 8 2 6 9 7 4  7 11 13 13 15 12 11 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g1=graph_simp(g)

// graph_sum
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
ta=[2 3 4 5 11 12 1];
he=[10 5 6 7 15 17 7];
g1=make_graph('foo',1,17,ta,he);
g2=graph_sum(g,g1)

// graph_union
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
v=[7 8 9 10 11 12 13];
g1=subgraph(v,'nodes',g);
v=[1 2 5 6 7 8 9 10];
g2=subgraph(v,'nodes',g);
g=graph_union(g1,g2)

// hamilton
ta=[2  1 3 2 2 4 4 5 6 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[1 10 2 5 7 3 2 4 5 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
cir=hamilton(g)

// is_connex
g=make_graph('foo',1,3,[1,2,3,1],[2,3,1,3]);
is_connex(g)
g=make_graph('foo',1,4,[1,2,3,1],[2,3,1,3]);
is_connex(g)

// knapsack
weight=ones(1,15).*.[1:4];
profit=ones(1,60);
capa=[15 45 30 60];
[earn,ind]=knapsack(profit,weight,capa)

// line_graph
ta=[1 1 2 4 4 5 6 7 2 3 5 1];
he=[2 6 3 6 7 8 8 8 4 7 3 5];
g=make_graph('foo',0,8,ta,he);
g1=line_graph(g)

// load_graph
g=load_graph(SCI+'/demos/metanet/colored')

// make_graph
g=make_graph('foo',1,3,[1,2,3,1],[2,3,1,3])
ta=[1  1 2 7 8 9 10 10 10 10 11 12 13 13];
he=[2 10 7 8 9 7  7 11 13 13 12 13  9 10];
g=make_graph('foo',1,0,ta,he)

// mat_2_graph
g=load_graph(SCI+'/demos/metanet/colored');
a=graph_2_mat(g);
g1=mat_2_graph(a,1)

// max_cap_path
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
ma=edge_number(g);
g('edge_max_cap')=[8 17 5 10 15 15 18 15 19 6 13 15 16 8 13 8 8 8 19 15 9 19 8 10 10 9 14];
[p,cap]=max_cap_path(1,14,g)

// max_flow
ta=[1 1 2 2 3 3 4 4 5 5 5 5 6 6 6 7 7 15 15 15 15 15 15];
ta=[ta, 15 8 9 10 11 12 13 14];
he=[10 13 9 14 8 11 9 11 8 10 12 13 8 9 12 8 11 1 2 3 4];
he=[he, 5 6 7 16 16 16 16 16 16 16];
n=16;
g=make_graph('foo',1,n,ta,he);
ma=edge_number(g);
g('edge_max_cap')=ones(1,ma);
g('edge_min_cap')=zeros(1,ma);
source=15; sink=16;
nodetype=0*ones(1,n); nodetype(source)=2; nodetype(sink)=1;
g('node_type')=nodetype;
nodecolor=0*ones(1,n); nodecolor(source)=11; nodecolor(sink)=11;
g('node_color')=nodecolor;
[v,phi,ierr]=max_flow(source,sink,g)

// max_clique
ta=[1 2 3 4 5 6 6 7 8  9 10 16 16 10 11 11 12 12 11 14 15 15 13 7 13 13];
he=[2 3 4 5 6 7 8 8 9 10 16  2  3 11 12 13  1 14 14 15  5  9 12 4 14 15];
g=make_graph('foo',0,16,ta,he);
[ns,no] = max_clique(g)
g1=graph_complement(g);
[ns,no] = max_clique(g1)

// mesh2d
theta=0.025*[1:40]*2.*%pi;
x=1+cos(theta);
y=1.+sin(theta+0.5);
theta=0.05*[1:20]*2.*%pi;
x1=1.3+0.4*cos(theta);
y1=1.+0.4*sin(theta+.2);
theta=0.1*[1:10]*2.*%pi;
x2=0.5+0.2*cos(theta);
y2=1.+0.2*sin(theta-0.1);
x=[x x1 x2];
y=[y y1 y2];
nu=mesh2d(x,y)

fr1=[[1:40] 1];fr2=[[41:60] 41];fr2=fr2($:-1:1);
fr3=[[61:70] 61];fr3=fr3($:-1:1);
front=[fr1 fr2 fr3];
xx=0.1*[1:20];
yy=xx.*.ones(1,20);
zz= ones(1,20).*.xx;
x3=yy;y3=zz;
wai=((x3-1).*(x3-1)+(y3-1).*(y3-1));
ii=find(wai >= .94);
x3(ii)=[];y3(ii)=[];
wai=((x3-0.5).*(x3-0.5)+(y3-1).*(y3-1));
ii=find(wai <= 0.055);
x3(ii)=[];y3(ii)=[];
wai=((x3-1.3).*(x3-1.3)+(y3-1).*(y3-1));
ii=find(wai <= 0.21);
x3(ii)=[];y3(ii)=[];
xnew=[x x3];ynew=[y y3];
nu=mesh2d(xnew,ynew,front)

// min_lcost_cflow
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1];
g=make_graph('foo',1,15,ta,he);
g1=g;ma=arc_number(g1);
g1('edge_min_cap')=0*ones(1,ma);
g1('edge_max_cap')=[15 8 16 6 9 7 19 4 14 7 17 10 12 6 9 10 9 14 6 17 14 8 8 15 17 18 16 8 16];
g1('edge_cost')=[10.7 3.6 8.4 9.3 7.6 10.7 4.9 6.8 2.3 6.0 5.5 3.0 5.0 1.9 2.6 6.1 7.4 2.3 9.2 10.8 3.2 4.4 8.9 2.8 3.9 8.2 4.0 9.8 2.1];
cv=5;
[c,phi,v,flag]=min_lcost_cflow(15,1,cv,g1)

// min_lcost_flow1
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10 1 8];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1 12 14];
g=make_graph('foo',1,15,ta,he);
g1=g;ma=arc_number(g1);
g1('edge_min_cap')=[17 11 10 5 2 5 16 3 20 15 11 4 6 5 5 3 2 8 0 4 14 1 11 14 13 17 3 3 20 2 17];
g1('edge_max_cap')=[37 36 37 25 26 42 35 23 56 52 43 40 42 26 42 33 24 27 34 23 45 29 49 48 45 42 25 24 56 34 45];
g1('edge_cost')=[8 7 3 2 6 10 6 3 5 10 7 11 2 8 1 2 2 4 8 4 4 4 6 8 9 7 5 11 8 2 8];
[c,phi,flag]=min_lcost_flow1(g1)

// min_lcost_flow2
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10 1 8];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1 12 14];
g=make_graph('foo',1,15,ta,he);
g1=g;ma=arc_number(g1);
g1('edge_min_cap')=0.*ones(1,ma);
n=g1('node_number');
g1('edge_max_cap')=[32 19 15 33 32 27 32 32 27 27 34 21 18 33 24 32 15 18 19 30 34 24 25 22 17 29 15 27 15 27 29];
g1('edge_cost')=[2 11 5 2 5 7 8 8 4 5 2 4 2 7 10 2 10 8 8 4 4 6 5 5 7 8 7 6 7 6 5];
dd=[14 -20 -4 8 -3 16 5 -23 0 7 -5 16 4 -24 9];
g1('node_demand')=dd;
[c,phi,flag]=min_lcost_flow2(g1)

// min_qcost_flow
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10 1 8];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1 12 14];
g=make_graph('foo',1,15,ta,he);
g1=g;ma=arc_number(g1);
g1('edge_min_cap')=[3 4 3 0 2 4 1 4 2 0 1 1 4 4 4 1 3 4 1 3 0 3 0 3 1 1 0 2 0 3 5];
g1('edge_max_cap')=[19 9 15 6 16 16 20 22 12 16 13 16 23 16 14 7 15 21 4 15 8 24 2 10 4 4 20 14 11 18 9];
g1('edge_q_orig')=0*ones(1,ma);
g1('edge_q_weight')=ones(1,ma);
[c,phi,flag]=min_qcost_flow(0.2,g1)

// min_weight_tree
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
t=min_weight_tree(1,g)

// neighbors
ta=[1  6  2  4  7  5  6  8  4  3  5  1];
he=[2  1  3  6  4  8  8  7  2  7  3  5];
g=make_graph('foo',1,8,ta,he);
a=neighbors(6,g)

// node_number
ta=[1 1 2 4 4 5 6 7 2 3 5 1];
he=[2 6 3 6 7 8 8 8 4 7 3 5];
g=make_graph('foo',1,8,ta,he);
node_number(g)

// nodes_2_path
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
ns=[1 10 15 16 17 14 11 12 13 9 7 8 6];
p=nodes_2_path(ns,g)

// nodes_degrees
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
[outdegree,indegree]=nodes_degrees(g)

// path_2_nodes
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
p=[2 16 23 25 26 22 17 18 19 13 10 11];
ns=path_2_nodes(p,g)

// perfect_match
ta=[27 27 3 12 11 12 27 26 26 25 25 24 23 23 21 22 21 20 19 18 18];
ta=[ta  16 15 15 14 12 9 10 6 9 17 8 17 10 20 11 23 23 12 18 28]; 
he=[ 1  2 2  4  5 11 13  1 25 22 24 22 22 19 13 13 14 16 16  9 16];
he=[he  10 10 11 12  2 6  5 5 7  8 7  9  6 11  4 18 13  3 28 17];
n=28;
g=make_graph('foo',0,n,ta,he);
arcost=[21 76  0 33 67 63 85 69 88  7 56 66 73 20 54 23 23 22 88 65 31 93..
21 31 36 29 57 48 33 59 50 44 27 63 41 92  4 48 26 41 28 13 78 21 11 69 15..
70 84 41 41 88 11 20 56 59 69 89 50 35 39 92 95 34 38 73 26 50 26 53 54 12..
23 63 76  5 67 20 39 83 59 48 10 11 88];
[cst,nmatch] = perfect_match(g,arcost)

// predecessors
ta=[1  6  2  4  7  5  6  8  4  3  5 1];
he=[2  1  3  6  4  8  8  7  2  7  3 5];
g=make_graph('foo',1,8,ta,he);
a=predecessors(8,g)

// qassign
n=15;
d=[..
21.1 23.2 50.2 68.6 38.7  4.9 64.9 12.5  2.6 45.0 42.6  1.2 69.1 84.7 12.0;..
75.6 23.1 43.7 15.3 92.2 67.2 99.2 72.8 51.7 72.3 24.6 18.7 76.6 78.8 82.9;..
 0.0 21.6 26.9 69.7 94.9 20.2  5.0 26.8 39.2 89.8 92.3 49.2 35.7 12.6 31.6;..
33.0 88.3 63.3 84.2 34.4 39.1 74.9 54.7 24.1 24.3 10.0 74.9 76.9 78.8 53.1;..
66.5 65.3 40.5 40.6 37.6 83.0 41.0 98.9 50.6 43.4 46.8 94.1 54.8 34.5 57.2;..
62.8 30.8 91.8 40.9 73.4 58.8 60.8 74.0 42.4 96.8 39.5 21.2  9.6 26.6  4.8;..
85.0 93.3  4.4 87.8 26.2 48.3 85.4  0.4 28.9 50.7  3.7 58.0 95.6 97.1 82.5;..
68.6 21.5 48.2 11.4 49.9 22.3  6.4 59.0  8.9 52.3 51.8 26.3 22.1 88.8 58.0;..
87.8 31.3 26.4 20.0 26.4 84.0 82.8 31.0 62.1 56.0 83.3 43.6  1.4 20.7 27.9;..
 6.8 36.2 41.5 56.2 52.5 12.1 92.6 25.5 34.5 56.2 61.0 91.1 81.9 85.3 95.5;..
56.1 29.2 28.1 59.0 53.8 28.6 56.7 62.5 70.6 46.8 18.7 80.8 13.0 67.4 90.7;..
66.2 56.6 12.8 68.5 12.0 86.1 57.1 11.6 52.1 77.9  1.9 81.0 96.8 91.5 33.6;..
72.6 48.3 77.8 89.1 22.6 84.9 81.6 61.2 28.7 79.0 84.3 25.9 65.6  2.8 11.8;..
19.9 33.2 21.2 50.4 62.7 52.6  5.7 67.8 65.0 98.1  7.5 41.4 24.5 23.7 92.5;..
54.4 59.4 11.2 34.9 76.1 99.3 56.0 33.2  8.8 81.9 85.3 36.0 52.8 70.2 72.6;..
];
d=d-diag(diag(d));
c=zeros(n,n);f=c;
f(2:n,1)=[1:n-1]';
[crit,order]=qassign(c,f,d)

// salesman
ta=[2  1 3 2 2 4 4 5 6 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[1 10 2 5 7 3 2 4 5 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',0,17,ta,he);
g1=make_graph('foo1',1,17,[ta he],[he ta]);
m=arc_number(g1);
v=[18 23 24 17 12 22 14 19 30  5 16  6 17 13  1 17 21 28  4  9  0 17 12 21 29..
26 10 26  3 16 16 28 29 19 29 23 16 22  1 12  3  8 11  5 28 23 17 11 26 25..
20  2  8 22 11 23];
g1('edge_length')=5+v;
cir = salesman(g1)

// save_graph
g=load_graph(SCI+'/demos/metanet/mesh100');
if MSDOS then
  if fileinfo('mymesh100.graph')<>[] then
    unix_s('del '+'mymesh100.graph');
  end
else
  unix('rm -f mymesh100.graph');
end
save_graph(g,'mymesh100.graph');
g=load_graph('mymesh100')
if MSDOS then
  unix_s('del '+'mymesh100.graph');
else
  unix('rm -f mymesh100.graph');
end

// shortest_path
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1];
g=make_graph('foo',1,15,ta,he);
g1=g;ma=prod(size(g1('head')));
g1('edge_length')=[13 17 10 6 7 18 18 6 7 14 5 9 5 10 10 2 4 12 15 0 13 4 7 16 11 9 4 16 2];
[p,lp]=shortest_path(13,1,g1,'length')

// split_edge
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
gt=split_edge(1,2,g)

// strong_con_nodes
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14];
g=make_graph('foo',1,15,ta,he);
ncomp=strong_con_nodes(3,g)

// strong_connex
ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14];
g=make_graph('foo',1,15,ta,he);
[nc,ncomp]=strong_connex(g)

// subgraph
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
v=[2 3 4 5 17 13 10];
g1=subgraph(v,'nodes',g)
v=[10 13 12 16 20 19];
g1=subgraph(v,'edges',g)

// successors
ta=[1  6  2  4  7  5  6  8  4  3  5  1];
he=[2  1  3  6  4  8  8  7  2  7  3  5];
g=make_graph('foo',1,8,ta,he);
a=successors(6,g)

// supernode
ta=[1 1 2 2 2 3 4 5 5 7 8 8 9 10 10 10 10 10 11 12 13 13 13 14 15 16 16 17 17];
he=[2 10 3 5 7 4 2 4 6 8 6 9 7 7 11 13 13 15 12 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
v=[7 10 13 9];
g1=supernode(v,g)

// trans_closure
ta=[2 3 3 5 3 4 4 5 8];
he=[1 2 4 2 6 6 7 7 4];
g=make_graph('foo',1,8,ta,he);
g1=trans_closure(g)
