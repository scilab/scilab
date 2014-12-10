// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

loadXcosLibs();
scicos_log("TRACE");

// Create one block and make a list out of it (duplicate it)
Sum = BIGSOM_f("define");
l = list(Sum);

// Copy the list content (no duplicate)
Sum2 = l(1)

// The following action should not delete the Block in the model
clear Sum

// Sum2's refCount must have been affected by "clear Sum"
Sum2

// Delete the original block in the model
clear
