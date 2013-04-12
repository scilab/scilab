// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Yann Collette
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 5395 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5395
//
// <-- Short Description -->
// The xcos block AFFICH_m do not bother with the max number of digits.
// 

// Using xcos, open the file test_affich_m.zcos and see if when there
// is an overflow is the display of affich_m that the number is
// still correctly displayed.
 
xcos();
