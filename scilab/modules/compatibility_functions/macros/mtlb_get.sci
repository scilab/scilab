// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function v=mtlb_get(H,property)

    [lhs,rhs]=argn()
    win=xget("window")
    xset("window",H)
    if rhs==1 then
    else
        v=[]
        select convstr(property)
        case "backingstore" then
        case "color" then
        case "colormap" then
            v=xget("colormap")
        case "currentaxes" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "currentcharacter" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "currentmenu" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "currentobject" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "currentpoint" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "fixedcolors" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "inverthardcopy" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "keypressfcn" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "menubar" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "mincolormap" then
            v=xget("colormap")
            v=size(v,1)
        case "name" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "nextplot" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "numbertitle" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "paperunits" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "paperorientation" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "paperposition" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "papersize" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "papertype" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "pointer" then
            v="arrow"
        case "position" then
            o=xget("wpos")
            sz=xget("wdim")
            v=[o(:);sz(:)]'
        case "resize" then
            v="on"
        case "resizefcn" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "selectiontype" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "sharecolors" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "units" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "windowbuttondownfcn" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "windowbuttonmotionfcn" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "windowbuttonupfcn" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "buttondownfcn" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "children" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "clipping" then
            v=xget("clipping")
            if v(1)<>0 then v="on",else v="off",end
        case "interruptible" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "parent" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "type" then
            v="figure"
        case "userdata" then
            error(msprintf(gettext("%s: No equivalent for ''%s'' property.\n"), "mtlb_get", property));
        case "visible" then
            v="on"
        end
    end
    xset("window",win)
endfunction
