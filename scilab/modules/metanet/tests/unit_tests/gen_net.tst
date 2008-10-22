// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
// Copyright (C) ????-2008 - INRIA - Claude GOMEZ <claude.gomez@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// ==============================================================================
// <-- INTERACTIVE TEST -->
// <-- TEST WITH GRAPHIC -->

// unit tests for gen_net function
// =============================================================================

v=[1,10,2,1,0,10,100,100,0,100,50,50];
g=gen_net('foo',1,v);
show_graph(g)
// generating using dialogs
g=gen_net();
show_graph(g)
 

