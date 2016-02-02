// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Scilab Enterprises
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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
