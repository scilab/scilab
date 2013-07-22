// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Simon MARCHETTO
//
// This file is distributed under the same license as the Scilab package.
// =============================================================================

function outStrings = removeTrailingChar(inStrings, trailingChar)
    outStrings = [];
    for i = 1:size(inStrings, "*")
        str = inStrings(i);
        if str <> "" then
            l = length(str);
            if part(str, l:l) == trailingChar then
                outStrings(i) = part(str, 1:l-1);
            else
                outStrings(i) = inStrings(i);
            end
        end
    end
endfunction

function res = addPathToEnv(envName, paths)
    if ~isempty(paths) then
        // Retrieve path list to add
        pathsConverted = pathconvert(paths, %t, %f);
        pathToAddList = tokens(pathsConverted, pathsep());
        pathToAddList = removeTrailingChar(pathToAddList, filesep());

        // Retrieve env path list
        envPaths = getenv(envName, "");
        tmpEnvPath = pathconvert(envPaths, %t, %f);
        envPathList = tokens(tmpEnvPath, pathsep());
        envPathList = removeTrailingChar(envPathList, filesep());
        envPathList(find(envPathList == "")) = [];

        // For each path to add to env
        for i = 1:size(pathToAddList, "r")
            pathToAdd = pathToAddList(i);
            lcPathToAdd = convstr(pathToAdd, "l");
            lcEnvPathList = convstr(envPathList, "l");

            // Add path if do not exist in env path list
            found = find(lcPathToAdd == lcEnvPathList);
            if found == [] then
                envPathList = [envPathList; pathToAdd];
            end

            // Get env new value by concatening env path list
            envPathList = removeTrailingChar(envPathList, pathsep());
            envPaths = strcat(envPathList, pathsep());

            // Set the new env value
            if ~setenv(envName, envPaths) then
                errMsg = msprintf("error while setting" ..
                + " environment variable ''%s''", envName);
                error(errMsg);
            end
        end
    end
    res = %t;
endfunction
