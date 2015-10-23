// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution. The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function ged_eventhandler(win,x,y,ibut)
    //Copyright INRIA
    //Author : Serge Steer 2002
    if ibut==-1 then return,end //ignore move
    if and(win<>winsid())| ibut==-1000 then
        //window has been deleted by the user
        return
    end
    cur=gcf(); //preserve current figure
    fig=scf(win) //make the window associated to the event active

    //disable the event handler not to execute new event before finishing this one
    fig.event_handler_enable = "off";


    global ged_handle;ged_handle=[]
    ged_handle=ged_getobject([x,y])

    if ged_handle~=[] then
        if  or(ibut==[0 3 10]) then //left button --> edit properties
            tkged()
        elseif or(ibut==[2 5 12]) then //right button -->move
            [x,y]=xchange(x,y,"i2f")
            pos=[x,y]
            while %t then
                rep=xgetmouse([%t %t])
                if rep(3)>0 then break,end
                move(ged_handle,rep(1:2)-pos)
                pos=rep(1:2)
            end
        end
    end
    fig.event_handler_enable = "on";
    scf(cur) //reset current window
endfunction
