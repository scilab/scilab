// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre KOUMAR
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
// <-- TEST WITH SCINOTES -->
//
// <-- Non-regression test for bug 5290 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5290
//
// <-- Short Description -->
//    Having two functions in Scilab for the editor is useless: editorHighlightLine & editor
//    We could add only one function with the profile:
//    editor(['SCI/modules/time/macros/datenum.sci','SCI/modules/time/macros/datevec.sci'],[10,23]); 
//    which will open datenum.sci at the line 10 and datevec.sci at the line 23

//    It will avoid many duplication (help pages, gateway) and the fact that editorHighlightLine does not accept the
//    following syntax (ie a matrix of filename as input argument):
//    editorHighlightLine(["/etc/resolv.conf","/etc/hosts"],[2,2]);
//    or
//    editorHighlightLine(["/etc/resolv.conf","/etc/hosts"],2);



editor(['SCI/modules/time/macros/datenum.sci','SCI/modules/time/macros/datevec.sci'],[10,23]); 






