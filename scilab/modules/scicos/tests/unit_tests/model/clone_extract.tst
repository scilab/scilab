// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

// Creating a Diagram containing a Link connected to a Block
scs = scicos_diagram( objs=list(scicos_block(),scicos_link(from=[1,1,0])) );

// Extracting and modifying the Link should not modify the original Diagram
newLink = scs.objs(2);
newLink.from = [3 1 0];

assert_checkequal(scs.objs(1).graphics.pout, 2);
assert_checkequal(scs.objs(2).from, [1 1 0]);
assert_checkequal(newLink.from, [3 1 0]);


// Check that all the model items are freed
clear
