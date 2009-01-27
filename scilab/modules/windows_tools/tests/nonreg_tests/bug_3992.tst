// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3992 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3992
//
// <-- Short Description -->
// fix some invalid dependencies on external libraries

// <-- INTERACTIVE TEST -->

// checks that we don't have a dependency on libmmd.dll

// dumpbin /IMPORTS hashtable.dll
// dumpbin /IMPORTS integer.dll
// dumpbin /IMPORTS interpolation.dll
// dumpbin /IMPORTS randlib.dll
// dumpbin /IMPORTS scicos.dll
// dumpbin /IMPORTS scicos_blocks.dll
// dumpbin /IMPORTS scicos_sundials.dll
// dumpbin /IMPORTS scigraphic_export.dll

// checks that we don't have a dependency on user32.dll
// dumpbin /IMPORTS io.dll
