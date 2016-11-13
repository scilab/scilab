// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
// Copyright (C) 2010 - DIGITEO - Allan CORNET
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

// Run the module+"/src/builder_src.sce" script if it exists

function tbx_builder_src(module)

    // Number of input arguments

    if argn(2)<>1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"tbx_builder_src",1));
    end

    // Input argument N°1
    // → module

    if type(module) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"tbx_builder_src",1));
    end

    if size(module,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"tbx_builder_src",1));
    end

    if ~isdir(module) then
        error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder_src",module));
    end

    // Check the compiler

    if ~haveacompiler() then
        error(msprintf(gettext("%s: This module requires a C or Fortran compiler and it has not been found.\n"),"tbx_builder_src"));
    end


    // check there is a builder file is  present and if so execute it with tbx_builder
    builder_src_dir = module + "/src/"
    if isdir(builder_src_dir)
        mprintf(gettext("Building sources...\n"));
        builder_src_file = findfiles(builder_src_dir, "build*.sce");
        if ~isempty(builder_src_file)
            builder_src_file = pathconvert(builder_src_dir + "/" + builder_src_file, %F);
            tbx_builder(builder_src_file);
        else
            // Default mode look in directories to find builder files and execute them
            d = dir(builder_src_dir);
            languages = d.name(d.isdir);
            if isempty(languages)
                warning(_("No builder file found, nothing to be done"));
                return
            end
            tbx_builder_src_lang(languages, builder_src_dir);
        end
    end
endfunction
