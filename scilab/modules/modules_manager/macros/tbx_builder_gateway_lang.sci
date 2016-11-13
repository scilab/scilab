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
            error(msprintf(gettext("%s: Wrong type for input argument #%d: string expected.\n"),"tbx_builder_gateway_lang",2));
        end

        if size(path,"*") <> 1 then
            error(msprintf(gettext("%s: Wrong size for input argument #%d: string expected.\n"),"tbx_builder_gateway_lang",2));
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
    //look for all files of type builder_something.sce and executes them
    builder_files = [];
    for i = 1:size(directories, "*")
        builder_file_i = findfiles(directories(i), "builder*.sce");
        if ~isempty(builder_file_i)
            tbx_builder(directories(i) + "/" + builder_file_i);
        else
            if languages(i) == "c"
                res = tbx_builder_gateway_c(fullpath(path));
                if ~res
                    msg = _("Could not use the default compilation of gateways in %s.");
                    warning(msprintf(msg, directories(i)));
                end
            else
                msg = _("No builder script found in %s.");
                warning(msprintf(msg, directories(i)));
            end
        end
    end

endfunction

function res = tbx_builder_gateway_c(path)
    res = %t;
    // The toolbox must have a name in order to create a library with its name
    // TODO: change this with information found in config.ini if tbx_config is validated
    if ~isempty( listfiles(path + "/../etc/*.start") )
        name_library = basename( listfiles(path + "/../etc/*.start") ) + "_c";
    else
        res = %f;
        return
    end

    // There must be C files to compile
    // functions will have the name of the C files without sci_ in front
    // automation is done only for the scilab 6 api_scilab
    files_in_library = listfiles(path + "/c/sci_*.c");
    if ~isempty(files_in_library)
        files_in_library = basename(listfiles(path + "/c/sci_*.c"));
        functions_in_library = part( files_in_library, 5:$ ); // functions are sci_foo without the sci_
        api_version = repmat("csci6", size(functions_in_library, "*"), 1); // only scilab 6 api_scilab supported
        src_library = basename( listfiles(path + "/../src/c/lib*" + getdynlibext()) );
        includes_src_c = ilib_include_flag(path + "/../src/c");

        tbx_build_gateway(name_library, .. // Name of the library to create
        [functions_in_library, files_in_library, api_version], .. // creation of the link between c functions and scilab gateways
        [files_in_library + ".c"], .. // path to the files
        path + "/c/", .. // path to the library
        path + "/../src/c/" + src_library, .. // linked library
        "", ..
        includes_src_c); // Include flags
    else
        res = %f;
    end
endfunction
