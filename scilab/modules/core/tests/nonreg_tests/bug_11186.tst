// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 11186 -->
//
// <-- CLI SHELL MODE -->
// 
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=11186
//
// <-- Short Description -->
// 
// 'typeof' of a type 130 returned an error
//

assert_checkequal(typeof(disp), 'fptr');