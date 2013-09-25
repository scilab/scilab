// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function datatipRemoveAll(curve_handles)
    //remove all the datatips for the given curves
    if argn(2)<>1 then //search for curves in the current axes
        ax=gca()
        curve_handles=datatipGetEntities(ax)
    elseif type(curve_handles)<>9 then
        error(msprintf(_("%s: Wrong type for input argument #%d: an array of graphic handle expected.\n"),"datatipRemoveAll",1))
    elseif size(curve_handles,"*")==1&curve_handles.type=="Axes" then
        //the argument is a handle on an axes, search for curves in it
        ax=curve_handles
        curve_handles=datatipGetEntities(ax)
    elseif and(curve_handles<>"Figure") then
        //check for a common axes entity for parent
        ax=[]
        for k=1:size(curve_handles,"*")
            a=curve_handles(k)
            while a.type<>"Axes" then a=a.parent,end
            if ax<>[]&a<>ax then
                error(msprintf(_("%s: Wrong value for input argument #%d: the handles must have the same parent.\n"),"datatipRemoveAll",1))
            end
            ax=a
        end
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: handle on axes or axes children expected.\n"),"datatipRemoveAll",1))
    end

    fig=ax.parent
    id=fig.immediate_drawing;
    fig.immediate_drawing="off"
    for k=1:size(curve_handles,"*")
        ck=curve_handles(k);

        num = 1;
        for t=1:size(ck.children,"*")
            tip = ck.children(num);
            if tip.type == "Datatip" then
                datatipRemove(tip);
            else
                num = num + 1;
            end
        end
    end
    fig.immediate_drawing=id
endfunction
