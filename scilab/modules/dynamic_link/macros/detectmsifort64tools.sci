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
function bOK = detectmsifort64tools()

    bOK = %F;
    if getos() == "Windows" & win64() then
        IFORTPATH = "";
        ifortcomp = findmsifortcompiler();
        if ifortcomp <> "unknown" then
            select ifortcomp,
            case  "ifort14" then
                IFORTPATH = getenv("IFORT_COMPILER14","");

            case  "ifort13" then
                IFORTPATH = getenv("IFORT_COMPILER13","");

            case  "ifort12" then
                IFORTPATH = getenv("IFORT_COMPILER12","");

            case  "ifort11" then
                IFORTPATH = getenv("IFORT_COMPILER11","");

            case  "ifort10" then
                IFORTPATH = getenv("IFORT_COMPILER10","");

            case  "ifort9" then
                IFORTPATH = getenv("IFORT_COMPILER9","");

            else
                show = displayWarningmsifort();
                if show then
                    TXT = gettext("Intel Fortran Compiler 9, 10, 11 or 12 Compiler not found.");
                    warning(TXT);
                    clear TXT;
                    disableWarningmsifort();
                end
                bOK = %F;
                return
            end
        end

        select ifortcomp,
        case  "ifort14" then
            IFORTSPATHEMT64 = IFORTPATH + "compiler\lib\intel64";
        case  "ifort13" then
            IFORTSPATHEMT64 = IFORTPATH + "compiler\lib\intel64";
        case  "ifort12" then
            IFORTSPATHEMT64 = IFORTPATH + "compiler\lib\intel64";
        case  "ifort11" then
            IFORTSPATHEMT64 = IFORTPATH + "lib\intel64";
        else
            IFORTSPATHEMT64 = IFORTPATH + "em64t";
        end

        if isdir(IFORTSPATHEMT64) then
            bOK = %T;
        end
    end

endfunction
//=============================================================================
function show = displayWarningmsifort()
    settings_filename = "/.settings_warning_msif";
    settings_file = pathconvert(SCIHOME+settings_filename,%f,%t);
    w = fileinfo(settings_file);
    show = %t;
    if isfile(settings_file) then
        show = grep(mgetl(settings_file),"displayWarningIFx64=no")==[]
    end
endfunction
//=============================================================================
function disableWarningmsifort()
    settings_filename = "/.settings_warning_msif";
    settings_file = pathconvert(SCIHOME+settings_filename,%f,%t);
    mputl("displayWarningIFx64=no",settings_file);
endfunction
//=============================================================================
