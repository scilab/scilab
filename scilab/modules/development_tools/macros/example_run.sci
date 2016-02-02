// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Michael Baudin
// Copyright (C) 2012 - Scilab Enterprises - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function boolStatus = example_run(modulesName, functionsName, helpLanguage, varargin)

    rhs = argn(2);

    if rhs < 1 then
        modulesName = getmodules();
    end
    if rhs < 2 then
        functionsName = [];
    end
    if rhs < 3 then
        helpLanguage = "en_US";
    end

    if typeof(modulesName)<>"string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A row array of strings expected.\n"), "example_run", 1));
    end
    if size(modulesName, 2)<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A row array of strings expected.\n"), "example_run", 1));
    end

    if (typeof(functionsName)=="constant" & ~isempty(functionsName)) & typeof(functionsName)<>"string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A row array of strings or empty matrix expected.\n"), "example_run", 2));
    end
    if ~isempty(functionsName) & size(functionsName, 2)<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A row array of strings or empty matrix expected.\n"), "example_run", 2));
    end

    if typeof(helpLanguage)<>"string" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"), "example_run", 3));
    end
    if size(helpLanguage, "*")<>1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"), "example_run", 3));
    end

    for kMod = 1:size(modulesName, "*")

        moduleName = modulesName(kMod);

        if (isdir(moduleName)) then
            moduleDir = fullfile(moduleName,"help");
        else
            moduleDir = fullfile(SCI,"modules",moduleName,"help");
            if ~isdir(moduleDir) then
                error(msprintf(gettext("%s: Wrong value for input argument #%d: A Scilab module name expected.\n"), "example_run", 1));
                return
            end
        end

        moduleDir = fullfile(moduleDir,helpLanguage);
        if ~isdir(moduleDir) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: A valid language expected.\n"), "example_run", 3));
            return
        end

        tempModuleName = basename(moduleName) + "_help";

        workDir = fullfile(TMPDIR,tempModuleName,"tests","unit_tests");
        if isdir(workDir) then
            rmdir(workDir, "s");
        end

        if isempty(functionsName) then
            xmlFiles = findHelpFiles(moduleDir, []);

            for kFile = 1:size(xmlFiles, "*")
                xmlFile = xmlFiles(kFile);
                createTestsFromHelp(moduleDir, workDir, xmlFile)
            end
        else
            for kFunction = 1:size(functionsName, "*")
                xmlFiles = findHelpFiles(moduleDir, functionsName(kFunction));
                if isempty(xmlFiles) then // The user wants to test a specific file
                    error(msprintf(gettext("%s: Wrong value for input argument #%d: A ''%s'' module function name expected.\n"), "example_run", 2, moduleName));
                    return
                end

                for kFile = 1:size(xmlFiles, "*")
                    xmlFile = xmlFiles(kFile);
                    createTestsFromHelp(moduleDir, workDir, xmlFile)
                end
            end
        end

        testDir=fullfile(TMPDIR,tempModuleName)
        if rhs > 3 then
            boolStatus = test_run(testDir, [], varargin(:))
        else
            boolStatus = test_run(testDir, [])
        end
    end

endfunction

function createTestsFromHelp(baseDirectory, outputDirectory, helpFile)

    mkdir(outputDirectory);

    helpFileDoc = xmlRead(helpFile);

    // Find all programlisting tag in the file
    exampleTags = xmlXPath(helpFileDoc, "//scilabhelpfile:programlisting", ["scilabhelpfile" "http://docbook.org/ns/docbook"]);

    for kExample = 1:size(exampleTags, "*")

        programListing = [];

        if exampleTags(kExample).attributes.role == "interactive-example" then
            programListing = ["// <-- INTERACTIVE TEST -->" ; programListing];
        end

        ignoredRoles = ["no-scilab-exec"];
        ignoreExample = or(exampleTags(kExample).attributes.role==ignoredRoles) | isempty(exampleTags(kExample).attributes.role);

        programListing = ["// <-- NO CHECK REF -->" ; programListing; exampleTags(kExample).content];

        if ~ignoreExample then
            functionName = fileparts(helpFile, "fname");
            relPath = dirname(getrelativefilename(baseDirectory, helpFile));

            mkdir(fullfile(outputDirectory,relPath))

            if size(exampleTags, "*")>1 then
                filebase=functionName + "_" + string(kExample) + ".tst"
                destfile=fullfile(outputDirectory,relPath,filebase);
                mputl(programListing,destfile);
            else
                filebase=functionName + ".tst"
                destfile=fullfile(outputDirectory,relPath,filebase)
                mputl(programListing,destfile);
            end
        end
    end

endfunction

function xmlFilenames = findHelpFiles(directory, functionName)

    xmlFilenames = [];

    allFiles = ls(directory);

    for kFile = 1:size(allFiles, "*")
        if isfile(allFiles(kFile)) then
            if fileparts(allFiles(kFile), "extension")==".xml" then
                if isempty(functionName) then
                    xmlFilenames($+1) = allFiles(kFile);
                elseif fileparts(allFiles(kFile), "fname")==functionName then
                    xmlFilenames($+1) = allFiles(kFile);
                end
            end
        else // Directory
            subDir=fullfile(directory,allFiles(kFile))
            subxmlfiles=findHelpFiles(subDir, functionName)
            xmlFilenames = [xmlFilenames;subxmlfiles]
        end
    end

endfunction
