// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

// test_run  --
//   Launch unit tests.

function test_run_level(severity)
    disp("Run all test level " + string(severity));
    doc = xmlRead(SCI+"/modules/development_tools/data/test_run_level.xml");

    // Retrieve the list of modules
    modules=xmlXPath(doc, "/tests/level[@severity=" + string(severity) + "]/module");

    for (i=1:modules.size)

        // Retrieve the list of tests
        tests = xmlAsText(xmlXPath(modules(i),"test"));

        // Run the test
        status = test_run(modules(i).attributes.name,  tests, ["no_check_error_output","short_summary","show_diff","show_error"],TMPDIR+"/../test_runelevel_report.xml");

        if (status == %f) then
            error("At least one error has been detected. Failing all tests.");
            // Since make check is launched by the build system
            // It checks if any error occurred and fails if it happens
        end
    end

    xmlDelete(doc);

endfunction
