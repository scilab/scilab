//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 1977 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1977
//
// <-- Short Description -->
//    inttype has stars in error message
//
//
//    -->inttype("hell")
//                    !--error 44
//    ***th argument is incorrect
//
//
//    Francois

execstr('inttype(''hell'')','errcatch');
if lasterror() <> msprintf(gettext("inttype: Wrong type for input argument #1 : int or double expected.\n")) then pause,end
