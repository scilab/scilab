//==============================================================================
// Authors : Claude GOMEZ, Serge STEER
// Scilab team
// Copyright INRIA
// Date : 01 Feb, 2008
//==============================================================================

// unit tests for mat_2_graph function
// =============================================================================

g=load_graph(SCI+"/modules/metanet/tests/unit_tests/colored");
show_graph(g);
a=graph_2_mat(g);
g1=mat_2_graph(a,1);
g1('node_x')=g('node_x'); g1('node_y')=g('node_y');
show_graph(g1,'new');
a=graph_2_mat(g,'node-node');
g1=mat_2_graph(a,1,'node-node');
g1('node_x')=g('node_x'); g1('node_y')=g('node_y');
show_graph(g1,'new');
