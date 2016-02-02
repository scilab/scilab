//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//
function compilerpath = getmodelicacpath()

    MODELICAC_FILENAME = "modelicac";

    compilerpath = "";

    if getos() == "Windows" then
        // --- WINDOWS ---
        MODELICAC_FILENAME = MODELICAC_FILENAME + ".exe";
        defaultPath = SCI + "/bin/";

        if isfile(defaultPath + MODELICAC_FILENAME) then
            compilerpath = defaultPath;
        else
            compilerpath = "";
        end

    else
        // --- LINUX/MACOS ---

        // Check if source tree version
        if isdir(SCI+"/modules/core/includes/") then
            defaultPath = SCI + "/modules/scicos/";

            if isfile(defaultPath + MODELICAC_FILENAME) then
                compilerpath = defaultPath;
            else
                compilerpath = "";
            end
        end

        // Check if binary version
        if compilerpath=="" & isdir(SCI+"/../../include/scilab/") then
            defaultPath = SCI + "/../../bin/";

            if isfile(defaultPath + MODELICAC_FILENAME) then
                compilerpath = defaultPath;
            else
                compilerpath = "";
            end
        end

        // Check if system version
        if compilerpath=="" & isdir("/usr/include/scilab/") then
            defaultPath = "/usr/bin/";

            if isfile(defaultPath + MODELICAC_FILENAME) then
                compilerpath = defaultPath;
            else
                compilerpath = "";
            end
        end

    end
endfunction

