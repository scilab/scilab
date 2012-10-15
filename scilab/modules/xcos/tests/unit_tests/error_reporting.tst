// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Short Description -->
// Check all the possible errors which should be reported on the diagram.


//-----------------------------------------------------------------------------
// at edition (sb port numbering)
//-----------------------------------------------------------------------------

// Check the report against wrong port numbering on super block
xcos(SCI + "/modules/xcos/tests/unit_tests/error_sb_port_numbering.xcos")
// open the super block
// check that the second input port report an error
// change the second input port value (to 2)
// check that the second input port error is cleared
// change the second input port value (to 5)
// check that the second input port report an error
// change the first output port value (to 3)
// check that the first output port report an error
// change the first output port value (to 1)
// check that the first output port error is cleared

//-----------------------------------------------------------------------------
// on do_eval (context evaluation)
//-----------------------------------------------------------------------------

// Check the report against an invalid context value
xcos(SCI + "/modules/xcos/tests/unit_tests/error_blocks_with_updated_context.xcos")
// simulate
// a comprehensive error should be reported but the simulation should run
xcos(SCI + "/modules/xcos/tests/unit_tests/error_blocks_with_updated_context_on_sb.xcos")

// Check the report against a not connected link
xcos(SCI + "/modules/xcos/tests/unit_tests/error_blocks_with_not_connected_link.xcos")
// simulate
// a comprehensive error should be reported but the simulation should run

//-----------------------------------------------------------------------------
// on c_pass1 (flow checking)
//-----------------------------------------------------------------------------

// Check the report against a removed flow path
xcos(SCI + "/modules/xcos/tests/unit_tests/error_blocks_with_undefined_input.xcos")
// compile
// a comprehensive error should be reported
xcos(SCI + "/modules/xcos/tests/unit_tests/error_blocks_with_undefined_input_on_sb.xcos")
// compile
// a comprehensive error should be reported

// Check the report against a from block without a goto block
xcos(SCI + "/modules/xcos/tests/unit_tests/error_from_without_goto.xcos")
// compile
// a comprehensive error should be reported
xcos(SCI + "/modules/xcos/tests/unit_tests/error_from_without_goto_on_sb.xcos")
// compile
// a comprehensive error should be reported

// Check the report against a two goto blocks with the same tag
xcos(SCI + "/modules/xcos/tests/unit_tests/error_goto_same_tag.xcos")
// compile
// a comprehensive error should be reported
xcos(SCI + "/modules/xcos/tests/unit_tests/error_goto_same_tag_on_sb.xcos")
// compile
// a comprehensive error should be reported

// Check the report against a port block out of a superblock
xcos(SCI + "/modules/xcos/tests/unit_tests/error_port_block_on_diagram.xcos")
// compile
// a comprehensive error should be reported

// Check the report against an unconnected superblock input
xcos(SCI + "/modules/xcos/tests/unit_tests/error_unconnected_sb_input.xcos")
// compile
// a comprehensive error should be reported

//-----------------------------------------------------------------------------
// on c_pass2 (type and size checking)
//-----------------------------------------------------------------------------

// Check the report against an unconnected superblock input
xcos(SCI + "/modules/xcos/tests/unit_tests/error_ports_with_incompatible_size.xcos")
// compile
// a comprehensive error should be reported
xcos(SCI + "/modules/xcos/tests/unit_tests/error_ports_with_incompatible_size_on_sb.xcos")
// compile
// a comprehensive error should be reported

// Check the report against an output with negative size (undetermined)
xcos(SCI + "/modules/xcos/tests/unit_tests/error_ports_with_negative_size.xcos")
// compile
// a comprehensive error should be reported
xcos(SCI + "/modules/xcos/tests/unit_tests/error_ports_with_negative_size_on_sb.xcos")
// compile
// a comprehensive error should be reported
xcos(SCI + "/modules/xcos/tests/unit_tests/error_ports_with_negative_size_on_sb.xcos")
// generate code for the main superblock
// a comprehensive error should be reported

//-----------------------------------------------------------------------------
// on simulation (Coserror)
//-----------------------------------------------------------------------------

// Check the report against a non existing variable on the FROMWS block
xcos(SCI + "/modules/xcos/tests/unit_tests/error_on_block.xcos")
// simulate
// a comprehensive error should be reported

