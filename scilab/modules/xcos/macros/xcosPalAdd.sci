//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function [status, msg] = xcosPalAdd(pal, category)
// Add a palette to the Scilab/Xcos palette manager. Optional property can be added to set the category of the palette.
//
// Calling Sequence
//   xcosPalAdd(pal);
//   xcosPalAdd(pal, category);
//   status = xcosPalAdd(pal);
//   status = xcosPalAdd(pal, category);
//   [status, msg] = xcosPalAdd(pal);
//   [status, msg] = xcosPalAdd(pal, category);
//
// Parameters
//   pal: palette tlist; the palette to add
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
//   loadScicosLibs();
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
    
    if exists("category", 'l') == 0 then category="", end
    
    if typeof(category) <> "string" & or(size(category) == [1 1]) then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": string vector expected.\n"), "xcosPalAdd", "pal"));
    end
    
    if typeof(pal) == "palette" then
        myPalette = pal;
        pal = TMPDIR + "/palette.h5";
        [status, msg] = xcosPalExport(myPalette, pal);
        if ~status then
            return;
        end
    end
    
    // call the gateway with a full path string and the category as
    // a string vector
    xcosPalLoad(pal, category);
    status = %T;
endfunction

