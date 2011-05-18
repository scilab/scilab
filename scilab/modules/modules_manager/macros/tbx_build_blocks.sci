// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Clément DAVID <clement.david@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function tbx_build_blocks(module, names)
    // Build a default block instance
    //
    // Calling Sequence
    //   tbx_build_blocks(module, names)
    //
    // Parameters
    // module: toolbox base directory
    // names: list of block names (sci file name without extension)

    if argn(2) <> 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"tbx_build_blocks",2));
    end

    // checking module argument
    if type(module) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"tbx_build_blocks",1));
    end
    if size(module,"*") <> 1 then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A string expected.\n"),"tbx_build_blocks",1));
    end
    if ~isdir(module) then
        error(msprintf(gettext("%s: The directory ''%s'' doesn''t exist or is not read accessible.\n"),"tbx_build_blocks",module));
    end

    // checking names argument
    if type(names) <> 10 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),"tbx_build_blocks",2));
    end

    mprintf(gettext("Building blocks...\n"));

    // load Xcos libraries when not already loaded.
    if ~exists("scicos_diagram") then loadXcosLibs(); end

    sciFiles = pathconvert(module + "/macros/") + names + ".sci";
    h5Files = pathconvert(module + "/images/h5/") + names + ".h5";
    gif_tlbx = pathconvert(module + "/images/gif");
    gifFiles = pathconvert(module + "/images/gif/") + names + ".gif";
    svg_tlbx = pathconvert(module + "/images/svg");
    svgFiles = pathconvert(module + "/images/svg/") + names + ".svg";
    handle = gcf();
    for i=1:size(names, "*")
        // load the interface function
        exec(sciFiles(i), -1);

        // export the instance
        execstr(msprintf("scs_m = %s (''define'');", names(i)));
        if ~export_to_hdf5(h5Files(i), "scs_m") then
            error(msprintf(gettext("%s: Unable to export %s to %s.\n"),"tbx_build_blocks",names(i), h5Files(i)));
        end

        block = scs_m;

        // export a gif file if it doesn't exist
        if ~isfile(gifFiles(i)) then
            if ~generateBlockImage(block, gif_tlbx, names(i), handle, "gif", %t) then
                error(msprintf(gettext("%s: Unable to export %s to %s.\n"),"tbx_build_blocks",names(i), gifFiles(i)));
            end
        end

        // export an svg file if it doesn't exist
        if ~isfile(svgFiles(i)) then
            if ~generateBlockImage(block, svg_tlbx, names(i), handle, "svg", %f) then
                error(msprintf(gettext("%s: Unable to export %s to %s.\n"),"tbx_build_blocks",names(i), svgFiles(i)));
            end
        end
    end

    delete(handle);
endfunction

