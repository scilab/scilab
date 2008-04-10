// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 174 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=174
//
// <-- Short Description -->
//   Sur des images de grandes tailles, Matplot trace plein de lignes blanches, ce qui est assez genant. 

// <-- TEST WITH GRAPHIC -->

Matplot(3*rand(64,64));

