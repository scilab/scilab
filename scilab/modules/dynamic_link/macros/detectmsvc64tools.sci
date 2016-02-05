// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) DIGITEO - 2010 - Allan CORNET
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
function bOK = detectmsvc64tools()

    bOK = %F;
    if getos() <> "Windows" then
        return
    end

    //=============================================================================
    // internal functions
    //=============================================================================
    function show = displayWarningMsVC()
        settings_filename = "/.settings_warning_msvc";
        settings_file = pathconvert(SCIHOME + settings_filename, %f, %t);
        w = fileinfo(settings_file);
        show = %t;
        if isfile(settings_file) then
            show = grep(mgetl(settings_file), "displayWarningMSVCx64=no") == [];
        end
    endfunction
    //=============================================================================
    function disableWarningMsVC()
        settings_filename = "/.settings_warning_msvc";
        settings_file = pathconvert(SCIHOME + settings_filename, %f, %t);
        mputl("displayWarningMSVCx64=no", settings_file);
    endfunction
    //=============================================================================


    // Load dynamic_link Internal lib if it's not already loaded
    if ~ exists("dynamic_linkwindowslib") then
        load("SCI/modules/dynamic_link/macros/windows/lib");
    end

    if win64() then
        compiler = findmsvccompiler();
        supported_compiler = [ ...
        "msvc140pro", ..
        "msvc140express", ..
        "msvc120pro", ..
        "msvc120express", ..
        "msvc110pro", ..
        "msvc110express", ..
        "msvc100pro", ..
        "msvc100express", ..
        "msvc90pro", ..
        "msvc90std", ..
        "msvc90express"];

        if (find(supported_compiler == compiler) <> []) then
            MSVCBIN64PATH = dlwGet64BitPath();
            if dlwIsVc11Express() | dlwIsVc12Express() | dlwIsVc14Express() then
                MSVCBIN64PATH = MSVCBIN64PATH + filesep() + "VC\bin";
            else
                MSVCBIN64PATH = MSVCBIN64PATH + filesep() + "VC\bin\amd64";
            end

            if isdir(MSVCBIN64PATH) then
                bOK = %T;
            else
                show = displayWarningMsVC();
                if show then
                    TXT = gettext("Microsoft Visual Studio C 2008 (or more recent) x64 Compiler not installed.");
                    warning(TXT);
                    clear TXT;
                    disableWarningMsVC();
                end
                bOK = %F;
            end
        else
            show = displayWarningMsVC();
            if show then
                TXT = gettext("Microsoft Visual Studio C 2008 (or more recent) Compiler not found.");
                warning(TXT);
                clear TXT;
                disableWarningMsVC();
            end
            bOK = %F;
        end
    end

endfunction
//=============================================================================
