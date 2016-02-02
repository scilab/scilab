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
