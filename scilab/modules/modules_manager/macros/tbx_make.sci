// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
// Copyright (C) 2016 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [] = tbx_make(tbx_path, sections)

    // Description
    //     Builds a toolbox if it follows the toolbox directories convention
    //
    // Syntax
    //     tbx_make
    //     tbx_make(tbx_path[, sections])
    //
    // Parameters
    //     tbx_path: a single string; path to the toolbox directory
    //     sections: a vector of strings, giving the sections subdirectories to
    //               compile, amongst
    //               ["macros", "help", "src", "sci_gateway", "localization"].
    //               If sections argin is not provided or is empty, tbx_make() makes
    //               all existing sections subdirectories among the default set.
    //
    // Description
    //     Builds a toolbox by executing the builder script.
    //     If no builder script is found, builds the toolbox using the directory
    //     naming conventions.
    //
    // Examples
    //     // Builds the toolbox mytoolbox at current location
    //     tbx_make() // build the toolbox in the current working directory
    //     tbx_make("./", "macros") build the macros in current working directory
    //
    //     // Builds the toolbox located in path/to/mytoolbox directory
    //     tbx_make("path/to/mytoolbox")
    //
    //     // Compiles macros help and src for the toolbox located in path/to/mytoolbox
    //     tbx_make("path/to/mytoolbox", ["macros", "help", "src"])
    //

    fname = "tbx_make"
    // Default values
    if ~isdef("tbx_path","l") then
        tbx_path = pwd()
    end
    if ~isdef("sections","l") || sections==[] then
        sections = ["macros", "help", "src", "sci_gateway", "localization"]
    end

    // Arguments checking
    msg = _("%s: Argument #%d: Text(s) expected.\n");
    if type(tbx_path) <> 10 then
        error(msprintf(msg, fname, 1));
    end
    if type(sections) <> 10 then
        error(msprintf(msg, fname, 2));
    end
    if size(tbx_path, "*") <> 1 then
        msg = _("%s: Argument #%d: Scalar (1 element) expected.\n");
        error(msprintf(msg, fname, 1));
    end
    if ~isdir(tbx_path) then
        msg = _("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n")
        error(msprintf(msg, fname, path))
    end

    // Default is to exec builder (in current directory or given tbx_path)
    buildscript = findfiles(tbx_path, "build*.sce");
    if ~isempty(buildscript)
        exec(tbx_path + "/" + buildscript);
        return
    end

    // Set the actual set of sections = only available ones among selected ones.
    tmp = dir(tbx_path)
    sections = sections(members(sections, tmp.name(tmp.isdir))>0)

    // Build sections asked
    if ( or(sections == "macros")  )
        tbx_builder_macros(tbx_path);
    end

    if ( or(sections == "src")  )
        tbx_builder_src(tbx_path);
    end

    if ( or(sections == "sci_gateway")  )
        tbx_builder_gateway(tbx_path);
    end

    if ( or(sections == "localization")  )
        tbx_build_localization(tbx_path);
    end

    if ( or(sections == "help")  )
        tbx_builder_help(tbx_path);
    end

    tbx_build_loader(tbx_path);
    tbx_build_cleaner(tbx_path);

endfunction
