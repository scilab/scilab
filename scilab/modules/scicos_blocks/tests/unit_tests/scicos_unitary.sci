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

function scicos_unitary(varargin)

    //  Description:
    //
    //    Unitary tests launcher
    //
    //  Input argument(s):
    //
    //    paletteNames - OPTIONAL - matrix of strings
    //      Name(s) of the palette(s) for which unit tests should be launched
    //        DEFAULT: '*'
    //
    //    blockNames - OPTIONAL - matrix of strings
    //      Name(s) of the blocks(s) for which unit tests should be launched
    //        DEFAULT: '*'
    //
    //  Output argument(s):
    //
    //     -
    //
    //  Usage:
    //
    //    scicos_unitary()
    //    scicos_unitary('Linear')
    //    scicos_unitary('Linear','GAINBLK')
    //
    //  Algorithm: (grep "^\s*//--" scicos_unitary.sci | awk -F "//-- " '{print "//  " $1 $2}')
    //
    //    Parse input arguments
    //    Initializations
    //    Retrieve names of palettes to check (if not provided by user)
    //    For each palette
    //      Display current palette name
    //      Save current directory
    //      Go to directory of selected palette
    //        If directory does not exist, proceed to next palette
    //      Find all available diagrams for current palette
    //      Define names of blocks to check
    //        If not provided by user, get names of all blocks in current palette
    //          On error, proceed to next palette
    //      For each block to check
    //        Find associated diagrams as well as their indices
    //        For each diagram
    //          Define diagram name
    //          Find available variants (different contexts) as well as their indices
    //            No variant found: define a dummy one
    //          For each variant
    //            Define complete test name
    //              Multiple variants: define test name and get associated context
    //              Get context for current variant
    //                On error, proceed to next test
    //              No variant found: use main diagram (no context to load)
    //            Launch simulation
    //              On error, proceed to next test
    //            Read output data
    //              On error, proceed to next test
    //            Read reference data
    //              On error, proceed to next test
    //            Compare output and reference
    //            Display comparison result
    //              OK: add to the list of successful tests
    //              KO: add to the list of failed tests
    //      Go back to original directory
    //
    //  Notes:
    //
    //    -
    //
    //  2008/03/31  Laurent Vaylet    Creation
    //  YYYY/MM/DD  ???               Modification: ???
    //

    //-- Parse input arguments
    lhs = argn(1)
    rhs = argn(2)

    // Default input arguments
    defaultArgs = list("*","*")
    inputArgs   = defaultArgs
    // Maximum number of input arguments
    MAX_ARGS  = length(inputArgs)

    if rhs > MAX_ARGS
        error(msprintf(gettext("%s: Wrong number of input arguments: At most %d expected.\n"), ..
        "scicos_unitary", MAX_ARGS))
    elseif rhs > 0
        for k = 1:rhs
            inputArgs(k) = varargin(k)
        end
    end

    paletteNames = inputArgs(1)
    blockNames   = inputArgs(2)

    //-- Initializations
    listNoResult    = list() // list of tests for which no result could be obtained
    // (an error occurred before, during or after the simulation)
    listSuccessful  = list() // list of successful tests
    listFailed      = list() // list of failed tests

    //-- Retrieve names of palettes to check (if not provided by user)
    if paletteNames == defaultArgs(1)
        filesList = dir(fullfile(SCI,"modules","scicos_blocks","macros"))
        paletteNames = filesList.name(filesList.isdir)
        paletteNames(paletteNames == ".svn") = [] // discard '.svn' folder
    end

    //-- For each palette
    for paletteName = paletteNames'

        //-- Display current palette name
        mprintf("\n ======================================\n")
        mprintf("  Palette: %s", paletteName)
        mprintf("\n ======================================\n\n")

        //-- Save current directory
        currentDir = pwd()
        //-- Go to directory of selected palette
        palettePath = fullfile("SCI","modules","scicos_blocks","tests","unit_tests",paletteName)
        if ~isempty(fileinfo(palettePath))
            chdir(palettePath)
        else
            //-- If directory does not exist, proceed to next palette
            mprintf("  No directory found for palette %s in SCI/modules/scicos_blocks/tests/units...\n", paletteName)
            continue
        end

        //-- Find all available diagrams for current palette
        cosFiles = gsort(listfiles("*.cos"),"lr","i")

        //-- Define names of blocks to check
        if blockNames == defaultArgs(2)
            //-- If not provided by user, get names of all blocks in current palette
            // Use file 'names' in 'macros/<paletteName>')
            try
                fid = mopen(fullfile(SCI,"modules","scicos_blocks","macros",paletteName,"names"))
            catch
                //-- On error, proceed to next palette
                mprintf("Cannot open file ''names'' in ''%s'' palette.\n"..
                + "Check that file exists in ''%s''\n", ..
                paletteName, fullfile(SCI,"modules","scicos_blocks","macros",paletteName))
                continue
            end
            blockNames = gsort(mgetl(fid),"lr","i") // sort alphabetically
            mclose(fid)
        end

        //-- For each block to check
        for idxBlockName = 1:size(blockNames,"*")

            //-- Find associated diagrams as well as their indices
            blockName = blockNames(idxBlockName)
            idxDiagrams = msscanf(-1, cosFiles, blockName + "_%03d")
            printf(" %s: %03d test(s) available\n", blockName, size(idxDiagrams,"*"))

            //-- For each diagram
            for idxDiagram = idxDiagrams'

                //-- Define diagram name
                diagramName = msprintf("%s_%03d", blockName, idxDiagram)

                //-- Find available variants (different contexts) as well as their indices
                cxtFiles = gsort(listfiles(diagramName + "*.cxt"),"lr","i")
                if ~isempty(cxtFiles)
                    idxContexts = msscanf(-1, cxtFiles, blockName + sprintf("_%03d", ..
                    idxDiagram) + "_%03d")
                    mprintf("\n   --> Test #%d: %s: %d variants found\n", idxDiagram, diagramName, length(idxContexts))
                else
                    //-- No variant found: define a dummy one
                    idxContexts = -1
                    mprintf("\n   --> Test #%d: %s: No variant found\n", idxDiagram, diagramName)
                end

                //-- For each variant
                for idxContext = idxContexts'

                    //-- Define complete test name
                    if idxContexts ~= -1
                        //-- Multiple variants: define test name and get associated context
                        testName    = msprintf("%s_%03d", diagramName, idxContext)
                        mprintf("\n     -> Variant #%d: %s:\n\n", idxContext, testName)

                        //-- Get context for current variant
                        mprintf("       Retrieving context...")
                        [context, status] = getcontext(testName)
                        if status.ok
                            mprintf(" OK\n")
                            %scicos_context = context
                        else
                            //-- On error, proceed to next test
                            mprintf("\n       Could not retrieve context for %s...\n", ..
                            "launchtest", testName)
                            mprintf(%s, status.details)
                            listNoResult($+1) = paletteName + "/" + testName
                            continue
                        end
                    else
                        //-- No variant found: use main diagram (no context to load)
                        testName = diagramName
                        mprintf("\n     -> Main diagram\n\n")
                    end

                    //-- Launch simulation
                    status = launchsim(diagramName, testName)
                    if ~status.ok
                        printf("%s\n", status.msg)
                        printf("%s\n", status.details)
                        //-- On error, proceed to next test
                        listNoResult($+1) = paletteName + "/" + testName
                        continue
                    end

                    //-- Read output data
                    mprintf("       Reading output data...")
                    outFile = testName + ".out"
                    [status, out] = readoutputfile(outFile)
                    printf(" %s\n", status.msg)
                    if ~status.ok
                        //-- On error, proceed to next test
                        listNoResult($+1) = paletteName + "/" + testName
                        continue
                    end

                    //-- Read reference data
                    mprintf("       Reading reference data...")
                    refFile = outFile + ".ref"
                    [status, ref] = readoutputfile(refFile)
                    printf(" %s\n", status.msg)
                    if ~status.ok
                        //-- On error, proceed to next test
                        listNoResult($+1) = paletteName + "/" + testName
                        continue
                    end

                    //-- Compare output and reference
                    mprintf("       Comparing output and reference data...\n")
                    status = compareresults(out, ref)
                    //-- Display comparison result
                    printf("\n     %s %s\n\n", testName, status.msg)
                    if status.ok
                        //-- OK: add to the list of successful tests
                        listSuccessful($+1) = paletteName + "/" + testName
                    else
                        //-- KO: add to the list of failed tests
                        printf("%s\n\n", status.details)
                        listFailed($+1) = paletteName + "/" + testName
                    end

                end // for idxDiagram = ...
            end // for idxContext = ...
        end // for idxBlockName = ...

        //-- Go back to original directory
        chdir(currentDir)

    end // for paletteName = ...

    //-- Display overall report
    nbSuccessful = length(listSuccessful)
    nbFailed     = length(listFailed)
    nbNoResult   = length(listNoResult)
    nbTotal      = nbSuccessful + nbFailed + nbNoResult
    mprintf("\n ======================================\n")
    mprintf("  Summary")
    mprintf("\n ======================================\n\n")
    mprintf("  Total number of tests:               %4d - 100.0% \n", nbTotal)
    mprintf("  Successful tests:                    %4d - %5.1f %% \n", nbSuccessful, nbSuccessful/nbTotal*100)
    mprintf("  Failed tests:                        %4d - %5.1f %% \n", nbFailed,     nbFailed/nbTotal*100)
    mprintf("  Undetermined tests (no result):      %4d - %5.1f %% \n", nbNoResult,   nbNoResult/nbTotal*100)

