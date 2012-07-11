// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre .marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 1914 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1914
//
// <-- Short Description -->
// Function grep does not implement regular expressions. Therefore either the  
// function name should change to something like "stringmatch" or true regular  
// expression capabilities should be implemented.
//

if grep("01234","/[0-9]/","r") <> 1 then pause,end
