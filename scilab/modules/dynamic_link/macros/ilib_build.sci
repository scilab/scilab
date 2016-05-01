// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC/INRIA
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
function ilib_build(ilib_name, ..
    table, ..
    files, ..
    libs, ..
    makename, ..
    ldflags, ..
    cflags, ..
    fflags, ..
    ismex, ..
    cc)

    if ~haveacompiler() then
        error(msprintf(gettext("%s: A Fortran or C compiler is required.\n"), "ilib_build"));
        return;
    end

    [lhs,rhs] = argn(0);
    if rhs < 4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), "ilib_build"));
        return
    end

    if type(ilib_name) <> 10 then
        error(999, msprintf(_("%s: Wrong type for input argument #%d: string expected.\n"), "ilib_build", 1));
    end

    if size(ilib_name,"*") <> 1 then
        error(999, msprintf(_("%s: Wrong size for input argument #%d: string expected.\n"), "ilib_build", 1));
    end

    if type(table) <> 10 then
        error(999,msprintf(_("%s: Wrong type for input argument #%d: A matrix of strings expected.\n"), "ilib_build", 2));
    end

    if size(table,"*") > 999 * 2 then
        error(999,msprintf(_("%s: Wrong size for input argument #%d: A matrix of strings < 999 expected.\n"), "ilib_build", 2));
    end

    if getos() <> "Windows" & strncpy(ilib_name, 3) <> "lib" then
        // We add a leading lib under Linux/Unix because it is the way
        ilib_name = "lib" + ilib_name;
    end

    if ~isempty(files) & (or(fileext(files)==".o") | or(fileext(files)==".obj")) then
        error(999, msprintf(_("%s: A managed file extension for input argument #%d expected."), "ilib_build", 3));
    end

    if rhs > 4 then
        if (makename <> [] & makename <> "") then
            warning(msprintf(_("%s: Wrong value for input argument #%d: """" or ""[]"" expected.\n"), "ilib_build", 5));
        end
    end

    if ~isempty(files) & ~and(isfile(files)) then
        error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected.\n"), "ilib_build", 3));
    end


    if rhs <= 5 then ldflags = ""; end
    if rhs <= 6 then cflags  = ""; end
    if rhs <= 7 then fflags  = ""; end
    if rhs <= 8 then ismex  = %f; end
    if rhs <= 9 then cc  = ""; end

    if getos() == "Windows" then
        if ~isdef("makename") | (makename == "") | (makename == []) then
            // Load dynamic_link Internal lib if it's not already loaded
            if ~exists("dynamic_linkwindowslib") then
                load("SCI/modules/dynamic_link/macros/windows/lib");
            end
            makename = dlwGetDefltMakefileName();
        end
    else
        makename = "Makefile";
    end

    // check if library is not already loaded
    if or(link() == ilib_name) then
        error(999, msprintf(_("%s: ""%s"" already loaded in scilab."),"ilib_build",ilib_name) + ..
        ascii(10) + _("You need to unload this library before."));
    end

    // generate the gateway file
    if ( ilib_verbose() <> 0 ) then
        mprintf(_("   Generate a gateway file\n"));
    end

    file_gw_name = ilib_gen_gateway(ilib_name, table);

    // generate a loader file
    if ( ilib_verbose() <> 0 ) then
        mprintf(_("   Generate a loader file\n"));
    end
    if ~ismex then
        ilib_gen_loader(ilib_name,table,libs);
    else
        ilib_gen_loader(ilib_name,table);
    end

    // generate a Makefile
    if ( ilib_verbose() <> 0 ) then
        mprintf(_("   Generate a Makefile\n"));
    end

    files = files(:)';
    files = unique([files, file_gw_name]);

    makename = ilib_gen_Make(ilib_name, table, files, libs, makename, %t, ldflags, cflags, fflags, cc);

    // we call make
    if ( ilib_verbose() <> 0 ) then
        mprintf(_("   Running the makefile\n"));
    end

    libn = ilib_compile(ilib_name, makename, files);

    if ( ilib_verbose() <> 0 ) then
        mprintf(_("   Generate a cleaner file\n"));
    end
    ilib_gen_cleaner(makename, "loader.sce", [libn; file_gw_name']);

endfunction
//=============================================================================

