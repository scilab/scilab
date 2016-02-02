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

// Run the help_path+"/"+lang+"/build_help.sce" script if it exists

function tbx_builder_help_lang(languages,path)

    // Number of input arguments

    rhs = argn(2);

    if and(rhs <> [1 2]) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"tbx_builder_help_lang",1,2));
    end

    // Input argument N°1
    // → languages

    if type(languages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string array expected.\n"),"tbx_builder_help_lang",1));
    end

    // Input argument N°2
    // → path

    if rhs < 2 then
        path = pwd();

    else
        if type(path) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"tbx_builder_help_lang",2));
        end

        if size(path,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"tbx_builder_help_lang",2));
        end

        if ~isdir(path) then
            error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder_help_lang",path));
        end
    end

    // Check path/languages

    directories = pathconvert(path+"/"+languages,%F);

    for i=1:size(directories,"*") do
        if ~isdir(directories(i)) then
            error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder_help_lang",directories(i)));
        end
    end

    // exec all build_help.sce
    tbx_builder(pathconvert(directories+"/build_help.sce",%F));

endfunction
