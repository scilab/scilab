// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4055 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4055
//
// <-- Short Description -->
// From time to time, when I plot a graphic, scilab displays a blank graphic windows.

if MSDOS then

if fileinfo(SCI+'\bin\jogl.dll') == [] then pause,end
if fileinfo(SCI+'\bin\jogl_awt.dll') == [] then pause,end
if fileinfo(SCI+'\bin\jogl_cg.dll') == [] then pause,end
if fileinfo(SCI+'\bin\gluegen-rt.dll') == [] then pause,end

if fileinfo(SCI+'\thirdparty\gluegen-rt.jar') == [] then pause,end
if fileinfo(SCI+'\thirdparty\jogl.jar') == [] then pause,end

end

