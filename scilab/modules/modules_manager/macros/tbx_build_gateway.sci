// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Simple wrapper around ilib_build
function tbx_build_gateway(libname,      ..
                           names,        ..
                           files,        ..
                           gateway_path, ..
                           libs,         ..
                           ldflags,      ..
                           cflags,       ..
                           fflags,       ..
                           cc,           ..
                           makename,     ..
                           ismex)


    rhs = argn(2);
    currentdirectory = pwd();

    // Number of input arguments

    if rhs<3 | rhs>11 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"tbx_build_gateway",3,11))
    end

    // Input argument N°1
    // → libname

    if type(libname) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"tbx_build_gateway",1));
    end

    if size(libname,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"tbx_build_gateway",1));
    end

    // Input argument N°2
    // → table

    if type(names) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A matrix of strings expected.\n"),"tbx_build_gateway",2));
    end

    if size(names,'r') > 999 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: At most %d rows expected.\n"),"tbx_build_gateway",2,999));
    end

    // Input argument N°3
    // → files

    if type(files) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A matrix of strings expected.\n"),"tbx_build_gateway",3));
    end

    // Input argument N°4
    // → gateway_path

    if rhs<4 then
        gateway_path = currentdirectory;

    else
        if type(gateway_path) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"tbx_build_gateway",4));
        end

        if size(gateway_path,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"tbx_build_gateway",4));
        end

        if ~isdir(gateway_path) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_build_gateway",4,gateway_path));
        end

        if ~chdir(gateway_path) then
            error(msprintf(gettext("%s: Wrong value for input argument #%d: Can''t go to ''%s''.\n"),"tbx_build_gateway",4,gateway_path));
        end
    end

    // Input argument N°5
    // → libs

    if rhs<5 then
        libs = [];
    end

    // Input argument N°6
    // → ldflags

    if rhs<6 then
        ldflags = "";
    end

    // Input argument N°7
    // → cflags

    if rhs<7 then
        cflags = "";
    end

    // Input argument N°8
    // → fflags

    if rhs<8 then
        fflags = "";
    end

    // Input argument N°9
    // → cc

    if rhs<9 then
        cc = "";
    end

    // Input argument N°10
    // → makename

    if rhs<10 then
        makename = "";
    end

    // Input argument N°11
    // → ismex

    if rhs<11 then
        ismex = %F;

    else
        if type(ismex) <> 4 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"tbx_build_gateway",11));
        end

        if size(ismex,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A boolean expected.\n"),"tbx_build_gateway",11));
        end
    end

    ilib_build(libname,  ..
               names,    ..
               files,    ..
               libs,     ..
               makename, ..
               ldflags,  ..
               cflags,   ..
               fflags,   ..
               ismex,    ..
               cc);

    if ~chdir(currentdirectory) then
        error(msprintf(gettext("%s: Can''t go to directory ''%s''.\n"),"tbx_builder_gateway",currentdirectory));
    end

endfunction
