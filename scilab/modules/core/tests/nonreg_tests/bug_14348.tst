// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 14348 -->
//
// <-- WINDOWS ONLY -->
//
// <-- INTERACTIVE TEST -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/14348
//
// <-- Short Description -->
// Scilab does not open sce/sci file in scilab correctly when calling by windows explorer ( double click on file )

//open a Windows explorer and double click on a sce/sci file.
//Scilab must be opened with scinotes and your selected file.
//double click on another sce/sci file.
//this file must be opened in the same Scilab, without launching a new one.