// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function datatipRadioCallback(key,v)
    global datatipGUIHandles datatipGUICurve
    Keys=["sty" "lab" "box" "pos" "mul"]
    i=find(key==Keys)
    if v then
        datatipGUIHandles(i,1).value=1
        datatipGUIHandles(i,2).value=0
    else
        datatipGUIHandles(i,1).value=0
        datatipGUIHandles(i,2).value=1
    end
    if i<=3 then
        ud=datatipGetStruct(datatipGUICurve)
        style=ud.style
    end
    select key
    case "sty"
        style(1)=bool2s(~v)+1
    case "lab"
        style(3)=bool2s(v)
    case "box"
        style(2)=bool2s(v)
    case "pos"
        datatipSetInterp(datatipGUICurve,~v);
    case "mul"
        datatipSetReplaceMode(datatipGUICurve,~v);
    end

    if i<=3 then
        ud.style=style
        datatipSetStruct(datatipGUICurve,ud)
        fig=datatipGUICurve.parent
        while fig.type<>"Figure" then fig=fig.parent,end
        id=fig.immediate_drawing;
        fig.immediate_drawing="off"
        for i=1:size(ud.tips.children,"*")
            datatipSetTipStyle(ud.tips.children(i),style)
        end
        fig.immediate_drawing=id
    end
endfunction
