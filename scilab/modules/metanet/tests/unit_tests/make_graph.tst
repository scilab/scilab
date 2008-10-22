// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// unit tests for make_graph function
// =============================================================================

// creating a directed graph with 3 nodes and 4 arcs.
g=make_graph('foo',1,3,[1,2,3,1],[2,3,1,3]);
// creating a directed graph with 13 nodes and 14 arcs.
ta=[1  1 2 7 8 9 10 10 10 10 11 12 13 13];
he=[2 10 7 8 9 7  7 11 13 13 12 13  9 10];
g=make_graph('foo',1,13,ta,he);
g('node_x')=[120  98  87 188 439 698 226 127 342 467 711 779 477];
g('node_y')=[ 21 184 308 426 435 428 129 360 435  55 109 320 321];
show_graph(g)
// creating same graph without isolated node and 14 arcs.
g=make_graph('foo',1,0,ta,he);
g('node_x')=[120  98 226 127 342 467 711 779 477];
g('node_y')=[ 21 184 129 360 435  55 109 320 321];
show_graph(g,'new')
 

