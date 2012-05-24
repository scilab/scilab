// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug C -->
//
// <-- OLD BUG -->

files     = [''];
files     = listfiles([SCI+'/modules/core/macros/*.bin']);
if files == [''] then pause,end
