// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 4607 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4607
//
// <-- Short Description -->
//    clipboard does not empty

clipboard('do','empty');
REF_STR = 'STRING copied in clipboard';
clipboard('copy', REF_STR);
r = clipboard('paste');
if r <> REF_STR then pause,end
clipboard('do','empty');
r = clipboard('paste');
if r <> '' then pause,end

