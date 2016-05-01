// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
//
// <-- CLI SHELL MODE -->

scicos_log("TRACE");

// Loading a Superblock containing an mlist as subdiagram
exec("SCI/modules/scicos/palettes/Lookup_Tables.cosf", -1);

// Check that all the model items are freed
clear
