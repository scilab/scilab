// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function datatipManagerMode(varargin)
    //Toggle or sets the interactive mode for datatips edition.;
    //  This function is called by the datatips menu callbacks.;
    global datatipAngles; //to be able to detect that a rotation occurred
    old=[] //to preserve current figure
    if winsid()<>[] then  old=gcf();end
    select size(varargin)
    case 0 then //toggle mode
        fig=gcf()
        action="toggle"
    case 1 then
        if type(varargin(1))==10 then
            action=varargin(1)
            fig=gcf()
        elseif type(varargin(1))==9 then
            fig=varargin(1)
            action="toggle"
        elseif type(varargin(1))==1 then
            fig=scf(varargin(1))
            if old<>[] then scf(old),end
            action="toggle"
        end
    case 2 then
        if type(varargin(1))==10 then
            action=varargin(1)
            fig=varargin(2)
        elseif type(varargin(1))==9 then
            fig=varargin(1)
            action=varargin(2)
        elseif type(varargin(1))==1 then
            fig=scf(varargin(1))
            if old<>[] then scf(old),end
            action=varargin(2)
        end
    else
        error(msprintf(_("%s: too many input arguments"),"datatipManagerMode"))
    end
    fig_ud=get(fig,"user_data")
    if action =="toggle" then
        if fig.event_handler <> "datatipEventhandler" then
            action="on"
        elseif fig.event_handler_enable == "on"
            action="off"
        else
            action="on"
        end
    end
    select action
    case "on"
        if fig.event_handler<>""& fig.event_handler<>"datatipEventhandler" then
            //push current event handler in fig user data if possible
            if fig_ud==[] then fig_ud=struct();end
            if typeof(fig_ud)=="st" then
                if ~isfield(fig_ud,"handlers") then  fig_ud.handlers=[],end
                fig_ud.handlers=[fig_ud.handlers;
                fig.event_handler fig.event_handler_enable]
                set(fig,"user_data",fig_ud)
            else
                warning(_("Datatip manager cannot be enabled, user data figure field is already used" ))
                return
            end
        end
        fig.event_handler_enable = "off" //to prevent against bug 7855
        fig.event_handler = "datatipEventhandler"
        fig.event_handler_enable = "on"
        xinfo(_("Left click on a curve to create a datatip, right opens contextual menu"))
        show_window(fig)
    case "off"
        fig.event_handler_enable = "off"
        if typeof(fig_ud)=="st"&isfield(fig_ud,"handlers")&fig_ud.handlers<>[] then
            fig.event_handler=fig_ud.handlers($,1)
            fig.event_handler_enable=fig_ud.handlers($,2)
            fig_ud.handlers= fig_ud.handlers(1:$-1,:)
            set(fig,"user_data",fig_ud)
        else
            fig.event_handler_enable = "off"
        end
        clearglobal datatipAngles
        xinfo("")
    end
endfunction
