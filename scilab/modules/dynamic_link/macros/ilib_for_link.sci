// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA/ENPC
// Copyright (C) DIGITEO - 2009-2011 - Allan CORNET
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
// Generate a shared library which can be used by link command.
function libn = ilib_for_link(names, ..
    files, ..
    libs, ..
    flag, ..
    makename, ..
    loadername, ..
    libname, ..
    ldflags, ..
    cflags, ..
    fflags, ..
    cc)

    [lhs, rhs] = argn(0);
    if rhs < 4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"),"ilib_for_link"));
        return
    end

    if rhs > 4 then
        if (makename <> [] & makename <> "") then
            warning(msprintf(_("%s: Wrong value for input argument #%d: """" or ""[]"" expected.\n"), "ilib_for_link", 5));
        end
    end
    if rhs <= 5 then loadername = "loader.sce";end
    if rhs <= 6 then libname = ""; end
    if rhs <= 7 then ldflags = ""; end
    if rhs <= 8 then cflags  = ""; end
    if rhs <= 9 then fflags  = ""; end
    if rhs <= 10 then cc  = ""; end

    if isempty(files) | ~and(isfile(files)) then
        error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected.\n"), "ilib_for_link", 2));
    end

    if ~isempty(files) & (or(fileext(files)==".o") | or(fileext(files)==".obj")) then
        error(999, msprintf(_("%s: A managed file extension for input argument #%d expected.\n"), "ilib_for_link", 2));
    end

    if getos() == "Windows" then
        if ~ exists("dynamic_linkwindowslib") then
            load("SCI/modules/dynamic_link/macros/windows/lib");
        end
        makename = dlwGetDefltMakefileName() + dlwGetMakefileExt();
    else
        makename = "Makefile";
    end

    // generate a loader file
    if ( ilib_verbose() <> 0 ) then
        mprintf(gettext("   Generate a loader file\n"));
    end

    tables = [];

    // we manage .f90 as .f on windows
    if getos() == "Windows" then
        if findmsifortcompiler() <> "unknown" then
            if flag == "f90" then
                flag = "f";
            end
        else
            if flag == "f90" then
                error(999, _("F2C cannot build fortran 90"));
            end
        end
    end

    ilib_gen_loader(names, tables, libs, libname, flag, loadername);

    // bug 4515 - unlink previous function with same name
    n = size(names,"*");
    for i = 1:n
        execstr("[bOK, ilib] = c_link(""" + names(i) + """);if bOK then ulink(ilib), end");
    end

    // generate a Makefile
    if ( ilib_verbose() <> 0 ) then
        mprintf(gettext("   Generate a Makefile\n"));
    end

    modifiedmakename = generateMakefile(names, ..
    files, ..
    libs, ..
    libname, ..
    ldflags, ..
    cflags, ..
    fflags, ..
    cc, ..
    flag);

    if modifiedmakename <> makename then
        makename = modifiedmakename;
    end

    // we call make
    if ( ilib_verbose() <> 0 ) then
        mprintf(gettext("   Running the Makefile\n"));
    end

    if (libname == "") then
        libname = names(1);
    end

    libn = ilib_compile("lib" + libname, makename, files);

    if ( ilib_verbose() <> 0 ) then
        mprintf(_("   Generate a cleaner file\n"));
    end

    ilib_gen_cleaner(makename, loadername, libn);

endfunction
//=============================================================================
function makename = generateMakefile(names, ..
    files, ..
    libs, ..
    libname, ..
    ldflags, ..
    cflags, ..
    fflags, ..
    cc, ..
    flag)

    makename = [];
    if getos() <> "Windows" then
        Makename = makename;
        ilib_gen_Make_unix(names, ..
        files, ..
        libs, ..
        libname, ..
        ldflags, ..
        cflags, ..
        fflags, ..
        cc);
    else
        if ~ exists("dynamic_linkwindowslib") then
            load("SCI/modules/dynamic_link/macros/windows/lib");
        end

        if strncpy(names,3) <> "lib" then
            names = "lib" + names;
        end

        names = names(1);

        makename = dlwGenerateMakefile(names, ..
        [], ..
        files, ..
        libs, ..
        libname, ..
        %f, ..
        ldflags, ..
        cflags, ..
        fflags, ..
        cc);
    end

endfunction
//=============================================================================

