// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// unit tests for connex function
// =============================================================================

ta=[1 1 2 2 2 3 4 4 5 6 7 7 7 8 9 10 12 12 13 13 14 15];
he=[2 6 3 4 5 1 3 5 1 7 5 8 9 5 8 11 10 11 11 15 13 14];
g=make_graph('foo',1,15,ta,he);
g('node_x')=[197 191 106 194 296 305 305 418 422 432 552 550 549 416 548];
g('node_y')=[76 181 276 278 276 83 174 281 177 86 175 90 290 397 399];
show_graph(g);
[nc,ncomp]=connex(g)
g('node_color')=10+ncomp; 
g('node_diam')=10+10*ncomp;
show_graph(g);
 
