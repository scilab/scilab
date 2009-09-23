// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->

// unit tests for plot_graphbug function
// =============================================================================

// simple graph with different choices for the plot
ta=[2 2 1 1 2 4 3 3 4];
he=[2 2 3 2 3 2 1 2 1];
g=make_graph('foo',1,4,ta,he);
g('node_type')=[1 1 1 2];g('node_name')=string([1:4]);
g('node_x')=[73 737 381 391]; g('node_y')=[283 337 458 142];
g('node_color')=[3 3 3 11];
g('node_diam')=[30 30 30 60];
g('edge_color')=[10 0 2 6 11 11 0 0 11];
rep=[2 2 1 1 2 2 2 2 2 2 2 2 2];
rep1=[100 -400 650 300];
xbasc(); plot_graph(g,rep,rep1);
rep=[2 1 1 1 2 2 2 2 2 2 2 2 2];
xbasc(); plot_graph(g,rep,rep1);
// plotting using dialogs
xbasc(); plot_graph(g);
xset("thickness",4);
xbasc();
plot_graph(g);

