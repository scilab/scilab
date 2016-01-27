// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
// === LICENSE_END ===

// Run the module+"/sci_gateway/builder_gateway.sce" script if it exists

function tbx_builder_gateway(module)

    // Number of input arguments

    if argn(2)<>1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"tbx_builder_gateway",1));
    end

    // Input argument N°1
    // → module

    if type(module) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"tbx_builder_gateway",1));
    end

    if size(module,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"tbx_builder_gateway",1));
    end

    if ~isdir(module) then
        error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder_gateway",module));
    end

    // Check the compiler

    if ~haveacompiler() then
        error(msprintf(gettext("%s: This module required a C compiler and it has not been found.\n"),"tbx_builder_gateway"));
    end


    mprintf(gettext("Building gateway...\n"));
    tbx_builder(pathconvert(module+"/sci_gateway/builder_gateway.sce",%F));

endfunction
