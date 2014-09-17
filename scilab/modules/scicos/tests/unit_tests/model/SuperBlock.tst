// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

loadXcosLibs();

// Allocate a SuperBlock and access its fields
sb = SUPER_f("define")
sb.graphics
sb.model
sb.gui
sb.doc

// Access the contained diagram's fields
diag = sb.model.rpar
diag.props

objs = diag.objs
objs(1).graphics
objs(1).model
objs(2).graphics
objs(2).model

diag.version
diag.contrib
