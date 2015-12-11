// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Cedric Delamarre
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 14036 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14036
//
// <-- Short Description -->
// userdata and tag are hidden light properties

l = light();
l.userdata = "Source 1";
l.tag = "LS1";

l