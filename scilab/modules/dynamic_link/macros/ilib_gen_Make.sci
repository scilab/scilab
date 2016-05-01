// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
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
function Makename = ilib_gen_Make(name, ..
    tables, ..
    files, ..
    libs, ..
    makename, ..
    with_gateway, ..
    ldflags, ..
    cflags, ..
    fflags, ..
    cc)

    [lhs,rhs] = argn(0);
    if rhs < 4 then
        error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), "ilib_gen_Make"));
        return
    end

    if argn(2) < 6 then
        with_gateway = %t
        ldflags = "";
        cflags = "";
        fflags = "";
        cc = "";
    end

    if ~isempty(files) & ~and(isfile(files)) then
        error(999, msprintf(_("%s: Wrong value for input argument #%d: existing file(s) expected.\n"), "ilib_gen_Make", 3));
    end

    // remove duplicate files
    files = unique(files);

    if getos() <> "Windows" then

        // change table if necessary
        if typeof(tables)<>"list" then
            tables= list(tables)
        end
        L = length(tables);
        for it = 1:L
            table = tables(it);
            [mt, nt] = size(table);

            if nt == 2 then
                col = "csci";
                table = [table, col(ones(mt,1))];
                nt = 3;
            end

            if nt <> 3 then
                error(msprintf(gettext("%s: Wrong size for input argument #%d.\n"), "ilib_gen_Make", 2));
            end

            tables(it) = table;
        end

        Makename = makename;
        ilib_gen_Make_unix(name, files, libs, name, ldflags, cflags, fflags, cc, tables)
    else
        //
        // Windows
        //
        // Load dynamic_link Internal lib if it's not already loaded
        if ~ exists("dynamic_linkwindowslib") then
            load("SCI/modules/dynamic_link/macros/windows/lib");
        end
        library_name = []; // auto. name library
        Makename = dlwGenerateMakefile(name, ..
        tables, ..
        files, ..
        libs, ..
        library_name, ..
        with_gateway, ..
        ldflags, ..
        cflags, ..
        fflags, ..
        cc);
    end
endfunction
//=============================================================================
