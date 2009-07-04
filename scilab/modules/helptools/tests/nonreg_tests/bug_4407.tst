// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 4407 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4407
//
// <-- Short Description -->
//  help file scilab_xx_XX_help.jar have another scilab_xx_XX_help.jar embedded


// build java help

xmltojar()

// check that scilab_xx_XX_help.jar do not have another scilab_xx_XX_help.jar embedded