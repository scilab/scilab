// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6419 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6419
//
// <-- Short Description -->
//  gstacksize() There was no way to retrieve minimum from scilab

gstacksize('min');
mingstacksize = gstacksize();

ierr = execstr('gstacksize(2048)','errcatch');
if ierr <> 1504 then pause,end

newmingstacksize = gstacksize();
if mingstacksize(1) <> newmingstacksize(1) then pause,end

