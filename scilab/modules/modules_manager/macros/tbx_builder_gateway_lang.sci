// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
// Copyright (C) 2010 - DIGITEO - Pierre MARECHAL
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run the builder_gateway_"+languages+".sce" scripts

// Input arguments:
//
// 1. languages
//        Languages of the sources files ("c", "cpp", "fortran", ...)
//
// 2. path
//        Path to the sources of the gateway ; in a normal toolbox it should be
//        the directory containing the builder_gateway.sce script (which should
//        be the script calling this function). Default is current directory.

function tbx_builder_gateway_lang(languages,path)

    // Number of input arguments

    rhs = argn(2);

    if and(rhs <> [1 2]) then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"tbx_builder_gateway_lang",1,2));
    end

    // Input argument N°1
    // → languages

    if type(languages) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string array expected.\n"),"tbx_builder_gateway_lang",1));
    end

    // Input argument N°2
    // → path

    if rhs < 2 then
        path = pwd();

    else
        if type(path) <> 10 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"tbx_builder_gateway_lang",2));
        end

        if size(path,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"tbx_builder_gateway_lang",2));
        end

        if ~isdir(path) then
            error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder_gateway_lang",path));
        end
    end

    // Check path/languages

    directories = pathconvert(path+"/"+languages,%F);

    for i=1:size(directories,"*") do
        if ~isdir(directories(i)) then
            error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_builder_gateway_lang",directories(i)));
        end
    end

    // Exec builders files
    tbx_builder(pathconvert(directories+"/builder_gateway_"+languages+".sce",%F));

endfunction
