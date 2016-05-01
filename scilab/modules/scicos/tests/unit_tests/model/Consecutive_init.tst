// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

// Check consecutive initialization of a diagram in a SuperBlock
super = scicos_block();

d         = scicos_diagram();
sub       = scicos_block();
d.objs(1) = sub;

super.model.rpar = d;
// 'b' is a SuperBlock containing a Block
sizeBeforeInsert = size(super.model.rpar.objs);
super.model.rpar = d;
sizeAfterInsert  = size(super.model.rpar.objs);

assert_checkequal(sizeBeforeInsert, 1);
assert_checkequal(sizeAfterInsert,  1);

// Check consecutive initialization of a SuperBlock in a diagram
superD = scicos_diagram();

superD.objs(1) = super;
// 'superD' is a diagram containing a SuperBlock
sBeforeInsert = size(superD.objs(1).model.rpar.objs);
superD.objs(1) = super;
sAfterInsert  = size(superD.objs(1).model.rpar.objs);

assert_checkequal(sizeBeforeInsert, 1);
assert_checkequal(sizeAfterInsert,  1);


// Check that all the model items are freed
clear
