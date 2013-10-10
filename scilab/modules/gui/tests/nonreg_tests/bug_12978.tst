// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 12978 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/12978
//
// <-- Short Description -->
// exportUI returned an error when arg was a figure handle

exportUI(gcf());