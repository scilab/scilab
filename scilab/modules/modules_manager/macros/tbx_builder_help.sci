// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Run the modules+"/help/builder_help.sce" script if it exists
// See devtools_run_builder

function tbx_builder_help(module)

    // Number of input arguments

    if argn(2)<>1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"tbx_builder_help",1));
    end

    // Input argument N°1
    // → module

    if type(module) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"tbx_builder_help",1));
    end

    if size(module,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"tbx_builder_help",1));
    end

    if ~isdir(module) then
        error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder_help",module));
    end


    mprintf(gettext("Building help...\n"));
    tbx_builder(pathconvert(module+"/help/builder_help.sce",%F));

endfunction
