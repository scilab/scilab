//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2009-2009 - Vincent COUVERT
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
//
//

// Generate xcos palette block icons and graph images from scicos pal files.
//
// @param palFiles set of string of palette files
// @param iconsOutPath output path of the palette icons (GIF files).
//             The default is the common `palFiles' dir.
// @param imagesOutPath output path of the graph images (SVG files).
//             The default is the common `palFiles' dir.
// @param[opt] traceEnable %T if a trace output must be printed, %F if not (default=%F).
function generateBlockImages(palFiles, iconsOutPath, imagesOutPath, traceEnable)
    [lhs, rhs] = argn(0);
    if rhs < 3 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "generateBlockImages", 3));
    end

    if typeof(iconsOutPath) <> "string" | ~isdir(iconsOutPath) then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": directory path string expected.\n"), "generateBlockImages", "iconsOutPath"));
    end

    if typeof(imagesOutPath) <> "string" | ~isdir(imagesOutPath) then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": directory path string expected.\n"), "generateBlockImages", "imagesOutPath"));
    end

    if exists("traceEnable", "l") == 0 then
        traceEnable = %f;
    else
        if typeof(traceEnable) <> "boolean" then
            error(msprintf(gettext("%s: Wrong type for input argument ""%s"": boolean expected.\n"), "generateBlockImage", "traceEnable"));
        end
    end

    // call loadXcosLibs if not loaded
    if exists("scicos_diagram", "a") == 0 then loadXcosLibs(); end

    if traceEnable then
        ncl = lines(), lines(0);
    end

    // iterator on all blocks
    for fIndex = 1:size(palFiles, "*")

        if ~isfile(palFiles(fIndex)) then
            if traceEnable then
                mprintf(gettext("%s: File ''%s'' does not exist.\n"), "generateBlockImages", palFiles(fIndex));
            end
            continue;
        end

        exec(palFiles(fIndex), -1);

        if isempty("scs_m") then
            if traceEnable then
                mprintf(gettext("%s: File ''%s'' is not a valid palette file.\n"), "generateBlockImages", palFiles(fIndex));
            end
            continue;
        end

        for iBlock = 1:size(scs_m.objs)
            block = scs_m.objs(iBlock);

            if typeof(block)=="Block" & block.gui == "PAL_f" then
                // Add PAL_f children blocks
                children = block.model.rpar.objs;
                for jBlock = 1:size(children)
                    varsToLoad($+1) = children(jBlock).gui;
                end
            elseif typeof(block)=="Block" then
                // old scicos palettes doesn't have a PAL_f block but directly
                // the reference instances instead.
                varsToLoad($+1) = block.gui
            else
                if traceEnable then
                    mprintf(gettext("%s: Found ''%s'' instead of a block.\n"), "generateBlockImages", typeof(block));
                end
                continue;
            end
        end

        clear scs_m;
    end

    varsToLoad = gsort(varsToLoad, "r", "i");
    for kBlock = 1 : size(varsToLoad, "*")
        ierr = execstr("scs_m  = " + varsToLoad(kBlock) + "(""define"")", "errcatch");
        if traceEnable then
            mprintf("%d: %s",  kBlock, varsToLoad(kBlock));
        end
        if ierr == 0 then
            status = generateBlockImage(scs_m, imagesOutPath, imageType=imageType, %f);
            if status & traceEnable then
                mprintf(" SUCCEED\n");
            elseif traceEnable then
                mprintf(" FAILED\n");
            end
        elseif traceEnable then
            mprintf(" FAILED\n");
        end
    end

    if traceEnable then
        lines(ncl);
    end
endfunction

function c=scs_color(c)
endfunction
