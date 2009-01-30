// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2255-->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2255
//
// <-- Short Description -->
// plot function fails to verify that x and y data have the same length in certain cases.
// 

err = execstr('plot(ones(1,2048),ones(1,1024))','errcatch','n');
// should produce an error message
if (err == 0) then pause; end
