// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2015-2015 - Clément DAVID
//
// This file is distributed under the same license as the Scilab package.

// <-- XCOS TEST -->
//
// <-- Short Description -->
// Internal test to check that xcos files and zcos files can be loaded and saved

function check_file(path, file)

    // load the empty file and save it
    mprintf("IMPORT %s\n", SCI+"/"+path+"/"+file);
    scs_m = xcosDiagramToScilab(SCI+"/"+path+"/"+file)
    mprintf("EXPORT %s\n", TMPDIR+"/"+file);
    xcosDiagramToScilab(TMPDIR+"/"+file, scs_m)

    // then reload it and check that the two diagrams are equals
    old_scs_m = scs_m;
    mprintf("REIMPORT %s\n", TMPDIR+"/"+file);
    scs_m = xcosDiagramToScilab(TMPDIR+"/"+file);
    //    assert_checktrue(old_scs_m == scs_m);
endfunction

check_file("modules/xcos/tests/unit_tests", "empty.xcos");
check_file("modules/xcos/tests/unit_tests", "empty.zcos");

check_file("modules/xcos/demos", "Simple_Demo.zcos");

