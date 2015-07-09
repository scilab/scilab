// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- NO CHECK REF -->
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 6776 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6776
//
// <-- Short Description -->
// the non-empty block.doc field was not cleared on loading.
//


// Check that importScicosDiagram perform block.doc clearing
scs_m = importScicosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/Antrieb3.cos");
obj = scs_m.objs(1);
if size(obj.doc) <> 0 then pause, end

// check that the export will not fail
tempFile = TMPDIR + "/6776temp.sod";
err = execstr("save(tempFile, ""scs_m"")", "errcatch");
if err <> %t then pause, end

deletefile(tempFile);

