// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 768 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=768
//
// <-- Short Description -->
//    as on Scilab 2.7.2, the "window" function still has a bug 
//    when choosing the "chebychev" type with its corresponding 
//    parameter vector. SAys the parameters are out of bounds 
//    whereas it is wrong

err_number  = execstr('window(''ch'',128,[0.25 0.1])','errcatch');
err_message = lasterror();
if stripblanks(err_message,%T) <> msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the interval [%s, %s].\n"), "window", 3, "dp", "df") then pause,end
