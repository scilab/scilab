// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

// Allocate a SuperBlock and access its fields
sb = SUPER_f("define")
sb.graphics
sb.model
sb.gui
sb.doc

// Access the contained diagram's fields
sb.model.rpar
sb.model.rpar.props

sb.model.rpar.objs
sb.model.rpar.objs(1).graphics
sb.model.rpar.objs(1).model
sb.model.rpar.objs(2).graphics
sb.model.rpar.objs(2).model

sb.model.rpar.version
sb.model.rpar.contrib

clk = CLOCK_c("define")
clk.graphics
clk.model
clk.model.rpar
clk.model.rpar.props

clk.model.rpar.objs
clk.model.rpar.objs(1).graphics
clk.model.rpar.objs(1).model
clk.model.rpar.objs(2).graphics
clk.model.rpar.objs(2).model
clk.model.rpar.objs(3).from
clk.model.rpar.objs(3).to
clk.model.rpar.objs(4).graphics
clk.model.rpar.objs(4).model
clk.model.rpar.objs(5).from
clk.model.rpar.objs(5).to
clk.model.rpar.objs(6).from
clk.model.rpar.objs(6).to


// Check that all the model items are freed
clear sb clk
