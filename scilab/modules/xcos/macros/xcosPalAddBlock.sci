// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Clément DAVID
// Copyright (C) - 2011 - Scilab Enterprises - Clément DAVID
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function pal = xcosPalAddBlock(pal, block, pal_block_img, style)

    //  Add a block to a Scilab/Xcos palette instance. Some optional properties can be added to customize the palette icon and the style of the block.
    //
    // Syntax
    //  pal = xcosPalAddBlock(pal, block);
    //  pal = xcosPalAddBlock(pal, block, pal_block_img);
    //  pal = xcosPalAddBlock(pal, block, [], style);
    //  pal = xcosPalAddBlock(pal, block, pal_block_img, style);
    //
    // Parameters
    //  pal: the palette to update
    //  block: the block to add to the palette
    //  pal_block_img: the block icon to use on the palette manager.
    //  style: the style to apply to the block
    //
    // Description
    //  This macros add a block instance to a palette. This block parameter can be an instantiated block or a name (interface-function) or a path to a saved instance. Some operations are performed to load this block and check it's availability so it's interface-function must be loaded on Scilab. Some temporary files are also generated without full path arguments.
    //
    //  The optional pal_block_img argument is generated on the <link linkend="TMPDIR">TMPDIR</link> using Scilab graphics if not specified.Be careful that if you use our palette to be persistent you then need to specify it. Otherwise the generated image will be deleted at the end of the Scilab session.
    //
    //  The optional style argument allow the user to determine the kind of style to be used by this block. This argument can be typed as a path <link linkend="string">string</link> or a <link linkend="struct">struct</link>. If it is a string then a default style value is generated and formatted as a style else a struct is wrapped to a key-value jgraphx settings. The <link linkend="jgraphx-style-list">following style</link> correspond to jgraphx version 1.4.0.2. These style keys can change with new version of jgraphx without any warranty.
    //
    //
    // Examples
    //  loadXcosLibs();
    //  pal = xcosPal();
    //
    //  sumPath = TMPDIR + "/sum.sod";
    //  bigSomPath = TMPDIR + "/sum.sod";
    //
    //  scs_m = SUM_f("define");
    //  save(sumPath, "scs_m");
    //  scs_m = BIGSOM_f("define");
    //  save(bigSomPath, "scs_m");
    //
    //  pal = xcosPalAddBlock(pal, sumPath);
    //  pal = xcosPalAddBlock(pal, bigSomPath);
    //
    //  xcosPalAdd(pal);
    //
    // See also
    //  xcosPal
    //  xcosPalAdd
    //
    // Authors
    //  Clément DAVID
    //  Yann COLLETTE

    // Checking arguments
    [lhs,rhs] = argn(0)
    if rhs < 2 | rhs > 5 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "xcosPalAddBlock", 2, 5));
    end

    if lhs > 1 then
        error(msprintf(gettext("%s: Wrong number of output arguments: %d expected.\n"), "xcosPalAddBlock", 1));
    end

    if typeof(pal) <> "palette" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": palette type expected.\n"), "xcosPalAddBlock", "pal"));
    end

    // check and tranform block argument
    if typeof(block) == "Block" then
        scs_m = block;
    elseif typeof(block) == "string" & isfile(block) then
        fd = mopen(block, "rb");
        [err, msg] = merror(fd);
        if err <> 0 then
            error(msg);
        end
        block = fullpath(block);
        mclose(fd);

        // store the block instance
        try
            load(block);
        catch
            error(msprintf(gettext("%s: Unable to load block from ""%s"": hdf5 file expected.\n"), "xcosPalAddBlock", block));
        end

        if exists("scs_m", "l") == 0 then
            error(msprintf(gettext("%s: Unable to load block from ""%s"": no `scs_m'' variable found.\n"), "xcosPalAddBlock", block));
        end
    elseif typeof(block) == "string" & exists(block) <> 0 & typeof(evstr(block)) == "function" then
        execstr("scs_m = " + block + "(""define"");");
    else
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": function as string or Block type or full path string expected.\n"), "xcosPalAddBlock", "block"));
    end

    // at this point we can assert that `scs_m' is a full path string to a
    // block reference instance.

    // now handle pal_block_img argument
    if ~exists("pal_block_img", "l") | isempty(pal_block_img) then
        // block icon by default
        pal_block_img = TMPDIR + "/" + scs_m.gui + ".gif";
        if isfile(pal_block_img) then
            error(msprintf(gettext("%s: Unable to generate the palette icon : ""%s"" already exists.\n"), "xcosPalAddBlock", pal_block_img));
        end
    else
        // specified block icon
        if typeof(pal_block_img) <> "string" then
            error(msprintf(gettext("%s: Wrong type for input argument ""%s"": path string expected.\n"), "xcosPalAddBlock", "pal_block_img"));
        end

        if ~isfile(pal_block_img) then
            error(msprintf(gettext("%s: Wrong value for input argument ""%s"": An existing file expected.\n"), "xcosPalAddBlock", "pal_block_img"));
        end

        valid_ext = ["png" "jpg" "gif" "PNG" "JPG" "JPEG" "GIF"];
        ext = strrchr(pal_block_img, ".");
        if isempty(strstr(emptystr(valid_ext) + ext, valid_ext)) then
            error(msprintf(gettext("%s: Wrong value for input argument ""%s"": A valid file format (png, jpg, gif) expected.\n"), "xcosPalAddBlock", "pal_block_img"));
        end
        clear valid_ext ext;
        pal_block_img = fullpath(pathconvert(pal_block_img, %f));
    end



    // now handle style argument
    if ~exists("style", "l") | isempty(style) then
        // style by default
        block_img = TMPDIR + "/" + scs_m.gui + ".svg";
        // protect drive letter
        if getos() == "Windows" then
            block_img = "/" + block_img;
        end
        style = "noLabel=1;image=file://" + block_img + ";";
        status = generateBlockImage(scs_m, TMPDIR, imageType="svg", withPort=%f);
        if ~status then
            error(msprintf(gettext("%s: Unable to generate the image ""%s"".\n"), "xcosPalAddBlock", block_img));
        end
    else
        // apply the specified style to the block
        if and(typeof(style) <> ["st" "string"]) then
            error(msprintf(gettext("%s: Wrong type for input argument ""%s"": string or struct expected.\n"), "xcosPalAddBlock", "style"));
        elseif typeof(style) == "st" then
            formattedStyle = "";
            fields = fieldnames(style);
            fieldsSize = size(fields, "*");
            for i=1:fieldsSize
                formattedStyle = formattedStyle + fields(i);
                fieldValue = getfield(fields(i), style);
                if ~isempty(fieldValue) then
                    formattedStyle = formattedStyle+ "=" + string(fieldValue);
                end
                formattedStyle = formattedStyle + ";";
            end
            style = formattedStyle;
        elseif typeof(style) == "string" then
            if isfile(style) then
                // protect drive letter
                if getos() == "Windows" then
                    style = "/" + style;
                end
                style = "shape=label;image=file://" + style + ";";
                //          else
                //              assume a well formatted string, do nothing
            end
        end
    end

    // Store the data into the palette structure
    pal.blockNames($+1) = scs_m.gui // block named class
    pal.icons($+1) = pal_block_img; // palette icon full path string
    pal.style($+1) = style; // block style (linked to style definition)

endfunction