endfunction

// -----------------------------------------------------------------------------

function status = launchsim(diagramName, testName)

    //  Description:
    //
    //    Launch simulation for specified test
    //
    //  Input argument(s):
    //
    //    diagramName - MANDATORY - string
    //      Name of the root diagram to use
    //
    //    testName - MANDATORY - string
    //      Name of the test to launch (usually the root diagram itself or one of its variants)
    //
    //  Output argument(s):
    //
    //    status - MANDATORY - structure
    //      Feedback on success or failure
    //
    //  Usage:
    //
    //    launchsim('GAINBLK_001', 'GAINBLK_001_001')
    //
    //  Algorithm:
    //    Initializations
    //    Load diagram -> scs_m, %cpr
    //      Proceed to next test if loading fails
    //    Rename output file to match variant name
    //      If no output found, proceed to next test
    //    Rename input file (if present) to match variant name
    //    Run simulation
    //      Force diagram recompilation
    //      Launch simulation and generate output data
    //      Proceed to next test if simulation fails

    //-- Initializations
    status.ok      = %t
    status.msg     = ""
    status.details = ""

    //-- Load diagram -> scs_m, %cpr
    mprintf("       Loading diagram...")
    try
        load(diagramName + ".cos")
    catch
        //-- Proceed to next test if loading fails
        status.ok = %f
        status.msg = msprintf("Loading failed...")
        status.details = msprintf("%s", lasterror())
        return
    end
    mprintf(" OK\n")

    //-- Rename output file to match variant name
    // This modification is not saved, it only exists during simulation
    //@TODO: Check that idxWrite is a scalar (only one block)
    mprintf("       Renaming output...")
    [idxWrite, idxRead] = findIOblocks(scs_m)
    if ~isempty(idxWrite)
        scs_m = renameIO(scs_m, idxWrite, testName + ".out")
        mprintf(" OK\n")
    else
        //-- If no output found, proceed to next test
        status.ok = %f
        status.msg = msprintf(" No output block found !!")
        return
    end

    //-- Rename input file (if present) to match variant name
    // This modification is not saved, it only exists during simulation
    //@TODO: Check that idxRead is a scalar (only one block)
    mprintf("       Renaming input...")
    if ~isempty(idxRead)
        scs_m = renameIO(scs_m, idxRead, testName + ".in")
        mprintf(" OK\n")
    else
        mprintf(" No input block found.\n");
    end

    //-- Run simulation
    try
        //-- Force diagram recompilation
        Info = list()
        //-- Launch simulation and generate output data
        // Use 'nw' in order not to display scopes or graphical blocks
        mprintf("       Launching simulation...")
        Info = scicos_simulate(scs_m, Info, %scicos_context, "nw")
        mprintf(" OK\n")
    catch
        //-- Proceed to next test if simulation fails
        status.ok = %f
        status.msg = msprintf("Simulation failed...")
        status.details = msprintf("%s", lasterror())
        return
    end

