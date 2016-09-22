// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA - Pierre MARECHAL
// Copyright (C) 2009-2011 - DIGITEO - Michael Baudin
// Copyright (C) 2010-2012 - DIGITEO - Antoine ELIAS
// Copyright (C) 2011 - DIGITEO - Allan CORNET
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

function test_run_result = test_run(varargin)
    lhs     = argn(1);
    rhs     = argn(2);
    test_run_result = %f;
    // =========================================================================
    // Print test_run help
    // =========================================================================
    if (rhs >= 3) & (~ isempty(grep(varargin(3),"help"))) then
        example = test_examples();
        printf("%s\n",example);
        return;
    end

    status.detailled_failures     = "";
    status.testsuites             = [];
    status.test_count             = 0;
    status.test_passed_count      = 0;
    status.test_failed_count      = 0;
    status.test_skipped_count     = 0;
    status.totalTime              = 0;
    status.list                   = [];

    params.longtime               = %f;
    params.wanted_mode            = "";
    params.error_output           = "check";
    params.reference              = "check";
    params.testTypes              = "all_tests";
    params.skip_mat               = [];
    params.tests_mat              = [];
    params.test_passed_percent    = 0;
    params.test_failed_percent    = 0;
    params.test_skipped_percent   = 0;
    params.full_summary           = %t;
    params.show_diff              = %f;
    params.show_error             = %f;

    // =======================================================
    // Management of the kind of tests to run and the options
    // =======================================================
    if rhs >= 3 then

        option_mat = varargin(3);
        if (option_mat == "[]")
            option_mat = [];
        end
        if (check_option(option_mat, "unit_tests") & check_option(option_mat, "nonreg_tests")) | check_option(option_mat, "all_tests") then
            params.testTypes = "all_tests";
        elseif check_option(option_mat, "unit_tests") then
            params.testTypes = "unit_tests";
        elseif check_option(option_mat, "nonreg_tests") then
            params.testTypes = "nonreg_tests";
        end

        option_mat = clean_option(option_mat, "unit_tests");
        option_mat = clean_option(option_mat, "nonreg_tests");
        option_mat = clean_option(option_mat, "all_tests");

        // Skip tests
        params.skip_mat     = assign_option(option_mat, "skip_tests", varargin(2), params.skip_mat);
        option_mat          = clean_option(option_mat, "skip_tests");

        // Mode
        params.wanted_mode  = assign_option(option_mat, "mode_nw", "NW", params.wanted_mode);
        option_mat          = clean_option(option_mat, "mode_nw");

        params.wanted_mode  = assign_option(option_mat, "mode_nwni", "NWNI", params.wanted_mode);
        option_mat          = clean_option(option_mat, "mode_nwni");

        params.wanted_mode  = assign_option(option_mat, "mode_nwni_profiling", ["NWNI" "PROFILING"], params.wanted_mode);
        option_mat          = clean_option(option_mat, "mode_nwni_profiling");

        // Reference
        params.reference    = assign_option(option_mat, "no_check_ref", "skip", params.reference);
        option_mat          = clean_option(option_mat, "no_check_ref");

        params.reference    = assign_option(option_mat, "create_ref", "create", params.reference);
        option_mat          = clean_option(option_mat, "create_ref");

        params.reference    = assign_option(option_mat, "list", "list", params.reference);
        option_mat          = clean_option(option_mat, "list");

        // Error Output
        params.error_output = assign_option(option_mat, "no_check_error_output", "skip", params.error_output);
        option_mat          = clean_option(option_mat, "no_check_error_output");

        // Enable long-time tests
        params.longtime     = assign_option(option_mat, "disable_lt", %f, params.longtime);
        option_mat          = clean_option(option_mat, "disable_lt");

        params.longtime     = assign_option(option_mat, "enable_lt", %t, params.longtime);
        option_mat          = clean_option(option_mat, "enable_lt");

        // Summary display management
        params.full_summary = assign_option(option_mat, "short_summary", %f, params.full_summary);
        option_mat          = clean_option(option_mat, "short_summary");

        params.show_diff = assign_option(option_mat, "show_diff", %t, params.show_diff);
        option_mat          = clean_option(option_mat, "show_diff");

        params.show_error = assign_option(option_mat, "show_error", %t, params.show_error);
        option_mat          = clean_option(option_mat, "show_error");

        if option_mat <> [] then
            printf("\nUnrecognized option(s): \n\n");
            for i=1:size(option_mat, "*")
                printf("  - ""%s""\n", option_mat(i));
            end

            return;
        end

        if rhs == 4 then
            params.exportFile = varargin(4);
            // Doing the XML export, force the display of the error and diff
            params.show_diff = %t;
            params.show_error = %t;
        end
    end


    if params.reference <> "list" & params.full_summary then
        printf("   TMPDIR = %s\n",TMPDIR);
        printf("\n");
    end
    // =======================================================
    // Management of the tests to run
    // =======================================================
    if (rhs == 0) ..
        | ((rhs == 1) & (varargin(1)==[] | varargin(1)=="[]")) ..
        | (rhs >= 2 & rhs <= 4) & ( varargin(1)==[]|varargin(1)=="[]") & (varargin(2)==[]|varargin(2)=="[]") then


        // No input argument
        // test_run()
        // test_run([])
        // => Launch each test of each module

        module_list = getmodules();
        module_list = gsort(module_list,"lr","i");

        test_count = size(module_list,"*");
        for i=1:test_count
            if params.reference <> "list" then
                printf("   %03d/%03d - [%s] : \n\n", i, test_count, module_list(i));
            end

            params.tests_mat    = [];
            params.moduleName   =  module_list(i);

            result              = test_module(params);

            if params.reference <> "list" then
                status.detailled_failures   = [status.detailled_failures; result.detailled_failures];
                status.testsuites(size(status.testsuites,"*")+1) = result.testsuite
                status.test_skipped_count   = status.test_skipped_count + result.test_skipped_count;

                // Do not take in account skipped tests
                status.test_count           = status.test_count + result.test_count - status.test_skipped_count;
                status.test_passed_count    = status.test_passed_count + result.test_passed_count;
                status.test_failed_count    = status.test_failed_count + result.test_failed_count;
                status.totalTime            = status.totalTime + result.totalTime;
                printf("\n");
            else
                status.test_count           = status.test_count + result.test_count;
                status.list                 = [status.list; result.list];
            end
        end

    elseif (rhs == 1) ..
        | ((rhs == 2) & (varargin(2)==[] || varargin(2)=="[]")) ..
        | ((rhs == 3) & (varargin(2)==[] || varargin(2)=="[]")) ..
        | ((rhs == 4) & (varargin(2)==[] || varargin(2)=="[]")) ..
        | ( ~ isempty(params.skip_mat)) then

        // One input argument
        // test_run(<module_name>)
        // test_run([<module_name_1>,<module_name_2>])
        // varargin(1) = [<module_name_1>,<module_name_2>]

        module_mat = varargin(1);

        test_count = size(module_mat,"*");
        for i = 1:test_count
            if params.reference <> "list" then
                printf("   %03d/%03d - [%s] : \n\n", i, test_count, module_mat(i));
            end

            params.tests_mat    = [];
            params.moduleName   =  module_mat(i);

            result              = test_module(params);

            status.test_count   = status.test_count + result.test_count;
            if params.reference <> "list" then
                status.detailled_failures       = [status.detailled_failures; result.detailled_failures];
                status.testsuites(size(status.testsuites,"*")+1) = result.testsuite

                status.test_skipped_count   = status.test_skipped_count + result.test_skipped_count;
                status.test_passed_count    = status.test_passed_count + result.test_passed_count;
                status.test_failed_count    = status.test_failed_count + result.test_failed_count;
                status.totalTime            = status.totalTime + result.totalTime;

                printf("\n");
            else
                status.test_count           = status.test_count + result.test_count;
                status.list                 = [status.list; result.list];
            end
        end
        // Do not take in account skipped tests
        status.test_count = status.test_count - status.test_skipped_count;
    elseif or(rhs==[2 3 4]) then
        // Two input arguments
        // test_run(<module_name>,<test_name>)
        // test_run(<module_name>,[<test_name_1>,<test_name_2>] )

        // varargin(1) = <module_name> ==> string 1x1
        // varargin(2) = <test_name_1> ==> mat nl x nc

        params.tests_mat    = varargin(2);
        params.moduleName   = varargin(1);

        if ((or(size(params.moduleName) <> [1,1])) & (params.tests_mat <> [])) then
            example = test_examples();
            err   = ["" ; msprintf(gettext("%s: Wrong size for input argument."),"test_run") ; "" ; example ];
            printf("%s\n",err);
            return;
        end

        result = test_module(params);

        if params.reference <> "list" then
            status.totalTime            = result.totalTime;
            status.detailled_failures   = [status.detailled_failures; result.detailled_failures];
            status.testsuites(size(status.testsuites,"*")+1) = result.testsuite
            status.test_skipped_count   = status.test_skipped_count + result.test_skipped_count;

            // Do not take in account skipped tests
            status.test_count           = status.test_count + result.test_count - status.test_skipped_count;
            status.test_passed_count    = status.test_passed_count + result.test_passed_count;
            status.test_failed_count    = status.test_failed_count + result.test_failed_count;
        else
            status.test_count           = status.test_count + result.test_count;
            status.list                 = [status.list; result.list];
        end
    else
        error(msprintf(gettext("%s: Wrong number of input arguments."),"test_run"));
    end

    if params.reference == "list" then
        test_count = size(status.list, "r");
        for i=1:test_count
            printf("   %4.d - [%s] %s\n", i, status.list(i, 1), status.list(i,2));
        end
        return;
    end

    // percent computation
    if status.test_count <> 0 then
        test_passed_percent  = status.test_passed_count  / status.test_count * 100;
        test_failed_percent  = status.test_failed_count  / status.test_count * 100;
    else
        test_passed_percent  = 0;
        test_failed_percent  = 0;
    end

    if isfield(params, "exportFile") then
        exportToXUnitFormat(params.exportFile, status.testsuites);
    end

    if params.full_summary then
        printf("\n");
        printf("   --------------------------------------------------------------------------\n");
        printf("   Summary\n\n");
        printf("   tests           %4d - 100 %%\n", status.test_count);
        printf("   passed          %4d - %3d %%\n", status.test_passed_count, test_passed_percent);
        printf("   failed          %4d - %3d %%\n", status.test_failed_count, test_failed_percent);
        printf("   skipped         %4d\n", status.test_skipped_count);
        printf("   length             %4.2f sec\n", status.totalTime);
        printf("   --------------------------------------------------------------------------\n");

        if isfield(params, "exportFile") then
            printf("   Export to          %s\n", params.exportFile);
            printf("   --------------------------------------------------------------------------\n");
        end

        if status.test_failed_count > 0 then
            printf("   Details\n\n");
            printf("%s\n",status.detailled_failures);
            printf("\n");
            printf("   --------------------------------------------------------------------------\n");
        end
    else
        printf("\n");
        printf("   --------------------------------------------------------------------------\n");
        printf("   Tests: %4d, ", status.test_count);
        printf("   Passed: %4d, ", status.test_passed_count);
        printf("   Failed: %4d, ", status.test_failed_count);
        printf("   Skipped: %4d\n", status.test_skipped_count);
        printf("   --------------------------------------------------------------------------\n");
    end

    //   Returns %t if no error has been detected
    //   Returns %f if any error has been detected
    test_run_result = (status.test_failed_count == 0);

