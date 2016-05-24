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

function [status, msg] = xcosPalAdd(pal, category)
    // Add a palette to the Scilab/Xcos palette manager. Optional property can be added to set the category of the palette.
    //
    // Syntax
    //   xcosPalAdd(pal);
    //   xcosPalAdd(pal, category);
    //   status = xcosPalAdd(pal);
    //   status = xcosPalAdd(pal, category);
    //   [status, msg] = xcosPalAdd(pal);
    //   [status, msg] = xcosPalAdd(pal, category);
    //
    // Parameters
    //   pal: palette tlist or path string; the palette to add
    //   category: string array; the optional category path to use
    //   status: boolean; the status of the operation
    //   msg: string; the error message
    //
    // Description
    // After setting the blocks into the palette, the user add this function to add a Scilab palette to the Xcos palette manager.
    //
    // The optional category argument can be used to add the palette to a customized category path. If not specified, the root category is used.
    //
    // Examples
    //   loadXcosLibs();
    //   pal = xcosPal();
    //   pal = xcosPalAddBlock(pal, "SUM_f");
    //   pal = xcosPalAddBlock(pal, "BIGSOM_f");
    //
    //   xcosPalAdd(pal, "my Summation blocks")
    //
    // See also
    //   xcosPal
    //   xcosPalAddBlock
    //
    // Authors
    //   Clément DAVID

    status = %F;
    msg = "";

    // Checking arguments
    [lhs,rhs] = argn(0)
    if rhs < 1 | rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "xcosPalAdd", 1, 2));
    end

    if lhs > 3 then
        error(msprintf(gettext("%s: Wrong number of output arguments: %d to %d expected.\n"), "xcosPalAdd", 1, 2));
    end

    if typeof(pal) <> "palette" & typeof(pal) <> "string" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": palette type or path expected.\n"), "xcosPalAdd", "pal"));
    end

    if typeof(pal) == "string" & ~isfile(pal) then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": File not found.\n"), "xcosPalAdd", "pal"));
    end

    if exists("category", "l") == 0 then category="", end

    if typeof(category) <> "string" & or(size(category) == [1 1]) then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": string vector expected.\n"), "xcosPalAdd", "pal"));
    end

    if typeof(pal) == "string" & isfile(pal) then
        path = pal;

        clear pal
        load(path);
        if ~exists("pal", "l") then
            error(msprintf(gettext("%s: Wrong type for input argument ""%s"": palette type or path expected.\n"), "xcosPalAdd", "pal"));
        end
    end

    // call the gateway with the variable name and the category as
    // a string vector
    xcosPalLoad("pal", category);
    status = %T;
endfunction

