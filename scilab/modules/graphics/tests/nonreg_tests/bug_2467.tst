// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Serge STEER <serge.steer@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2467 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2467
//
// <-- Short Description -->
//    when trying to open plot files (scg) containing graphs with
//    logarithmic axes, I get this mysterious error message..

plot(1:10)
a             = gca();
a.log_flags   = 'lnn';
path          = TMPDIR+'/bug2467.scg';
xsave(path);
clf();

if execstr("xload(path)",'errcatch')<>0 then pause,end
