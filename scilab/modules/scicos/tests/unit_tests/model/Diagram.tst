// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

loadXcosLibs();

// Allocate a diagram and access its fields
scs_m = scicos_diagram()
scs_m.props
scs_m.objs
scs_m.version
scs_m.contrib

// Create a diagram containing 2 summation blocks and two links
Sum    = BIGSOM_f("define");
Scope1 = CSCOPE("define");
Scope2 = CSCOPE("define");
lnk1   = scicos_link();          // Data link
lnk2   = scicos_link(ct=[1,-1]); // Activation link

scs_m = scicos_diagram( objs=list(Sum,Scope1,Scope2,lnk1,lnk2) )
scs_m.objs

// Link output port #1 of block #1 with input port #1 of block #2 thanks to lnk1
l = scs_m.objs;
l(4).from = [1 1 0]; // Link block #1
l(1).graphics.pout   // Check that block #1 is connected to lnk1
l(1).model.out       // "
l(4).to = [2 1 1];   // Link block #2
l(4).from
l(4).to
l(2).graphics.pin    // Check that block #2 is connected to lnk1
l(2).model.in        // "

// Change the end of the link to input port #2 of block #2
l(4).to = [2 2 1];
l(4).from
l(4).to
l(2).graphics.pin    // Check that block #2 is connected to lnk1
l(2).model.in        // "

// Disconnect the source, the destination is also disconnected
l(4).from = [0 0 0];
l(4).from
l(4).to
l(1).graphics.pout   // Check that both blocks are unconnected
l(2).graphics.pin    // "

// Now link the two Scope blocks together by adding an event output port to block #3 thanks to lnk2
l(5).from = [2 1 1]; // Link the input of block #2
l(5).to = [3 1 0];   // Add an event output to block #3 and link it to the previous
l(5).from
l(5).to
l(2).graphics.pein   // Check that block #2 is connected to lnk2
l(2).model.evtin     // "
l(3).graphics.peout  // Check that block #3 is connected to lnk2
l(3).model.evtout    // "

// Verify that it is impossible to link two inputs or two outputs together
l(5).from = [2 1 0]; // Two outputs
l(5).to = [3 1 1];   // Two inputs
