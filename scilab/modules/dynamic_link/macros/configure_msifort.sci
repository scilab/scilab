// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


//=============================================================================
function bOK = configure_msifort()

    // Load dynamic_link Internal lib if it's not already loaded
    if ~ exists("dynamic_linkutilslib") then
        load("SCI/modules/dynamic_link/macros/utils/lib");
    end

    //=============================================================================
    //  functions defined only in configure_ifort
    //=============================================================================
    function ifpath = getIFpath(ifversion)
        select ifversion ,
        case "ifort9" then
            ifpath = getenv("IFORT_COMPILER9", "");
        case "ifort10" then
            ifpath = getenv("IFORT_COMPILER10", "");
        case "ifort11" then
            ifpath = getenv("IFORT_COMPILER11", "");
        case "ifort12" then
            ifpath = getenv("IFORT_COMPILER12", "");
        case "ifort13" then
            ifpath = getenv("IFORT_COMPILER13", "");
        case "ifort14" then
            ifpath = getenv("IFORT_COMPILER14", "");
        else
            ifpath = "";
        end
    endfunction
    //=============================================================================
    function bOK = set_commons_ifort12(ifpath, machinepath)
        // intel fortran directories changed (AGAIN !!!) with version 12
        bOK = %F;
        // example set PATH=%IFORT_COMPILER12%\Bin\intel64;
        PATH_TO_ADD = ifpath + "bin" + filesep() + machinepath;
        if isdir(PATH_TO_ADD) then
            err = addPathToEnv("PATH", PATH_TO_ADD);
            if (err == %F) then bOK = %F,return,end
            bOK = %T;
        else
            bOK = %F;
            return;
        end

        // example set INCLUDE=%IFORT_COMPILER12%\compiler\include\intel64;
        PATH_TO_ADD = ifpath + "compiler" + filesep() + "include" + filesep() + machinepath;
        if isdir(PATH_TO_ADD) then
            err = addPathToEnv("INCLUDE", PATH_TO_ADD);
            if (err == %F) then bOK = %F,return,end
            bOK = %T;
        else
            bOK = %F;
            return;
        end

        // example set LIB=%IFORT_COMPILER12%\compiler\lib\intel64;
        PATH_TO_ADD = ifpath + "compiler" + filesep() + "lib" + filesep() + machinepath;
        if isdir(PATH_TO_ADD) then
            err = addPathToEnv("LIB", PATH_TO_ADD);
            if (err == %F) then bOK = %F,return,end
            bOK = %T;
        else
            bOK = %F;
            return;
        end

    endfunction
    //=============================================================================
    function bOK = set_commons_ifort11(ifpath, machinepath)
        // intel fortran directories changed with version 11
        bOK = %F;
        // example set PATH=%IFORT_COMPILER11%\Bin\intel64;
        PATH_TO_ADD = ifpath + "bin" + filesep() + machinepath;
        if isdir(PATH_TO_ADD) then
            err = addPathToEnv("PATH", PATH_TO_ADD);
            if (err == %F) then bOK = %F,return,end
            bOK = %T;
        else
            bOK = %F;
            return;
        end

        // example set INCLUDE=%IFORT_COMPILER11%\include\intel64;
        PATH_TO_ADD = ifpath + "include" + filesep() + machinepath;
        if isdir(PATH_TO_ADD) then
            err = addPathToEnv("INCLUDE", PATH_TO_ADD);
            if (err == %F) then bOK = %F,return,end
            bOK = %T;
        else
            bOK = %F;
            return;
        end

        // example set LIB=%IFORT_COMPILER10%\lib\intel64;
        PATH_TO_ADD = ifpath + "lib" + filesep() + machinepath;
        if isdir(PATH_TO_ADD) then
            ENV_LIB = PATH_TO_ADD + pathsep() + ENV_LIB;
            err = addPathToEnv("LIB", PATH_TO_ADD);
            if (err == %F) then bOK = %F,return,end
            bOK = %T;
        else
            bOK = %F;
            return;
        end

    endfunction
    //=============================================================================
    function bOK = set_commons_msi9and10(ifpath,machinepath)
        bOK = %F;

        // example set PATH=%IFORT_COMPILER10%\EM64T\Bin;
        PATH_TO_ADD = ifpath + machinepath + filesep() + "bin";
        if isdir(PATH_TO_ADD) then
            err = addPathToEnv("PATH", PATH_TO_ADD);
            if (err == %F) then bOK = %F,return,end
            bOK = %T;
        else
            bOK = %F;
            return;
        end

        // example set INCLUDE=%IFORT_COMPILER10%\EM64T\Include;
        PATH_TO_ADD = ifpath + machinepath + filesep() + "Include";
        if isdir(PATH_TO_ADD) then
            err = addPathToEnv("INCLUDE", PATH_TO_ADD);
            if (err == %F) then bOK = %F,return,end
            bOK = %T;
        else
            bOK = %F;
            return;
        end

        // example set LIB=%IFORT_COMPILER10%\EM64T\Lib;
        PATH_TO_ADD = ifpath + machinepath + filesep() + "Lib";
        if isdir(PATH_TO_ADD) then
            err = addPathToEnv("LIB", PATH_TO_ADD);
            if (err == %F) then bOK = %F,return,end
            bOK = %T;
        else
            bOK = %F;
            return;
        end

    endfunction
    //=============================================================================
    bOK = %F;
    if getos() == "Windows" then
        ifortcompiler = findmsifortcompiler();
        if ifortcompiler <> "unknown" then
            if_path = getIFpath(ifortcompiler);

            machinepath = "";

            if ~win64() then
                machinepath = "ia32";
            else
                select ifortcompiler,
                case "ifort14" then
                    machinepath = "intel64";
                case "ifort13" then
                    machinepath = "intel64";
                case "ifort12" then
                    machinepath = "intel64";
                case "ifort11" then
                    machinepath = "intel64";
                else
                    machinepath = "EM64T";
                end
            end

            select ifortcompiler,
            case "ifort14" then
                bOK = set_commons_ifort12(if_path, machinepath);
            case "ifort13" then
                bOK = set_commons_ifort12(if_path, machinepath);
            case "ifort12" then
                bOK = set_commons_ifort12(if_path, machinepath);
            case "ifort11" then
                bOK = set_commons_ifort11(if_path, machinepath);
            case "ifort10" then
                bOK = set_commons_msi9and10(if_path, machinepath);
            case "ifort9" then
                bOK = set_commons_msi9and10(if_path, machinepath);
            else
                bOK = %F;
            end
        end
    end

endfunction
//=============================================================================


