// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6831 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6831
//
// <-- Short Description -->
// in cli (=nwni) mode, tbx_build_help failed.

// launch scilab as CLI or NWNI mode

cd SCI/contrib/toolbox_skeleton
exec builder.sce;

// toolbox should be built, but help generation must be skipped
