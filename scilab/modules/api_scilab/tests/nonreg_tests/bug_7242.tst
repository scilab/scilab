// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Antoine ELIAS
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7242 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7242
//
// <-- Short Description -->
//  API was unable to retrieve named variable address
//  when variable name is passed in argument of function

function myExport(data, filepath)
    save(filepath, "data");
endfunction

myData = "blam";
myFile = TMPDIR + "/myFile.sod";

myExport(myData, myFile); // Should not produce any error
