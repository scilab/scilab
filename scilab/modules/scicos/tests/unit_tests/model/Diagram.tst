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

// Create a diagram containing 2 summation blocks and an empty link
blk  = BIGSOM_f("define");
blk2 = BIGSOM_f("define");
lnk  = scicos_link();

scs_m = scicos_diagram( objs=list(blk,blk2,lnk) )
scs_m.objs
