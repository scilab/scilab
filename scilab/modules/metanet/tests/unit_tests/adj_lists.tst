// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// unit tests for adj_lists function
// =============================================================================

ta=[2 3 3 5 3 4 4 5 8];
he=[1 2 4 2 6 6 7 7 4];
g=make_graph('foo',1,8,ta,he);
g('node_x')=[129 200 283 281 128 366 122 333];
g('node_y')=[61 125 129 189 173 135 236 249];
show_graph(g)
[lp,la,ls]=adj_lists(g)
[lp,la,ls]=adj_lists(1,g('node_number'),ta,he)
