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

    builder_gateway_dir = pathconvert(module+"/sci_gateway",%F);
    if isdir(builder_gateway_dir)
        mprintf(gettext("Building gateway...\n"));
        if ~isempty(findfiles(builder_gateway_dir, "builder*.sce"))
            builder_gateway_file = pathconvert(builder_gateway_dir + "/" + findfiles(builder_gateway_dir, "builder*.sce"), %F);
            tbx_builder(builder_gateway_file);
        else
            // Default mode try to build the gateways in language folders inside sci_gateway/
            d = dir(builder_gateway_dir);
            languages = d.name(d.isdir);
            tbx_builder_gateway_lang(languages, builder_gateway_dir); // executes the builder_gateway
            tbx_build_gateway_loader(languages, builder_gateway_dir);
            // TODO: not sure the cleaner is necessary here because it must be manually created for each language...
            // A user defined cleaner seems more appropriate
            tbx_build_gateway_clean(languages, builder_gateway_dir);
        end
    end

endfunction
