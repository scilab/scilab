// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// unit tests for bandwr function
// =============================================================================

ta=[2  1 3 2 2 4 4 5 6 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[1 10 2 5 7 3 2 4 5 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',0,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
// THE GRAPH
show_graph(g);
a=graph_2_mat(g,'node-node');
ww=tril(a)'+eye();
ww1=full(ww);
xset('window',1)
hist3d((ww1+tril(ww1',-1)+tril(ww1,-1)'),52,85); 
// BANDWIDTH REDUCTION FOR THE MATRIX
[iperm,mrepi,prof,ierr]=bandwr(ww);
max(prof(2:$)-prof(1:($-1)))
// GRAPH WITH THE NEW NUMBERING
g2=g;g2('node_name')=string(iperm);
show_graph(g2,'new')
// NEW MATRIX
n=g('node_number');
yy=ww1(mrepi,mrepi);
xset('window',3)
hist3d((yy+tril(yy',-1)+tril(yy,-1)'),52,85); 
// STARTING WITH THE SAME MATRIX
[ij,v,mn]=spget(ww);
g1=make_graph('foo',0,n,ij(:,1)',ij(:,2)');
g1('node_x')=g('node_x');g1('node_y')=g('node_y');
// GRAPH
//show_graph(g1,'rep');
[lp,la,l_s] = adj_lists(1,n,g1('tail'),g1('head'));
[iperm,mrepi,prof,ierr]=bandwr(lp,l_s,n,0);
g2=g;g2('node_name')=string(iperm);
show_graph(g2,'new');
 

