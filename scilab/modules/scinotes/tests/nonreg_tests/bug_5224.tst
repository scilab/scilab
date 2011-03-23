// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5224 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5224
//
// <-- Short Description -->
//    Maj+Tab modify the text selection

editor()

// write the 3 following line :

//Eminuit autem inter humilia 
//  supergressa iam impotentia 
//  fines mediocrium delictorum

// Select line the whole line 2 & 3

// <Maj+Tab>

// The selection should no more start at the end of the line 1







