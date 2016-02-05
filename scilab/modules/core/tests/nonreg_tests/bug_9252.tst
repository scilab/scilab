// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Charlotte HECQUET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 9252 -->
//
// <-- CLI SHELL MODE -->
// 
// <-- INTERACTIVE TEST -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9252
//
// <-- Short Description -->
// Some error prototypes produce screwed display

function bugme()
    error(666,"My freaking error")
endfunction

bugme

// Check that the error message is this one:
// !--error 666 
//My freaking error
//at line       2 of function bugme called by :  
//bugme

// and not this one:
// !--error 666 
//My freaking errorat line       2 of function bugme called by :  
//bugme()
