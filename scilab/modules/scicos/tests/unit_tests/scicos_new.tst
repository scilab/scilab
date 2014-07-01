// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Clement David
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================+

loadXcosLibs();

p = funcprot();
funcprot(0);
mlist = scicos_new;
tlist = scicos_new;
funcprot(p);

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

// allocate a Link
o = scicos_diagram()

