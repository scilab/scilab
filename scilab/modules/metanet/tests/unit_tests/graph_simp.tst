// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// unit tests for graph_simp function
// =============================================================================

ta=[1 1  1 2 2 2 3 4 4 4 5 5 6 7 7 8 8 9 9 10 10 10 10 10 11 12 12 13 13 13 14 15 16 16 17 17];
he=[1 2 10 3 5 7 4 2 9 9 4 6 6 8 2 6 9 7 4  7 11 13 13 15 12 11 13 9 10 14 11 16 1 17 14 15];
g=make_graph('foo',1,17,ta,he);
g('node_x')=[283 163  63  98 164 162 273 235 267 384 504 493 409 573 601 627 642];
g('node_y')=[ 59 133 223 311 227 299 221 288 384 141 209 299 398 383 187 121 301];
show_graph(g);
g1=graph_simp(g);
show_graph(g1,'new');
 
