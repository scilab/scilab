// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3757 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3757
//
// <-- Short Description -->
// Under Scilex, the completion of paths is incorrect


// <-- INTERACTIVE TEST -->

if MSDOS then

mkdir('d:\Project')
cd('d:\Project')
mkdir('d:\Test')                    
cd ../../
cd d:\Pro[TAB]

// it should be 
// cd d:\Project