endfunction



function status = test_module(_params)
    name = splitModule(_params.moduleName);

    if with_module(name(1)) then
        // It's a scilab internal module
        module.path = pathconvert(SCI + "/modules/" + name(1), %F);
    elseif or(librarieslist() == "atomslib") & atomsIsLoaded(name(1)) then
        // It's an ATOMS module
        module.path = pathconvert(atomsGetLoadedPath(name(1)) , %F, %T);
    elseif isdir(name(1)) then
        // It's an external module
        module.path = pathconvert(name(1), %F);
    else
        // It's an error
        error(sprintf(gettext("%s is not an installed module or toolbox"), name(1)));
    end

    //get tests from path
    my_types = ["unit_tests","nonreg_tests"];

    directories = [];
    for i=1:size(my_types,"*")
        if (_params.testTypes == "all_tests") | (_params.testTypes == my_types(i)) then
            directory_path = module.path + "/tests/" + my_types(i);
            for j=2:size(name,"*")
                directory_path = directory_path + filesep() + name(j);
            end

            if isdir(directory_path) then
                directories = [directories;getDirectories(directory_path + filesep())];
            end
        end
    end

    tests = [];
    if( _params.tests_mat == [])
        for i=1:size(directories, "*")
            currentDir = directories(i);
            tests_mat = gsort(basename(listfiles(currentDir + filesep() + "*.tst")),"lr","i");

            for j = 1:size(tests_mat, "*")
                if or(tests_mat(j) == _params.skip_mat) == %f then
                    tests($+1, [1,2]) = [currentDir, tests_mat(j)];
                end
            end
        end
    else
        //not empty tests_mat
        for i = 1:size(_params.tests_mat, "*")
            bFind = %f;
            for j = 1:size(directories, "*")
                currentDir = directories(j);
                testFile = currentDir + filesep() + _params.tests_mat(i) + ".tst";
                if isfile(testFile) then
                    tests($+1, [1,2]) = [currentDir, _params.tests_mat(i)];
                    bFind = %t;
                end
            end

            if bFind == %f then
                error(sprintf(gettext("The test ""%s"" is not available from the ""%s"" module"), _params.tests_mat(i), name(1)));
            end
        end
    end

    //initialize counter
    detailled_failures      = "";
    test_count          = size(tests, "r");
    test_passed_count   = 0;
    test_failed_count   = 0;
    test_skipped_count  = 0;

    moduleName          = _params.moduleName;
    // Improve the display of the module
    if isdir(moduleName) then
        if part(moduleName,1:length(SCI)) == SCI then
            moduleName = "SCI" + part(moduleName,length(SCI)+1:length(moduleName));
        elseif part(moduleName,1:length(SCIHOME)) == SCIHOME then
            moduleName = "SCIHOME" + part(moduleName,length(SCIHOME)+1:length(moduleName));
        end
    end

    // For the XML export
    testsuite.name=moduleName
    testsuite.time=0
    testsuite.tests=0
    testsuite.errors=0

    //don't test only return list of tests.
    if _params.reference == "list" then
        for i = 1:test_count
            if size(name, "*") > 1 then
                displayModuleName = sprintf("%s", name(1));
                for j=2:size(name, "*")
                    displayModuleName = displayModuleName + sprintf("|%s", name(j));
                end
            else
                displayModuleName = sprintf("%s", name(1));
            end
            tests(i,1) = displayModuleName;
        end
        status.list         = tests;
        status.test_count   = test_count;
        return;
    end

    tic();
    for i = 1:test_count
        printf("   %03d/%03d - ",i, test_count);

        if size(name, "*") > 1 then
            displayModuleName = sprintf("[%s", name(1));
            for j=2:size(name, "*")
                displayModuleName = displayModuleName + sprintf("|%s", name(j));
            end
            displayModuleName = displayModuleName + sprintf("] %s", tests(i,2));
        else
            displayModuleName = sprintf("[%s] %s", name(1), tests(i,2));
        end

        printf("%s", displayModuleName);
        if length(displayModuleName) >= 50 then
            printf(" ");
        end
        for j = length(displayModuleName):50
            printf(".");
        end

        elapsedTimeBefore=toc();
        result = test_single(_params, tests(i,1), tests(i,2));
        elapsedTimeAfter=toc();


        testsuite.tests = testsuite.tests + 1

        testsuite.testcase(i).name=tests(i,2);
        testsuite.testcase(i).time=elapsedTimeAfter-elapsedTimeBefore;
        testsuite.testcase(i).skipped=(result.id >= 10) & (result.id < 20);

        if result.id == 0 then
            printf("passed\n");
            test_passed_count = test_passed_count + 1;
        else
            msg = sprintf(result.message);
            printf("%s \n", msg(1));
            for kline = 2:size(msg, "*")
                printf(part(" ", 1:62) + "%s \n", msg(2));
            end

            if result.id < 10 then
                //failed
                test_failed_count = test_failed_count + 1;
                detailled_failures = [ detailled_failures ; sprintf("   TEST : [%s] %s", _params.moduleName, tests(i,2))];
                detailled_failures = [ detailled_failures ; sprintf("   %s", result.message) ];
                detailled_failures = [ detailled_failures ; result.details ];
                detailled_failures = [ detailled_failures ; "" ];

                testsuite.errors = testsuite.errors + 1
                testsuite.testcase(i).failure.type=result.message
                testsuite.testcase(i).failure.content=result.details

            elseif (result.id >= 10) & (result.id < 20) then
                // skipped
                test_skipped_count = test_skipped_count + 1;
            end
        end

        if ~isempty(result.warning) then
            warning(result.warning);
        end
    end

    status.totalTime = toc();

    testsuite.time=status.totalTime;

    clearglobal TICTOC;
    status.test_passed_count  = test_passed_count;
    status.test_failed_count  = test_failed_count;
    status.test_skipped_count = test_skipped_count;

    // Summary
    status.test_count     = test_count;
    status.detailled_failures   = detailled_failures;
    status.testsuite   = testsuite;
