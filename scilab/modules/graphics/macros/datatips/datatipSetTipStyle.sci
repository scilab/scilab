// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function datatipSetTipStyle(tip_handle,style)
    //datatip utility function
    point_handle=tip_handle.children(1)
    string_handle=tip_handle.children(2)
    bg=addcolor([255 255 238]/255);
    tip_refs=point_handle.user_data
    [curve_handle,k]=tip_refs(1:2)

    pt=point_handle.data(1,:)

    //label
    if style(3)==0 then //no label
        string_handle.visible="off"
    else
        string_handle.visible="on"
    end
    if style(2)==1 then //boxed
        string_handle.background=bg;
        string_handle.box="on";
        string_handle.fill_mode="on";
        string_handle.font_foreground=curve_handle.foreground;
    else
        string_handle.box="off"
        string_handle.foreground=color("gray");
        string_handle.fill_mode="off";
        string_handle.font_foreground=curve_handle.foreground;
    end
    string_handle.clip_state="off";
    if string_handle.user_data<>[] then //preserve orientation if any
        orient=string_handle.user_data(1)
    else
        orient=0;
    end
    string_handle.user_data=[orient style(2)]

    //marker
    point_handle.mark_style=11;
    point_handle.mark_size_unit="point";
    point_handle.mark_size=6;
    select style(1)
    case 1  then //square marker
        point_handle.data= pt
        point_handle.mark_mode="on";
        point_handle.mark_background=bg;
        point_handle.mark_foreground=curve_handle.foreground;
        point_handle.arrow_size_factor = 0;
    case 2 then //directional arrow marker

        //compute tangeant
        tip_refs=point_handle.user_data
        [curve_handle,k]=tip_refs(1:2)
        if k<size(curve_handle.data,1)
            dxy=(curve_handle.data(k+1,:)-pt)/100000;
        else
            dxy=(pt-curve_handle.data(k-1,:))/100000;
        end
        point_handle.data=[pt;pt+dxy]
        //set properties
        point_handle.mark_background=curve_handle.foreground;
        point_handle.mark_foreground=curve_handle.foreground;
        point_handle.mark_mode="off";
        point_handle.arrow_size_factor = 1.5;
        point_handle.polyline_style = 4;
        point_handle.foreground=curve_handle.foreground;
    end
    if style(3)<>0 then setStringPosition(tip_handle,pt),end
endfunction
