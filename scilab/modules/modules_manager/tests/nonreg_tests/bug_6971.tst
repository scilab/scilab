// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 6971 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6971
//
// <-- Short Description -->
// after launching a toolbox by loader.sce, variable v was predefined.

// launch scilab
cd SCI/contrib/toolbox_skeleton
exec builder.sce;
exec loader.sce;
if isdef('v') then pause,end
