// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function blocks = xcosPalGenerateAllIcons(path)
    // 0. Check number of arguments
    [lhs,rhs]=argn();
    if rhs<>1 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"xcosPalGenerateAllIcons",1));
    end
    if lhs<>1 then
        error(msprintf(gettext("%s: Wrong number of output arguments: %d expected.\n"),"xcosPalGenerateAllIcons",1));
    end
    // 1. Check type
    if typeof(path)<>"string" then
        error(sprintf(gettext("%s: Wrong type for argument #%d: Matrix of strings expected.\n"), "xcosPalGenerateAllIcons", 1));
    end
    // 2. Check size
    if and(size(path)<>1) then
        error(sprintf(gettext("%s: Wrong size for input argument #%d: Vector expected.\n"), "xcosPalGenerateAllIcons" , 1));
    end

    // 3. check the argument validity
    xcosPalGet(path); // will push a 'pal' mlist

    if size(pal.blockNames, "*") <= 0 then
        error(sprintf(gettext("%s: Wrong value for input argument #%d: No block found.\n"), "xcosPalGenerateAllIcons" ,1));
    end

    blocks = [];
    for i=1:size(pal.blockNames, "*")
        name = pal.blockNames(i);
        iconPath = pal.icons(i);

        // load the block
        execstr("blk="+name+"(''define'');");

        // filter-out the text block
        if typeof(blk) == "Text" then
            continue,
        end

        // generate the icon
        xcosPalGenerateIcon(iconPath);

        blocks($+1) = name;
    end
endfunction
