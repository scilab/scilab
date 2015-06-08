// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 7943 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7943
//
// <-- Short Description -->
// On a linux binary version, it was not possible to use the fortran_block block.
//

// copy paste in scilab with a binary version (installed)
// click ok (message box)

loadXcosLibs();
scs_m_tmp = fortran_block("define");
scs_m_res = fortran_block("set", scs_m_tmp, []);
if ~isdef("scs_m_res") then pause, end

// pause level must be 0
