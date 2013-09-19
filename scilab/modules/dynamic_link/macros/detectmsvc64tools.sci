// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
    supported_compiler = ['msvc110pro', ..
                          'msvc110express', ..
                          'msvc100pro', ..
                          'msvc100express', ..
                          'msvc90pro', ..
                          'msvc90std', ..
                          'msvc90express'];

        if (find(supported_compiler == compiler) <> []) then
            MSVCBIN64PATH = dlwGet64BitPath();
            if dlwIsVc11Express() then
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
