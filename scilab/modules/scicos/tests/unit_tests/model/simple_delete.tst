// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

// Test #1: the temporary block is deleted with the diagram
scs_m = scicos_diagram( objs=list(scicos_block()) );
clear scs_m

// Test #2: the block is deleted after the diagram
blk = scicos_block();
scs_m = scicos_diagram( objs=list(blk) );
clear scs_m
clear blk
