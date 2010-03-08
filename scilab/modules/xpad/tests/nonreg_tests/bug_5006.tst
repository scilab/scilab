// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 5006 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5006
//
// <-- Short Description -->
// 

// editor does not save my modifications in a new file

cd TMPDIR

mdelete('fun1.sci') // to be sure that file does not exist
editor fun1.sci

//copy/paste in fun1.sci

//function fun1()
//  disp('OK')
//endfunction


// save file in editor
// close editor

editor fun1.sci  // checks that you have text

mgetl('fun1.sci')
