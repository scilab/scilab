// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [h,Axes]=ged_getobject(pt)
    h=[];Axes=[];
    f=get("current_figure");
    aold=get("current_axes")
    axes_array=f.children
    //retains only the entities of type Axes (remove uimenus)
    axes_array(axes_array.type<>"Axes")=[];
    // assume that the lastly created objects
    // are at the beginning of the arrays of children
    // We can then select the last object
    // first in the loop.
    for k=1:size(axes_array,"*")
        Axes=axes_array(k)
        set("current_axes",Axes)
        h=ged_loop(Axes,pt)
        if h<>[] then break,end
    end
    set("current_axes",aold)
endfunction
