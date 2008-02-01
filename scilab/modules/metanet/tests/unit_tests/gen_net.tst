//==============================================================================
// Authors : Claude GOMEZ, Serge STEER
// Scilab team
// Copyright INRIA
// Date : 01 Feb, 2008
//==============================================================================

// unit tests for gen_net function
// =============================================================================

v=[1,10,2,1,0,10,100,100,0,100,50,50];
g=gen_net('foo',1,v);
show_graph(g)
// generating using dialogs
g=gen_net();
show_graph(g)
 

