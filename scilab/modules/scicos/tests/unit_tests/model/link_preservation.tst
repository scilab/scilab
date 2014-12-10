// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

b = scicos_block();
d = scicos_diagram( objs=list(scicos_link(from=[3 3 0],to=[5 2 1]) ));
b.model.rpar = d;
// b is now a SuperBlock containing a diagram containing a link, with its 'from' and and 'to' properties set

assert_checkequal(b.model.rpar.objs(1).from, [3 3 0]);
assert_checkequal(b.model.rpar.objs(1).to, [5 2 1]);


// Check that all the model items are freed
clear
