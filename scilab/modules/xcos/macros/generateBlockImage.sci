//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

// Generate a block image from the instance.
//
// @param block the block instance
// @param path output file path
// @param[opt] filename the file name to use (without extension).
//             The default is `block.gui'.
// @param[opt] imageType the exported image type. only "svg", "gif", "jpg" is
//             supported. The default is to use "gif".
// @param[opt] withPort true if the exported image should contains the port,
//             false otherwise. The default is value is true.
// @return status %T if the operation has been successfull, %F otherwise.
function status = generateBlockImage(block, path, filename, imageType, withPort)
    status = %f;

    // call loadXcosLibs if not loaded
    if exists("scicos_diagram", "a") == 0 then loadXcosLibs(); end

    [lhs,rhs] = argn(0)
    if rhs < 2 | rhs > 6 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "generateBlockImage", 2, 4));
    end

    if typeof(block) <> "Block" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": Block type expected.\n"), "generateBlockImage", "block"));
    end

    if typeof(path) <> "string" | ~isdir(path) then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": directory path string expected.\n"), "generateBlockImage", "path"));
    end

    // generate a default graphic or clear the existing one
    if exists("imageType", "l") == 0 then
        imageType = "gif";
    else
        if typeof(imageType) <> "string" | and(imageType <> ["svg", "gif", "jpg"]) then
            error(msprintf(gettext("%s: Wrong input argument ""%s"": ""svg"", ""gif"" or ""jpg"" expected.\n"), "generateBlockImage", "imageType"));
        end
    end

    if exists("withPort", "l") == 0 then
        withPort = %t;
    end

    // set the default outFile
    if exists("filename", "l") == 0 then
        outFile = path + "/" + block.gui + "." + imageType;
    else
        if typeof(filename) <> "string" | size(filename, "*") <> 1 then
            error(msprintf(gettext("%s: Wrong type for input argument ""%s"": string expected.\n"), "generateBlockImage", "filename"));
        end
        outFile = path + "/" + filename + "." + imageType;
    end

    // if the gr_i value of a bloc is empty, return
    if (block.graphics.gr_i(1) == []) then
        mputl("<svg/>", outFile);
        status = %t;
        return
    end

    // set export properties before creating any graphic object (including any figure)
    previous_driver = driver(imageType);
    xinit(outFile);

    handle = gcf();

    if ~withPort then
        prot = funcprot();
        function standard_draw_port(varargin)
        endfunction
        standard_draw_port_up = standard_draw_port;
        funcprot(prot);
    end

    // constants
    diagram = scicos_diagram();
    options = diagram.props.options;
    options("3D")(1) = %f;
    sz = block.graphics.sz;
    orig = block.graphics.orig;

    gh_axes = gca();
    gh_curwin = handle;

    // draw settings
    // note that the gh_axes variable have to be known on the "plot" call
    isoview("on")
    gh_axes.fractional_font = "off";
    gh_axes.arc_drawing_method = "lines";
    o_size = size(gh_axes.children);
    gh_axes.data_bounds = [orig(1), orig(2); sz(1), orig(2)+sz(2)];
    gh_axes.margins = 0.01 * ones(1, 4);
    gh_axes.box ="off";
    handle.axes_size = [max(20, 20 * sz(1)), max(20, 20 * sz(2))];

    // Create variable o because needed inside "plot"
    o = block;
    ierr = execstr("block  = " + o.gui + "(""plot"",o)", "errcatch");
    if (ierr <> 0) then
        return;
    end

    // export
    try
        xend();
        status = %t;
    catch
        status = %f;
    end

    // post operations
    driver(previous_driver);
endfunction

