// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for checking loop links -->
//
// <-- Short Description -->
// Any block can contains non-port children. Some operation, cast without 
// checking the type.

// start demo --> standard demos --> Scilab block
// open the Scifunc block
// OK -> OK -> OK -> ...
// Dnd the SUM_f block
// link the output of this block to the first input.
// Open the block setting and enter [1;1;1] to add an input
// Select the block and use the "Region to super block" action
// Check that the block and the link is into the super block

