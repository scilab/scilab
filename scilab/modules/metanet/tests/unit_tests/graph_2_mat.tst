// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// unit tests for graph_2_mat function
// =============================================================================

g=load_graph(SCI+'/modules/metanet/demos/colored.graph');
a=graph_2_mat(g)
a=graph_2_mat(g,'node-node')
