//==============================================================================
// Authors : Claude GOMEZ, Serge STEER
// Scilab team
// Copyright INRIA
// Date : 01 Feb, 2008
//==============================================================================

// unit tests for graph_2_mat function
// =============================================================================

g=load_graph(SCI+'/demos/metanet/colored');
a=graph_2_mat(g)
a=graph_2_mat(g,'node-node')


