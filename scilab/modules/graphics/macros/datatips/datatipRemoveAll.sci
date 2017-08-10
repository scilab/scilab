// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
// Copyright (C) 2015 - Samuel GOUGEON : http://bugzilla.scilab.org/13881
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function datatipRemoveAll(curve_handles)
    //remove all the datatips for the given curves
    if argn(2)<>1 then //search for curves in the current axes
        ax = gca()
        curve_handles = datatipGetEntities(ax)
    elseif type(curve_handles)<>9 then
        error(msprintf(_("%s: Wrong type for input argument #%d: an array of graphic handle expected.\n"),"datatipRemoveAll",1))
    elseif size(curve_handles,"*")==1 & curve_handles.type=="Axes" then
        //the argument is a handle on an axes, search for curves in it
        ax = curve_handles
        curve_handles = datatipGetEntities(ax)
    elseif and(curve_handles.type<>"Figure") then
        //check for a common axes entity for parent
        ax = []
        for k=1:size(curve_handles,"*")
            a = curve_handles(k)
            while a.type<>"Axes" then a = a.parent,end
            if ax<>[] & a<>ax then
                error(msprintf(_("%s: Wrong value for input argument #%d: the handles must have the same parent.\n"),"datatipRemoveAll",1))
            end
            ax = a
        end
    else
        error(msprintf(_("%s: Wrong type for input argument #%d: handle on axes or axes children expected.\n"),"datatipRemoveAll",1))
    end
    fig = ax.parent
    id = fig.immediate_drawing;
    fig.immediate_drawing = "off"
    for k = 1:size(curve_handles,"*")
        tips = curve_handles(k).datatips
        for i = 1:size(tips,"*")
            datatipRemove(tips(i))
        end
    end
    fig.immediate_drawing = id
endfunction
