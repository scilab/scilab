// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Clement David
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();

// sub-objects not mapped to the model

o = scicos_graphics()
o = scicos_model()

// model objects

// allocate an Annotation
o = TEXT_f("define");
clear o;

// allocate a Block
o = scicos_block()
clear o;

// allocate a Link
o = scicos_link()
clear o;

// allocate a Diagram
scs_m = scicos_diagram()

// allocate a specific block
blk = BIGSOM_f("define");

// manipulate a field
model = blk.model;
model.in = [1 1 1 1]';
blk.model = model;

// add a block to a diagram
scs_m.objs(1) = blk;

// add another block to a diagram (have to perform a copy)
scs_m.objs($+1) = blk;

// add a link to connect blocks
scs_m.objs($+1) = scicos_link();
lnk = scs_m.objs($);
lnk.from = [1 1 0]
lnk.to = [2 1 1]
scs_m.objs($+1) = lnk;

// use scicos_new to retrieve and adapter from an uid
blk2 = scicos_new(blk.modelID);

clear blk
blk2.model.rpar;
clear blk2

