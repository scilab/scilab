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

// Create a new palette
// 
// @param[opt] name the palette name
// @param[opt] scs_m the source scicos palette to import
// @return pal the palette instance.
function pal = xcosPal(name, scs_m)
    [lhs,rhs] = argn(0);
    
    if rhs > 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "xcosPal", 0, 2));
    end
    
    if exists("name", 'l') == 0 then
        name = "New palette";
    elseif typeof(name) == "diagram" then
        scs_m = name;
        name = "New palette";
    end
    // loading the scicos_diagram macro
    if exists("scicos_diagram", 'a') == 0 then loadScicosLibs(), end
    if exists("scs_m", 'l') == 0 then scs_m = scicos_diagram(), end
    
    if typeof(name) <> "string" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": string type expected.\n"), "xcosPal","name"));
    end
    
    if typeof(scs_m) <> "diagram" then
        error(msprintf(gettext("%s: Wrong type for input argument ""%s"": diagram type expected.\n"), "xcosPal","scs_m"));
    end
    
    // default instance
    pal = tlist(["palette", "name", "blockNames",  "blocks", "icons", "style"],..
                            [],     [],            [],       [],      []);
                            
    // find palette name
    if ~isempty(scs_m.objs) then
        name = scs_m.props.title(1);
    end
    
    // getting a block name list
    varsToLoad = [];
    for block = scs_m.objs
        if typeof(block)=="Block" & block.gui == "PAL_f" then
            // customize palette name
            name = block.model.rpar.props.title(1);
            
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
    for index = 1:size(varsToLoad, '*')
        pal = xcosPalAddBlock(pal, varsToLoad(index));
    end
endfunction

