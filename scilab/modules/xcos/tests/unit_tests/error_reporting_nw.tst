// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- XCOS TEST -->
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// Check all the possible errors which should be reported on the diagram.


//-----------------------------------------------------------------------------
// at edition (sb port numbering)
//-----------------------------------------------------------------------------

// Check the report against wrong port numbering on super block
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/error_sb_port_numbering.xcos"));
scicos_simulate(scs_m, "nw");

//-----------------------------------------------------------------------------
// on do_eval (context evaluation)
//-----------------------------------------------------------------------------

// Check the report against an invalid context value
assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/tests/unit_tests/error_blocks_with_updated_context.xcos"));
scicos_simulate(scs_m, "nw");


