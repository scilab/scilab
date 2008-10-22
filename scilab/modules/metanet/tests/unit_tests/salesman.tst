// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// unit tests for salesman function
// =============================================================================

ta=[2  1 3 2 2 4 4 5 6 7 8 8 9 10 10 10 10 11 12 13 13 14 15 16 16 17 17];
he=[1 10 2 5 7 3 2 4 5 8 6 9 7 7 11 13 15 12 13  9 14 11 16 1 17 14 15];
g=make_graph('foo',0,17,ta,he);
g('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g('node_diam')=[1:(g('node_number'))]+20;
show_graph(g);
g1=make_graph('foo1',1,17,[ta he],[he ta]);
g1('node_x')=[283 163 63 57 164 164 273 271 339 384 504 513 439 623 631 757 642];
g1('node_y')=[59 133 223 318 227 319 221 324 432 141 209 319 428 443 187 151 301];
g1('node_diam')=[1:(g1('node_number'))]+20;
m=arc_number(g1);
g1('edge_length')=5+round(30*rand(1,m));

cir = salesman(g1);
ii=find(cir > edge_number(g)); 
if(ii <> []) then cir(ii)=cir(ii)-edge_number(g);end;
show_arcs(cir);
 

