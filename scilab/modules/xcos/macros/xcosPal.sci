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

function pal = xcosPal(name, scs_m)
    // Instantiate a new Xcos palette on Scilab.
    //
    // Syntax
    //   pal = xcosPal();
    //   pal = xcosPal(name);
    //   pal = xcosPal(scs_m);
    //   pal = xcosPal([], scs_m);
    //   pal = xcosPal(name, scs_m);
    //
    // Parameters
    //   name: string; the optional palette name
    //   scs_m: diagram mlist; the optional source diagram
    //   pal: palette tlist; the palette instance
    //
    // Description
    // Instantiate a new palette diagram.
    //
    // The optional name argument can be used to set a name to the palette. The optional scs_m argument can be used to import diagrams as palettes.
    //
    // Examples
    //   loadXcosLibs();
    //
    //   // from scratch
    //   pal = xcosPal("My sum palette");
    //   pal = xcosPalAddBlock(pal, "SUM_f");
    //   pal = xcosPalAddBlock(pal, "BIGSOM_f");
    //   xcosPalAdd(pal);
    //
    //   // from an old palette
    //   exec(SCI + "/modules/scicos/palettes/Integer.cosf", -1);
    //   pal = xcosPal(scs_m);
    //   xcosPalAdd(pal);
    //
    // See also
    //   xcosPal
    //   xcosPalAddBlock
    //
    // Authors
    //   Clément DAVID


    [lhs,rhs] = argn(0);

    if rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "xcosPal", 0, 2));
    end

    if ~isdef("name", "l") then
        name = "New palette";
    elseif typeof(name) == "diagram" then
        scs_m = name;
        name = "New palette";
    elseif isempty(name) then
        name = "New palette";
    end
    // loading the scicos_diagram macro
    if exists("Sourceslib", "a") == 0 then loadXcosLibs(), end
    if exists("scs_m", "l") == 0 then scs_m = scicos_diagram(), end

    if typeof(name) <> "string" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": string type expected.\n"), "xcosPal","name"));
    end

    if typeof(scs_m) <> "diagram" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": diagram type expected.\n"), "xcosPal","scs_m"));
    end

    // default instance
    pal = tlist(["palette", "name",  "blockNames", "icons", "style"],..
    [],        [],      [],      []);

    // find palette name
    if length(scs_m.objs) > 0 then
        name = scs_m.props.title(1);
    end

    // getting a block name list
    varsToLoad = [];
    for block = scs_m.objs
        if typeof(block)=="Block" & block.gui == "PAL_f" then
            // customize palette name
            name = scs_m.props.title(1);

            // Add PAL_f children blocks
            children = block.model.rpar.objs;
            for jBlock = 1:size(children)
                varsToLoad($+1) = children(jBlock).gui;
            end
        elseif typeof(block)=="Block" then
            // old scicos palettes doesn't have a PAL_f block but directly
            // the reference instances instead.
            varsToLoad($+1) = block.gui
        end
    end

    // set name
    pal.name = name;

    // add block instance by interfunction (will generate a new reference instance).
    for index = 1:size(varsToLoad, "*")
        pal = xcosPalAddBlock(pal, varsToLoad(index));
    end
endfunction

