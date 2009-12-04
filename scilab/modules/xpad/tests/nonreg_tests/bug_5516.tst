// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan SIMON
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH XPAD -->
//
// <-- Non-regression test for bug 5516 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5516
//
// <-- Short Description -->
// Encoding preference is not saved


//xpad()
//choose "Menu→Document→Encoding US-ASCII"
//close xpad
//xpad()
//=> "Menu→Document→Encoding" should be US-ASCII

