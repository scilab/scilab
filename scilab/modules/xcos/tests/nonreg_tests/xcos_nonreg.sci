// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Laurent VAYLET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function xcos_nonreg(varargin)

    //  Description:
    //
    //    Launcher for Xcos non-regression tests
    //
    //  Input argument(s):
    //
    //    testNames - OPTIONAL - matrix of strings
    //      Only tests in this list will be run
    //        DEFAULT: '*' (all tests are run)
    //
    //  Output argument(s):
    //
    //    -
    //
    //  Usage:
    //
    //     cd SCI/modules/xcos/tests/nonreg_tests;
    //     exec('xcos_nonreg.sci');
    //     xcos_nonreg();
    //     xcos_nonreg('disease');
    //     xcos_nonreg(['disease','rossler']);
    //
    //  Algorithm: (grep "^\s*//--" xcos_nonreg.sci | awk -F "//-- " '{print "//  " $1 $2}')
    //
    //    Parse input arguments
    //    Disable vertical paging
    //    Keep track of number of successful and failed tests
    //    Check if user explicitly supplied a list of tests to run
    //      No: list and sort all available tests (*.zcos files)
    //      Yes: check that given list is valid (test existence of diagrams)
    //      For each available test
    //        Print some info about current test
    //        Launch test and display result
    //        Update lists of successful and failed tests
    //      Display overall report

    //
    //  Notes:
    //
    //    -
    //
    //  2008/04/22     Laurent VAYLET   Creation
    //  2009/11/13     Yann COLLETTE    Xcos adaptation
    //  <YYYY/MM/DD>   <AUTHOR>         Modification
    //

    //-- Parse input arguments
    lhs = argn(1)
    rhs = argn(2)

    // Define default input arguments
    defaultArgs = list("*")
    inputArgs   = defaultArgs

    // Define maximum number of input arguments
    MAX_ARGS  = length(inputArgs)

    // Check number of input arguments
    if rhs > MAX_ARGS
        error(msprintf("%s: Wrong number of input arguments: At most %d expected.\n", ..
        "xcos_nonreg", MAX_ARGS))
    elseif rhs > 0
        // Replace any default value with user-provided one
        for k = 1:rhs
            inputArgs(k) = varargin(k)
        end
    end

    //-- Disable vertical paging
    lines(0)

    //-- Keep track of number of successful and failed tests
    listSuccessful  = list() // list of successful tests
    listFailed      = list() // list of failed tests
    listSkipped     = list() // list of skipped tests
    failureDetails  = ""     // details about every failed test

    // baseDir = fullfile(SCI,'modules','xcos','tests','nonreg_tests')
    baseDir = pwd()

    //-- Check if user explicitly supplied a list of tests to run
    if inputArgs(1) == defaultArgs(1)
        //-- No: list and sort all available tests (*.zcos files)
        xcosFiles   = gsort(basename(listfiles("*.zcos")),"lr","i")
    else
        //-- Yes: check that given list is valid (test existence of diagrams)
        xcosFiles = inputArgs(1)
        idxFilesToDelete = []
        for k = 1:size(xcosFiles,"*")
            if isempty(fileinfo(fullfile(baseDir,cosFiles(k) + ".zcos")))
                // File does not exist: warn user and mark test for deletion
                printf(" WARNING: Test ''%s'' could not be found in ''%s''\n", xcosFiles(k), baseDir)
                idxFilesToDelete = [ idxFilesToDelete ; k ]
            end
        end

        // Delete marked tests (invalid tests)
        xcosFiles(idxFilesToDelete) = []
    end

    nbTests = size(xcosFiles,"*")

    if nbTests ~= 0

        printf("\n")

        //-- For each available test
        for k = 1:nbTests

            //-- Print some info about current test
            printf("   %03d/%03d - ", k, nbTests)
            printf("[%s] %s","xcos",xcosFiles(k))
            for j = length("xcos" + xcosFiles(k)):50
                printf(".")
            end

            //-- Launch test and display result
            try
                status = launch_nonreg(baseDir,xcosFiles(k))
            catch
                // Set status.ok to false in case of error in launch_nonreg
                status.ok = %f
                status.msg = ""
                status.details = ""
            end
            printf("%s \n", status.msg)

            //-- Update lists of successful and failed tests
            if status.ok
                listSuccessful($+1) = xcosFiles(k)
            else
                listFailed($+1)     = xcosFiles(k)
                failureDetails = [ failureDetails ; sprintf("   TEST : [%s] %s","xcos",xcosFiles(k)) ]
                failureDetails = [ failureDetails ; sprintf("     %s", status.msg) ]
                failureDetails = [ failureDetails ; status.details ]
                failureDetails = [ failureDetails ; "" ]
            end
        end

        //-- Display overall report
        nbPassed  = length(listSuccessful)
        nbFailed  = length(listFailed)
        nbSkipped = length(listSkipped)

        printf("\n")
        printf("   --------------------------------------------------------------------------\n")
        printf("   Summary\n\n")
        printf("   tests                     %4d - 100.0 %% \n", nbTests)
        printf("   passed                    %4d - %5.1f %% \n", nbPassed,  nbPassed/nbTests*100)
        printf("   failed                    %4d - %5.1f %% \n", nbFailed,  nbFailed/nbTests*100)
        printf("   skipped                   %4d - %5.1f %% \n", nbSkipped, nbSkipped/nbTests*100)
        printf("   --------------------------------------------------------------------------\n")

        if nbFailed > 0 then
            printf("   Details\n\n")
            printf("%s\n", failureDetails)
            printf("\n")
            printf("   --------------------------------------------------------------------------\n")
        end

    else
        printf("\n ERROR: No valid test found in following directory: ''%s''\n", baseDir)
    end

