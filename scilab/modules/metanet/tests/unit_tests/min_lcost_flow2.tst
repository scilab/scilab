// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// unit tests for min_lcost_flow2 function
// =============================================================================

ta=[1 1 2 2 2 3 4 4 5 6 6 6 7 7 7 8 9 10 12 12 13 13 13 14 15 14 9 11 10 1 8];
he=[2 6 3 4 5 1 3 5 1 7 10 11 5 8 9 5 8 11 10 11 9 11 15 13 14 4 6 9 1 12 14];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[194 191 106 194 296 305 305 418 422 432 552 550 549 416 548];
g('node_y')=[56 221 316 318 316 143 214 321 217 126 215 80 330 437 439];
show_graph(g);
g1=g; ma=arc_number(g1); n=g1('node_number');
g1('edge_min_cap')=0.*ones(1,ma);
while %T then
 rand('uniform');
 g1('edge_max_cap')=round(20*rand(1,ma))+20*ones(1,ma);
 g1('edge_cost')=round(10*rand(1,ma)+ones(1,ma));
 rand('normal');
 dd=20.*rand(1,n)-10*ones(1,n);
 dd=round(dd-sum(dd)/n*ones(1,n));
 dd(n)=dd(n)-sum(dd);
 g1('node_demand')=dd;
 [c,phi,flag]=min_lcost_flow2(g1);
 if flag==1 then break; end;
end;
ii=find(phi<>0); edgecolor=phi; edgecolor(ii)=11*ones(ii);
g1('edge_color')=edgecolor;
edgefontsize=8*ones(1,ma); edgefontsize(ii)=18*ones(ii);
g1('edge_font_size')=edgefontsize;
g1('edge_label')=string(phi);
g1('node_label')=string(g1('node_demand'));
show_graph(g1);
 
   