endfunction

function status = test_single(_module, _testPath, _testName)
    //option flag

    skip          = %F;
    interactive   = %F;
    notyetfixed   = %F;
    longtime      = %F;
    reopened      = %F;
    jvm           = %T;
    graphic       = %F;
    mpi           = %F;
    execMode      = "";
    platform      = "all";
    language      = "any";
    //try_catch     = %T; // Scilab 5.4.0
    try_catch     = %f; // see comment about "dia(find(dia == '')) = [];" (~line 890)
    error_output  = "check";
    reference     = "check";
    xcosNeeded    = %F;

    //some paths
    tmp_tst     = pathconvert( TMPDIR + "/" + _testName + ".tst", %F);
    tmp_dia     = pathconvert( TMPDIR + "/" + _testName + ".dia.tmp", %F);
    tmp_res     = pathconvert( TMPDIR + "/" + _testName + ".res", %F);
    tmp_err     = pathconvert( TMPDIR + "/" + _testName + ".err", %F);
    path_dia    = pathconvert( TMPDIR + "/" + _testName + ".dia", %F);
    tmp_prof    = pathconvert( TMPDIR + "/" + _testName + ".prof", %F);

    path_dia_ref  = _testPath + _testName + ".dia.ref";
    // Reference file management OS by OS
    if getos() == "Windows" then
        [branch info] = getversion();
        if info(2) == "x86" then // Look for a 32bits-specific reference file
            altreffile  = [ _testPath + _testName + ".win.dia.ref" ; _testPath + _testName + ".win32.dia.ref" ];
        else
            altreffile  = [ _testPath + _testName + ".win.dia.ref" ];
        end
    elseif getos() == "Darwin" then
        altreffile  = [ _testPath + _testName + ".unix.dia.ref" ; _testPath + _testName + ".macosx.dia.ref" ];
    elseif getos() == "Linux" then
        [branch info] = getversion();
        if info(2) == "x86" then // Look for a 32bits-specific reference file
            altreffile  = [ _testPath + _testName + ".unix.dia.ref" ; _testPath + _testName + ".linux.dia.ref" ; _testPath + _testName + ".linux32.dia.ref" ];
        else
            altreffile  = [ _testPath + _testName + ".unix.dia.ref" ; _testPath + _testName + ".linux.dia.ref" ];
        end
    else
        altreffile  = [ _testPath + _testName + ".unix.dia.ref" ];
    end

    for i=1:size(altreffile,"*")
        if isfile(altreffile(i)) then
            path_dia_ref = altreffile(i);
        end
    end

    //output structure
    status.id = 0;
    status.message = "";
    status.details = "";
    status.warning = "";

    //Reset standard globals
    rand("seed",0);
    rand("uniform");

    //load file
    testFile = _testPath + _testName + ".tst";
    sciFile = mgetl(testFile);

    //read options
    if ~isempty(grep(sciFile, "<-- NOT FIXED -->")) then
        status.id = 10;
        status.message = "skipped: not yet fixed";
        return;
    end

    if ~isempty(grep(sciFile, "<-- REOPENED -->")) then
        status.id = 10;
        status.message = "skipped: Bug reopened";
        return;
    end

    // platform
    if ~isempty(grep(sciFile, "<-- WINDOWS ONLY -->")) & getos() <> "Windows" then
        status.id = 10;
        status.message = "skipped: Windows only";
        return;
    end

    if ~isempty(grep(sciFile, "<-- UNIX ONLY -->")) & getos() == "Windows" then
        status.id = 10;
        status.message = "skipped: Unix only";
        return;
    end

    if ~isempty(grep(sciFile, "<-- LINUX ONLY -->")) & getos() <> "Linux" then
        status.id = 10;
        status.message = "skipped: Linux only";
        return;
    end

    if ~isempty(grep(sciFile, "<-- MACOSX ONLY -->")) & getos() <> "Darwin" then
        status.id = 10;
        status.message = "skipped: MacOSX only";
        return;
    end

    // Test execution
    if ~isempty(grep(sciFile, "<-- INTERACTIVE TEST -->")) then
        status.id = 10;
        status.message = "skipped: interactive test";
        return;
    end

    if ~isempty(grep(sciFile, "<-- LONG TIME EXECUTION -->")) & ~_module.longtime then
        status.id = 10;
        status.message = "skipped: Long time duration";
        return;
    end

    if ~isempty(grep(sciFile, "<-- TEST WITH GRAPHIC -->")) then
        if or(_module.wanted_mode == "NWNI") then
            status.id = 10;
            status.message = "skipped: Test with graphic";
            return;
        end

        graphic = %T;
        jvm = %T;
        execMode = "NW";
    end

    if or(_module.wanted_mode == "NWNI") & isempty(grep(sciFile, "<-- CLI SHELL MODE -->")) then
        status.id = 10;
        status.message = "skipped: not CLI SHELL MODE test";
        return;
    end

    if ~isempty(grep(sciFile, "<-- JVM NOT MANDATORY -->")) then
        status.warning = _("option ""JVM NOT MANDATORY"" is deprecated, please use ""CLI SHELL MODE"" instead");
        jvm = %F;
        execMode = "NWNI";
    end

    if ~isempty(grep(sciFile, "<-- CLI SHELL MODE -->")) then
        jvm = %F;
        execMode = "NWNI";
    end

    MPITestPos=grep(sciFile, "<-- MPI TEST")
    if ~isempty(MPITestPos) then
        mpi_node=msscanf(sciFile(MPITestPos), "// <-- MPI TEST %d -->")
        if mpi_node == [] then
            // No node found ? No worries, default to 2
            mpi_node = 2
        end
        mpi = %t;
        execMode = "NWNI";
        reference = "skip";
    end
    clear MPITestPos

    if ~isempty(grep(sciFile, "<-- XCOS TEST -->")) then
        if or(_module.wanted_mode == "NWNI") then
            status.id = 10;
            status.message = "skipped: Test with xcos";
            return;
        end
        xcosNeeded = %T;
        jvm = %T;
    end

    // Language
    if ~isempty(grep(sciFile, "<-- FRENCH IMPOSED -->")) then
        language = "fr_FR";
    end


    if ~isempty(grep(sciFile, "<-- ENGLISH IMPOSED -->")) then
        language = "en_US";
    end

    // Test building
    if ~isempty(grep(sciFile, "<-- NO TRY CATCH -->")) then
        try_catch = %F;
    end

    // Test result
    if ~isempty(grep(sciFile, "<-- NO CHECK ERROR OUTPUT -->")) then
        error_output = "skip";
    end

    if ~isempty(grep(sciFile, "<-- NO CHECK REF -->")) then
        reference = "skip";
    end

    //build real test file

    // Do some modification in tst file
    //replace "pause,end" by "bugmes();quit;end"
    sciFile = strsubst(sciFile, "pause,end", "bugmes();quit;end");
    sciFile = strsubst(sciFile, "pause, end", "bugmes();quit;end");
    sciFile = strsubst(sciFile, "pause;end", "bugmes();quit;end");
    sciFile = strsubst(sciFile, "pause; end", "bugmes();quit;end");

    //to avoid suppression of input --> with prompts
    sciFile = strsubst(sciFile, "--> ", "@#> ");
    //remove halt calls
    sciFile = strsubst(sciFile, "halt();", "");

    // Build test header
    head = [
    "// <-- HEADER START -->";
    "mode(3);" ;
    "lines(28,72);";
    "lines(0);" ;
    "function []=bugmes(), printf(''error on test'');endfunction"
    "function %onprompt" ;
    "   [msg, num] = lasterror();" ;
    "   if (num <> 0) then" ;
    "       bugmes()" ;
    "   end" ;
    "   quit;" ;
    "endfunction"];
    if ~interactive then
        head($+1) = "function []=messagebox(msg, msg_title, info), disp(''messagebox: '' + msg);endfunction";
    end
    head = [ head ;
    "predef(''all'');";
    "tmpdirToPrint = msprintf(''TMPDIR1=''''%s'''';//\n'',TMPDIR);"
    ];

    if xcosNeeded then
        head = [
        head;
        "prot=funcprot(); funcprot(0);";
        "loadXcosLibs(); loadScicos();";
        "funcprot(prot);";
        ];
    end

    if try_catch then
        head = [ head ; "try" ];
    end

    head = [
    head;
    "diary(''" + tmp_dia + "'');";
    "printf(''%s\n'',tmpdirToPrint);";
    "// <-- HEADER END -->"
    ];

    // Build test footer
    tail = [ "// <-- FOOTER START -->" ];

    if try_catch then
        tail = [
        tail;
        "catch";
        "   errmsg = ""<--""+""Error on the test script file""+""-->"";";
        "   printf(""%s\n"",errmsg);";
        "   lasterror()";
        "end";
        ];
    end

    tail = [ tail; "diary(0);" ];

    if graphic then
        tail = [ tail; "xdel(winsid());sleep(1000);" ];
    end

    tail = [ tail; "exit(0);" ; "// <-- FOOTER END -->" ];

    //Build final test
    sciFile = [head ; sciFile ; tail];


    //Build command to execute

    //scilab path
    if (getos() <> "Windows") & ~isfile(SCI+"/bin/scilab") then
        SCI_BIN = strsubst(SCI,"share/scilab","");
    else
        SCI_BIN = SCI;
    end

    //mode
    valgrind_opt = "";
    winbin = "wscilex.exe";
    if _module.wanted_mode == "NW" then
        mode_arg = "-nw";
    elseif _module.wanted_mode == "NWNI" then
        winbin = "scilex.exe";
        mode_arg = "-nwni";
    elseif _module.wanted_mode == ["NWNI" "PROFILING"] && getos() == "Linux" then
        mode_arg = "-nwni -profiling";
        valgrind_opt = "SCILAB_VALGRIND_OPT=""--log-file=" + tmp_prof + " """;
    else
        if execMode == "NWNI" then
            winbin = "scilex.exe";
            mode_arg = "-nwni";
        elseif execMode == "NW" then
            mode_arg = "-nw";
        else
            mode_arg = "-nw";
        end
    end

    if mpi == %t then
        prefix_bin="mpirun -c " + string(mpi_node) + "  -bynode"
    else
        prefix_bin=""
    end


    //language
    if language == "any" then
        language_arg = "";
    else
        language_arg = "-l "+ language;
    end

    loader_path = pathconvert(fullfile(_module.moduleName, "loader.sce"), %f);

    SCI_ARGS = " -nb -quit "
    if ~_module.longtime then
        SCI_ARGS = SCI_ARGS + "--timeout 15m "
    end

    // Build final command
    if getos() == "Windows" then
        if (isdir(_module.moduleName) & isfile(loader_path)) // external module not in Scilab
            test_cmd = "( """ + SCI_BIN + "\bin\" + winbin + """" + " " + mode_arg + " " + language_arg + SCI_ARGS + "-e ""exec(""""" + loader_path + """"");exec(""""" + tmp_tst + """"", -1);"" > """ + tmp_res + """ ) 2> """ + tmp_err + """";
        else // standard module
            test_cmd = "( """ + SCI_BIN + "\bin\" + winbin + """" + " " + mode_arg + " " + language_arg + SCI_ARGS + "-e ""exec(""""" + tmp_tst + """"", -1);"" > """ + tmp_res + """ ) 2> """ + tmp_err + """";
        end
    else
        if (isdir(_module.moduleName) & isfile(loader_path))
            test_cmd = "( " + valgrind_opt + " " + SCI_BIN + "/bin/scilab " + mode_arg + " " + language_arg + SCI_ARGS + "-e ""exec(''" + loader_path + "'');exec(''" + tmp_tst +"'');""" + " > " + tmp_res + " ) 2> " + tmp_err;
        else
            test_cmd = "( " + valgrind_opt + " " + prefix_bin + " " + SCI_BIN + "/bin/scilab " + mode_arg + " " + language_arg + SCI_ARGS + " -f " + tmp_tst + " > " + tmp_res + " ) 2> " + tmp_err;
        end
    end

    //clean previous tmp files
    if isfile(tmp_tst) then
        deletefile(tmp_tst);
    end

    if isfile(tmp_dia) then
        deletefile(tmp_dia);
    end

    if isfile(tmp_res) then
        deletefile(tmp_res);
    end

    if isfile(tmp_err) then
        deletefile(tmp_err);
    end

    //create tmp test file
    mputl(sciFile, tmp_tst);

    //execute test
    returnStatus = host(test_cmd);
    //Check return status
    if (returnStatus <> 0)
        status.id = 5;
        status.message = "failed: Slave Scilab exited with error code " + string(returnStatus);
        return;
    end

    //Check errors
    if (error_output == "check") & (_module.error_output == "check") then
        if getos() == "Darwin" then
            tmp_errfile_info = fileinfo(tmp_err);
            msg = "Picked up _JAVA_OPTIONS:"; // When -Djava.awt.headless=false is forced for example

            if ~isempty(tmp_errfile_info) then
                txt = mgetl(tmp_err);
                toRemove = grep(txt, msg);
                txt(toRemove) = [];
                if isempty(txt) then
                    deletefile(tmp_err);
                else // Remove messages due to JOGL2 RC8
                    toRemove = grep(txt, "__NSAutoreleaseNoPool()");
                    txt(toRemove) = [];
                    if isempty(txt) then
                        deletefile(tmp_err);
                    end
                end
            end
        end

        if getos() == "Linux" then // Ignore JOGL2 debug message
            tmp_errfile_info = fileinfo(tmp_err);
            msg = "Error: unable to open display (null)"

            if ~isempty(tmp_errfile_info) then
                txt = mgetl(tmp_err);
                txt(txt==msg) = [];

                // Remove messages due to warning message from external
                // libraries

                if ~isempty(txt) then
                    // MESA / EGL display some warning on stderr
                    toRemove = grep(txt, "libEGL warning:");
                    txt(toRemove) = [];
                end

                if ~isempty(txt) then
                    toRemove = grep(txt, "extension ""RANDR"" missing on display");
                    txt(toRemove) = [];
                end

                if isempty(txt) then
                    deletefile(tmp_err);
                end
            end
        end

        if getos() == "Windows" then // Ignore JOGL 2.2.4 debug message
            tmp_errfile_info = fileinfo(tmp_err);
            msg = "Info: GLReadBufferUtil.readPixels: pre-exisiting GL error 0x500";

            if ~isempty(tmp_errfile_info) then
                txt = mgetl(tmp_err);
                txt(txt==msg) = [];
                if isempty(txt) then
                    deletefile(tmp_err);
                else // Ignore JOGL 2.1.4 debug message
                    msg = "Info: GLDrawableHelper.reshape: pre-exisiting GL error 0x500";
                    txt(txt==msg) = [];
                    if isempty(txt) then
                        deletefile(tmp_err);
                    end
                end
            end
        end

        if isfile(tmp_prof) then
            txt = mgetl(tmp_prof);
            if grep(txt($), "ERROR SUMMARY: 0 errors from 0 contexts") then
                deletefile(tmp_prof);
            else
                status.id = 5;
                status.message = "failed: Valgrind error detected";
                status.details = checkthefile(tmp_prof);
                return;
            end
        end


        tmp_errfile_info = fileinfo(tmp_err);

        if isfile(tmp_err) & tmp_errfile_info(1) <> 0 then
            status.id = 5;
            status.message = "failed: error_output not empty\n   Use ''no_check_error_output'' option to disable this check.";
            status.details = checkthefile(tmp_err);
            return;
        end
    end

    //Process output files

    //Get the dia file
    if isfile(tmp_dia) then
        dia = mgetl(tmp_dia);
    else
        status.id = 6;
        status.message = "failed: Cannot find the dia file: " + tmp_dia + "\nCheck if the Scilab used correctly starts";
        status.details = checkthefile(tmp_dia);
        return;
    end

    // To get TMPDIR value
    tmpdir1_line = grep(dia, "TMPDIR1");
    execstr(dia(tmpdir1_line));

    //Check for execution errors
    if try_catch & grep(dia,"<--Error on the test script file-->") <> [] then
        details = [ checkthefile(tmp_dia); ..
        launchthecommand(testFile)];
        status.id = 3;
        status.message = "failed: premature end of the test script";
        status.details = details;
        if params.show_error == %t then
            status.details = [ status.details; dia($-10:$) ]
        end

        return;
    end

    // Remove Header and Footer
    dia = remove_headers(dia);

    // Remove empty lines
    // In scilab 5, the test is executed in a try/catch
    // which remove empty lines.
    // In scilab 6, we can't execute the test in a try/catch
    // because it will be parsed first then executed
    // so the diary will contain all the script followed by the display
    // of the execution.
    // The try/catch is desactived ~line 513 by "try_catch     = %f;"
    // and the following line remove empty lines to reproduce the old operation.
    dia(find(dia == "")) = [];

    //Check for execution errors
    dia_tmp = dia;

    // remove commented lines
    dia_tmp(grep(dia_tmp, "//")) = [];

    if try_catch & grep(dia_tmp, "!--error") <> [] then
        details = [ checkthefile(tmp_dia); ..
        launchthecommand(testFile) ];
        status.id = 1;
        status.message = "failed: the string (!--error) has been detected";
        status.details = details;
        return;
    end


    if grep(dia_tmp,"error on test")<>[] then
        details = [ checkthefile(tmp_dia); ..
        launchthecommand(testFile) ];
        status.id = 2;
        status.message = "failed: one or several tests failed";
        status.details = details;
        if params.show_error == %t then
            status.details = [ status.details; dia($-min(10, size(dia, "*")-1):$) ]
        end
        return;
    end


    if tmpdir1_line == [] then
        status.id = 6;
        status.message = "failed: the dia file is not correct";
        status.details = checkthefile(tmp_dia);
        return;
    end


    // Check the reference file only if check_ref (i.e. for the whole
    // test sequence) is true and this_check_ref (i.e. for the specific current .tst)
    // is true.

    if (reference=="check") & (_module.reference=="check")  then
        if isfile(path_dia_ref) == %f then
            status.id = 5;
            status.message = "failed: the ref file doesn''t exist\n   Use ''no_check_ref'' option to disable this check.";
            status.details = createthefile(path_dia_ref);
            return;
        end
    end

    // Comparaison ref <--> dia

    if ( (reference=="check") & (_module.reference=="check") ) | (_module.reference=="create") then
        //  Do some modification in  dia file

        dia(grep(dia, "printf(''%s\n'',tmpdirToPrint);")) = [];
        dia(grep(dia, "TMPDIR1")) = [];
        dia(grep(dia, "diary(0)")) = [];

        if getos() == "Darwin" then // TMPDIR is a symblic link
            dia = strsubst(dia,"/private" + TMPDIR1, "TMPDIR");
            dia = strsubst(dia,"/private" + TMPDIR, "TMPDIR");
        end
        dia = strsubst(dia,TMPDIR ,"TMPDIR");
        dia = strsubst(dia,TMPDIR1, "TMPDIR");

        if getos() == "Windows" then
            dia = strsubst(dia, strsubst(TMPDIR, "\","/"), "TMPDIR");
            dia = strsubst(dia, strsubst(TMPDIR1, "\","/"), "TMPDIR");
            dia = strsubst(dia, strsubst(TMPDIR, "/","\"), "TMPDIR");
            dia = strsubst(dia, strsubst(TMPDIR1, "/","\"), "TMPDIR");
            dia = strsubst(dia, strsubst(getshortpathname(TMPDIR), "\","/"), "TMPDIR");
            dia = strsubst(dia, strsubst(getshortpathname(TMPDIR1), "\","/"), "TMPDIR");
            dia = strsubst(dia, getshortpathname(TMPDIR), "TMPDIR");
            dia = strsubst(dia, getshortpathname(TMPDIR1), "TMPDIR");
        end

        dia = strsubst(dia, SCI, "SCI");

        if getos() == "Windows" then
            dia = strsubst(dia, strsubst(SCI, "\","/"), "SCI");
            dia = strsubst(dia, strsubst(SCI, "/","\"), "SCI");
            dia = strsubst(dia, strsubst(getshortpathname(SCI), "\","/"), "SCI");
            dia = strsubst(dia, getshortpathname(SCI), "SCI");
        end

        //suppress the prompts
        dia = strsubst(dia, "--> ", "");
        dia = strsubst(dia, "@#> ", "--> ");
        dia = strsubst(dia, "-1-> ", "");

        //standardise  number display

        // strsubst(dia, " .", "0.");
        // strsubst(dia, "-.", "-0.")
        // strsubst(dia, "E+", "D+");
        // strsubst(dia, "E-", "D-");

        //not to change the ref files
        dia = strsubst(dia ,"bugmes();return", "bugmes();quit");

        if _module.reference=="create" then
            // Delete previous .dia.ref file
            if isfile(path_dia_ref) then
                deletefile(path_dia_ref)
            end

            mputl(dia, path_dia_ref);
            status.id = 20;
            status.message = "passed: ref created";
            return;
        else
            // write down the resulting dia file
            mputl(dia, path_dia);

            //Check for diff with the .ref file
            [u,ierr] = mopen(path_dia_ref, "r");
            if ierr== 0 then //ref file exists
                ref=mgetl(u);
                mclose(u)

                // suppress blank (diff -nw)
                dia = strsubst(dia, " ", "")
                ref = strsubst(ref, " ", "")

                dia(find(dia == "")) = [];
                ref(find(ref == "")) = [];

                dia(find(dia == "")) = [];
                ref(find(ref == "")) = [];

                dia(find(part(dia, (1:2)) == "//")) = [];
                ref(find(part(ref, (1:2)) == "//")) = [];

                if or(ref <> dia) then
                    status.id = 4;
                    status.message = "failed: dia and ref are not equal";
                    status.details = comparethefiles(path_dia, path_dia_ref);
                    return;
                end

            else
                error(sprintf(gettext("The ref file (%s) doesn''t exist"), path_dia_ref));
            end
        end
    end
endfunction

// checkthefile
function msg = checkthefile( filename )
    // Returns a 2-by-1 matrix of strings, containing a message such as:
    //   Check the following file :
    //   - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.tst
    // Workaround for bug #4827
    msg(1) = "   Check the following file :"
    msg(2) = "   - "+filename
    if params.show_error == %t then
        msg=[msg; mgetl(filename)]
    end

endfunction

// launchthecommand
function msg = launchthecommand( filename )
    // Returns a 2-by-1 matrix of strings, containing a message such as:
    //   Or launch the following command :
    //   - exec("C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.tst")
    // Workaround for bug #4827
    msg(1) = "   Or launch the following command :"
    msg(2) = "   - exec(""" + fullpath(filename) + """);"
endfunction

// => remove header from the diary txt
function dia_out = remove_headers(dia_in)
    dia_out = dia_in;
    body_start = grep(dia_out,"// <-- HEADER END -->");
    if body_start <> [] then
        dia_out(1:body_start(1)) = [];
    end

    body_end   = grep(dia_out,"// <-- FOOTER START -->");
    if body_end <> [] then
        [dia_nl,dia_nc] = size(dia);
        dia_out(body_end(1):dia_nl) = [];
    end
endfunction

//createthefile
function msg = createthefile ( filename )
    // Returns a 2-by-1 matrix of strings, containing a message such as:
    //   Add or create the following file :
    //   - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.dia.ref
    // Workaround for bug #4827
    msg(1) = "   Add or create the following file: "
    msg(2) = "   - "+filename
endfunction

// comparethefiles
function msg = comparethefiles ( filename1 , filename2 )
    // Returns a 3-by-1 matrix of strings, containing a message such as:
    //   Compare the following files :
    //   - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.dia
    //   - C:\path\scilab\modules\optimization\tests\unit_testseldermeadeldermead_configure.dia.ref
    // Workaround for bug #4827
    msg(1) = "   Compare the following files:"
    msg(2) = "   - "+filename1
    msg(3) = "   - "+filename2
    if params.show_diff == %t then
        if getos() == "Windows" then
            diffTool = SCI + "\tools\diff\diff.exe";
        else
            diffTool = "diff";
        end
        targetFile=TMPDIR + filesep() + "tempdiff.diff";
        unix(diffTool + " -u " + filename1 + " " + filename2 + " > " + targetFile);
        // unix_g is failing to return the output into a variable
        msg=[msg; mgetl(targetFile)]
        deletefile(targetFile);
    end
endfunction

function directories = getDirectories(directory)
    directories = directory;
    items = gsort(listfiles(directory),"lr","i");

    for i=1:size(items,"*")
        if isdir(directory + items(i)) then
            directories = [directories; getDirectories(directory + items(i) + filesep())];
        end
    end
endfunction

function name = splitModule(name)
    if ~isempty( regexp(stripblanks(name),"/\|/") ) then
        name = stripblanks( strsubst( strsplit(name,regexp(stripblanks(name),"/\|/")) , "/\|$/","","r" ) );
    end
endfunction

function example = test_examples()
    example = [ sprintf("Examples :\n\n") ];
    example = [ example ; sprintf("// Launch all tests\n") ];
    example = [ example ; sprintf("test_run();\n") ];
    example = [ example ; sprintf("test_run([]);\n") ];
    example = [ example ; sprintf("test_run([],[]);\n") ];
    example = [ example ; "" ];
    example = [ example ; sprintf("// Test one or several module\n") ];
    example = [ example ; sprintf("test_run(''core'');\n") ];
    example = [ example ; sprintf("test_run(''core'',[]);\n") ];
    example = [ example ; sprintf("test_run([''core'',''string'']);\n") ];
    example = [ example ; "" ];
    example = [ example ; sprintf("// Launch one or several test in a specified module\n") ];
    example = [ example ; sprintf("test_run(''core'',[''trycatch'',''opcode'']);\n") ];
    example = [ example ; "" ];
    example = [ example ; sprintf("// With options\n") ];
    example = [ example ; sprintf("test_run([],[],''no_check_ref'');\n") ];
    example = [ example ; sprintf("test_run([],[],''no_check_error_output'');\n") ];
    example = [ example ; sprintf("test_run([],[],''create_ref'');\n") ];
    example = [ example ; sprintf("test_run([],[],''list'');\n") ];
    example = [ example ; sprintf("test_run([],[],''help'');\n") ];
    example = [ example ; sprintf("test_run([],[],[''no_check_ref'',''mode_nw'']);\n") ];
    example = [ example ; "" ];
endfunction

function newOption = clean_option(var, option)
    newOption = var;
    newOption(newOption == option) = [];
endfunction

function result = check_option(var, option)
    result = or(var == option);
endfunction

function value = assign_option(var, option, truevalue, falsevalue)
    if check_option(var, option) then
        value = truevalue;
    else
        value = falsevalue;
    end
endfunction


function exportToXUnitFormat(exportToFile, testsuites)

    if isfile(exportToFile) then
        // File already existing. Append the results
        doc = xmlRead(exportToFile);
        appendIntoFile = %t;
        node = xmlXPath(doc, "//testsuites");
        if node.size == 0 then
            error(msprintf(gettext("The file ''%s'' is not following the XUnit XML format. Root tag expected ''testsuites''.\n"),exportToFile))
        end
    else
        doc = xmlDocument(exportToFile);

        appendIntoFile = %f;
    end
    root = xmlElement(doc, "testsuites");

    for i=1:size(testsuites, "*") // Export module by module
        module = testsuites(i);
        testsuite = xmlElement(doc,"testsuite");
        testsuite.attributes.name = module.name;

        testsuite.attributes.time  = string(module.time);

        testsuite.attributes.tests = string(module.tests);
        testsuite.attributes.errors = string(module.errors);

        for j=1:size(module.testcase,"*") // Export test by test
            testsuite.children(j) = xmlElement(doc,"testcase");
            unitTest = module.testcase(j);
            testsuite.children(j).attributes.name = unitTest.name;
            testsuite.children(j).attributes.time = string(unitTest.time);
            testsuite.children(j).attributes.classname = getversion()+"."+module.name;
            if isfield(unitTest,"failure") & size(unitTest.failure,"*") >= 1 then
                testsuite.children(j).children(1) = xmlElement(doc,"failure");
                testsuite.children(j).children(1).attributes.type = unitTest.failure.type;
                content = unitTest.failure.content;
                for kL=1:size(content, "*")
                    ampIdx = strindex(content(kL), "&");
                    while ~isempty(ampIdx)
                        cur = ampIdx(1);
                        ampIdx(1) = [];
                        if or(part(content(kL), (cur+1):(cur+3))==["gt;" "lt"]) then
                            // Ignored
                        else
                            content(kL) = part(content(kL), 1:cur) + "amp;" + part(content(kL), (cur+1):$);
                            ampIdx = strindex(part(content(kL), (cur+1):$), "&");
                        end
                    end
                end
                testsuite.children(j).children(1).content = content;
            elseif unitTest.skipped then
                testsuite.children(j).children(1) = xmlElement(doc,"skipped");
            end
        end

        if appendIntoFile then
            // We will add the new elements into 'testsuites'
            c=node(1).children;
            nb=size(c,"*");
            c(nb + 1)=testsuite; // Add the new results into the list of results
            root.children=c;
        else
            root.children(i)=testsuite
        end
    end // list of modules

    doc.root=root

    xmlWrite(doc);
endfunction