endfunction

// -----------------------------------------------------------------------------

function [status, str] = readoutputfile(filename)

    //  Description:
    //
    //    Read contents of an output file
    //
    //  Input argument(s):
    //
    //    filename - MANDATORY - string
    //      Name of the file containing data
    //
    //  Output argument(s):
    //
    //    status - MANDATORY - structure
    //      Feedback on success or failure
    //
    //    str - MANDATORY - matrix of strings
    //      File contents, such as read by 'mgetl'
    //
    //  Usage:
    //
    //    [status, str] = readoutputfile('GAINBLK_001_001.out')
    //
    //  Algorithm:
    //    Initializations
    //    Open file for reading
    //    Read contents and put in 'str'
    //    Close file

    //-- Initializations
    status.ok      = %t
    status.msg     = "OK"
    status.details = ""
    str            = []

    //-- Open file for reading
    try
        fidOutput = mopen(filename, "r")
    catch
        status.ok = %f
        status.msg = msprintf("Could not open file ''%s'' for reading", filename)
        return
    end
    //-- Read contents and put in 'str'
    str = mgetl(fidOutput)
    //-- Close file
    mclose(fidOutput)

endfunction

// -----------------------------------------------------------------------------

function status = compareresults(out, ref)

    //  Description:
    //
    //    Compare output from simulation with reference data
    //
    //  Input argument(s):
    //
    //    out - MANDATORY - matrix of strings
    //      Output from simulation
    //
    //    ref - MANDATORY - matrix of strings
    //      Reference data
    //
    //  Output argument(s):
    //
    //    status - MANDATORY - structure
    //      Feedback on success or failure
    //
    //  Usage:
    //
    //    status = compareresults(out, ref)
    //
    //  Algorithm:
    //    Initializations
    //    Perform line-by-line comparison of output and reference data

    //-- Initializations
    status.ok      = %t
    status.msg     = "passed: output and reference are equal                [OK]"
    status.details = ""

    //-- Perform line-by-line comparison of output and reference data
    if or(out<>ref)
        status.ok      = %f // KO
        status.msg     = "failed: output and reference are NOT equal            [KO]"
        status.details = msprintf(..
        "       Compare the following files for more details :\n" + ..
        "         - %s\n" + ..
        "         - %s", outFile, refFile)
    end

endfunction

// -----------------------------------------------------------------------------

