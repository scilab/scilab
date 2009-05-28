// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->


// <-- Non-regression test for bug 4386 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4298
//
// <-- Short Description -->
// ilib_for_link fails with lcc-win32

if MSDOS then
  if with_lcc() then
    if fileinfo(SCIHOME + '/lcclib/blaspluslcc.lib') <> [] then pause,end
    if fileinfo(SCIHOME + '/lcclib/blasplus.lib') == [] then pause,end
  end
end