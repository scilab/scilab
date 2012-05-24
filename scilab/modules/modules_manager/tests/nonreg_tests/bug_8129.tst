// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 8129 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8129
//
// <-- Short Description -->
// a error message was obscure in tbx_builder_src

// Try to build a toolbox needing building from sources on a machine 
// without a compiler (tested here on XP).
// for example SCI/contrib/toolbox_skeleton

// Message must be :
// tbx_builder_src: This module requires a C or Fortran compiler and it has not been found.
