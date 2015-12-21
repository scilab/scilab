// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
