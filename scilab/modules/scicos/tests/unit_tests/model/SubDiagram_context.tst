// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

scs_m = scicos_diagram( objs=list(scicos_block()) );
scs_m.props.context = ["My context"; "with two lines"];
assert_checkequal(scs_m.props.context, ["My context"; "with two lines"]);

super = scicos_block();
super.model.rpar = scs_m;

// Check that the simple diagram's context is equal to the superblock's diagram's context (it has been preserved)
assert_checkequal(super.model.rpar.props.context, scs_m.props.context);


// Check that all the model items are freed
clear
