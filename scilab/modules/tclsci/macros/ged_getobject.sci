// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2017 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [h, Axes] = ged_getobject(pt)
    // Internal private function called by ged_move_entity()
    // pt: [x,y] coordinates of the last left mouse click, in pixels from top-left
    // h: Handle of the graphic object
    h = [];
    Axes = [];
    f = gcf();
    aold = gca();
    Children = f.children
    //retains only the entities of type Axes or uicontrol (remove uimenus)
    axes_array = Children(Children.type=="Axes");
    uicontrol_array = Children(Children.type=="uicontrol");
    // Axes
    for k = 1:size(axes_array,"*")
        Axes = axes_array(k)
        set("current_axes", Axes)
        h = ged_loop(Axes, pt)
        if h<>[] then break,end
    end
    if h~=[] then
        set("current_axes",aold)
        return
    end
    // uicontrols
    Axes = f;
    h = ged_loop(uicontrol_array, pt)
endfunction
