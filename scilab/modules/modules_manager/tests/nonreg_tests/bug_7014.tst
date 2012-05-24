// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->
// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 7014 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7014
//
// <-- Short Description -->
// ilib_verbose value modified after toolbox_skeleton

// launch scilab

DEFAULT_VALUE_VERBOSE = ilib_verbose();

cd SCI/contrib/toolbox_skeleton
exec builder.sce;
exec loader.sce;

if ilib_verbose() <> DEFAULT_VALUE_VERBOSE then pause, end
