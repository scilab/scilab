// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Clement David
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

loadXcosLibs();

// sub-objects not mapped to the model

o = scicos_graphics()
o = scicos_model()

// model objects

// allocate an Annotation
o = TEXT_f("define");

// allocate a Block
o = scicos_block()

// allocate a Link
o = scicos_link()

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