endfunction

// -----------------------------------------------------------------------------

function status = launch_nonreg(baseDir, testName)

    //  Algorithm: (grep '^\s*//--' xcos_nonreg.sci | awk -F '//-- ' '{print '//  ' $1 $2}')
    //
    //    Initializations
    //    Build a script whose purpose is to launch simulation and log console output
    //    Launch script using a background Scilab
    //    Which version of Scilab was used ?
    //      Non-regression tests launched under Scilab 4.X ?
    //        Rename file.out -> file.out.ref
    //      Non-regression tests launched under Scilab 5.X ?
    //        Compare output data with reference data:
    //        Read output data
    //        Read reference data
    //        Compare (%F meaning identical) and update status


    //-- Initializations

    status.ok      = %f
    status.msg     = []
    status.details = []

    // Define some aliases
    currentScilabFamily = get_scilab_family()
    testFilename   = fullfile(baseDir, testName + ".test")
    modelFilename  = fullfile(baseDir, testName + ".zcos")
    // Keep separate references and results for Windows and Linux, as results given
    // by the two versions have always been different (though it's not logical)
    if getos() == "Windows"
        baseName = testName + ".win"
    else
        baseName = testName + ".unix"
    end
    outFilename    = fullfile(baseDir, baseName + ".out") // foo.win.out, foo.unix.out
    logFilename    = fullfile(baseDir, baseName + ".log") // foo.win.log, foo.unix.log
    errFilename    = fullfile(baseDir, baseName + ".err") // foo.win.err, foo.unix.err


    select currentScilabFamily

    case "4" then
        // Add additional suffix to generate reference files
        outFilename    = outFilename   + ".ref" // foo.win.out.ref, foo.unix.out.ref
        logFilename    = logFilename   + ".ref" // foo.win.log.ref, foo.unix.log.ref
        errFilename    = errFilename   + ".ref" // foo.win.err.ref, foo.unix.err.ref

    case "5" then
        // Do nothing, filenames are already OK

    else
        status.ok = %f
        status.msg = "failed  : Unknown Scilab version"
        status.details = sprintf("     Unknown Scilab version (%s)", getversion())
        return

    end

    // Delete any existing files holding the same names as the ones that are going to be generated
    // After the simulation, a test on the existence of these files can ensure that simulation went all right
    mdelete(testFilename)
    mdelete(outFilename)
    mdelete(logFilename)
    mdelete(errFilename)

    // Define format used in 'Write to File' blocks to log output (Fortran syntax)
    outputFormat = "(7(e22.15,1x))"

    //-- Build a script whose purpose is to launch simulation and log console output
    txt = [ "// Set display settings";
    "mode(3)";
    "clear";
    "lines(28,72)";
    "lines(0)";
    "";
    "// Go to folder containing the diagram to test";
    "cd(''" + baseDir + "'')";
    "";
    "// Load some helper functions (findIOblocks, renameIO, setW2Fformat, ...)";
    "getd(''./utils'')";
    "";
    "// Load and launch simulation, displaying some debug data in the mean time";
    "importXcosDiagram(''" + modelFilename + "'')";
    "";
    "// Rename output file to match variant name";
    "// Override any existing format with predefined one (''" + outputFormat + "'')";
    "// This modification is not saved, it only exists during simulation";
    "[idxWrite, idxRead] = findIOblocks(scs_m)";
    "if ~isempty(idxWrite) & size(idxWrite,''*'') == 1";
    "  scs_m = renameIO(scs_m, idxWrite, ''" + get_filename(outFilename) + "'')";
    "  scs_m = setW2Fformat(scs_m, idxWrite, ''" + outputFormat + "'')";
    "end";
    "";
    "// Rename input file (if present) to match variant name";
    "// This modification is only valid during simulation, thus not saved";
    "if ~isempty(idxRead) & size(idxRead,''*'') == 1";
    "  scs_m = renameIO(scs_m, idxRead, ''" + testName + ".in'')";
    "end";
    "";
    "disp(scs_m)";
    "";
    "// Force compilation";
    "//          %tcur  %cpr    alreadyran  needstart  needcompile  %state0";
    "Info = list(0,     list(), %f,         %t,        4,           list())";
    "try";
    "  Info = scicos_simulate(scs_m,Info,[],''nw'')";
    "catch";
    "  disp(sprintf(''%-25s: ERROR while simulating '',''" + testName + "''))";
    "end";
    "";
    "disp(Info)";
    "";
    "// Quit background Scilab session";
    "exit"];
    mputl(txt, get_filename(testFilename));

    //-- Launch script using a background Scilab
    // Binary or source version ?
    if (getos() <> "Windows") & isempty(fileinfo(SCI + "/bin/scilab")) then
        SCI_BIN = strsubst(SCI, "/share/scilab", "")
    else
        SCI_BIN = SCI
    end
    // Launch previous script inside a NW Scilab and redirect both standard and error output to files
    if getos() == "Windows" then
        cmd = "(""" + SCI_BIN + "\bin\scilex.exe"" -nw -nb -args -nouserstartup -f """ + testFilename + """ > """ + logFilename + """) 2> """ + errFilename + """"
    else
        cmd = "(''" + SCI_BIN + "/bin/scilab'' -nw -nb -args -nouserstartup -f ''" + testFilename + "'' > ''" + logFilename + "'') 2> ''" + errFilename + "''"
    end
    // mputl(cmd, fullfile(baseDir, testName + '.cmd')) // Log the command for debug purpose
    host(cmd)

    // Sleep for 100 ms to let files be created and saved
    sleep(100)

    //-- Which version of Scilab was used ?
    select currentScilabFamily

        //-- Non-regression tests launched under Scilab 4.X ?
    case "4" then

        //-- Check that reference file has been created and exists
        status.ok = ~isempty(fileinfo(outFilename))

        // Status determines if copy succeeded or failed
        // A failure might indicate that simulation failed and did not produce any output
        if status.ok
            status.msg = "passed  : Reference file successfully generated"
            status.details = ""
            return
        else
            status.msg = "failed  : Reference file NOT generated"
            status.details = sprintf("     It might indicate a failure during simulation")
            status.details = [ status.details ; sprintf("     Try running the simulation manually by opening ''%s.zcos'' in Scicos", testName) ]
            status.details = [ status.details ; sprintf("     Last know error:") ]
            status.details = [ status.details ; lasterror() ]
            return
        end

        //-- Non-regression tests launched under Scilab 5.X ?
    case "5" then

        //-- Compare output data with reference data:
        outRefFilename = outFilename + ".ref"
        //-- Read output data
        try
            fidOut = mopen(outFilename, "r")
            out = mgetl(fidOut)
            mclose(fidOut)
        catch
            status.ok = %f
            status.msg = "failed  : Cannot read output data"
            status.details = sprintf("     Cannot read output data from file ''%s''", outFilename)
            status.details = [ status.details ; sprintf("     It might indicate a failure during simulation") ]
            status.details = [ status.details ; sprintf("     Try running the simulation manually by opening ''%s.zcos'' in Scicos", testName) ]
            status.details = [ status.details ; sprintf("     Last know error:") ]
            status.details = [ status.details ; lasterror() ]
            return // go on to next test
        end

        //-- Read reference data
        try
            fidRef = mopen(outRefFilename, "r")
            ref = mgetl(fidRef)
            mclose(fidRef)
        catch
            status.ok = %f
            status.msg = "failed  : Cannot read reference data"
            status.details = sprintf("     Cannot read reference data from file ''%s''", outRefFilename)
            status.details = [ status.details ; sprintf("     Last know error:") ]
            status.details = [ status.details ; lasterror() ]
            return // go on to next test
        end

        //-- Compare (%F meaning identical) and update status
        if or(out<>ref)
            status.ok      = %f
            status.msg     = "failed  : Output and reference are NOT equal"
            status.details = sprintf("     Compare the following files for more details:")
            status.details = [ status.details ; sprintf("     - %s", outFilename) ]
            status.details = [ status.details ; sprintf("     - %s", outRefFilename) ]
            return
        else
            status.ok      = %t
            status.msg     = "passed  : Output and reference are equal"
            status.details = ""
            return
        end
    end
endfunction

// -----------------------------------------------------------------------------

function family = get_scilab_family()
    // Get family (major version) of currently running Scilab
    //
    // Algorithm: (grep '^\s*//--' xcos_nonreg.sci | awk -F '//-- ' '{print '//  ' $1 $2}')
    //
    //    Initialize output to [] <=> unknown version of Scilab
    //    Get complete version name
    //    Extract family from a known pattern found in version name


    //-- Initialize output to [] <=> unknown version of Scilab
    family = []

    //-- Get complete version name
    version = getversion()

    //-- Extract family from a known pattern found in version name
    if ~isempty(grep(getversion(),"scilab-4")) ..
        | ~isempty(grep(getversion(), "Scilab-4")) ..
        | ~isempty(grep(getversion(), "scicos_work"))

        // 4.X version
        family = "4"

    elseif ~isempty(grep(getversion(), "trunk")) ..
        | ~isempty(grep(getversion(), "scilab-5"))

        // 5.X version
        family = "5"

    end

endfunction

// -----------------------------------------------------------------------------

function filename = get_filename(fullPath)
    // Extract filename from a full path
    // Ex: --> get_filename('/home/vaylet/dev/scicos_work/modules/xcos/tests/nonreg_tests/constant.test')
    //      ans =
    //
    //      contant.test

    [path,base,extension] = fileparts(fullPath)
    filename = base + extension

endfunction

// -----------------------------------------------------------------------------

