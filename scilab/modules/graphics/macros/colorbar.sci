// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Bruno Pincon
// Copyright (C) Serge Steer (adaptation to new graphic system)
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function colorbar(umin, umax, colminmax, fmt)

    //  PURPOSE
    //     Draw a colorbar for a plot3d, fec, Sgrayplot, etc...
    //
    //  PARAMETERS
    //     umin : min value of the plot
    //     umax : max value of the plot
    //     colminmax : (optional) a vector with 2 integer components
    //                 the first is the color number (of the current
    //                 colormap) associated with umin
    //                 the second the max color number ....
    //                 default : [1 nb_colors] where nb_colors is
    //                 the number of colors of the current colormap.
    //                 May be useful to deal with a part of the colormap
    //                 (for instance using fec or plot3d)
    //     fmt : optional, a C format to display colorbar graduations
    //
    //  CAUTION
    //     this function may be used BEFORE a plot3d, fec, Sgrayplot, ...
    //     It is important because this function set and change the
    //     frame for the plot. This way the colorbar is not part of
    //     the "associated" plot and so is not modified by a zoom or
    //     a rotation of the plot.
    //
    //
    //  EXAMPLES
    //     see the help page

    // Check number of input argument
    [lhs, rhs] = argn();
    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"), "colorbar", 2, 4));
    end
    nb_grad = 5
    if ~exists("fmt","local") then
        fmt="%-5.2g";
    else
        if type(fmt)<>10|size(fmt,"*")<>1 then
            error(msprintf(gettext("%s: Wrong type for input argument #%d: %s data structure expected.\n"), "colorbar", 4, "string (containing a C format)"));
        end
    end

    f=gcf();
    if ~exists("colminmax","local") then
        nb_colors = size(f.color_map,1)
        colminmax = [1 nb_colors]
    else
        nb_colors = colminmax(2) - colminmax(1) + 1
    end

    //defer the drawing to avoid binking
    id=f.immediate_drawing=="on";
    if id then f.immediate_drawing=="off",end
    // get current axes and properties
    a=gca();
    fg_color=a.foreground
    wr=a.axes_bounds; //get the rectangle of the current axes

    // modify the orginal axes to let space for the colorbar
    a_pl=a;
    a_pl.axes_bounds=[wr(1) , wr(2) , 0.85*wr(3) , wr(4)]

    // create a new axes for the colorbar et set its properties
    a_cb=newaxes();
    a_cb.axes_bounds=[wr(1)+0.83*wr(3) , wr(2)+wr(4)*0.2 , 0.2*wr(3) , wr(4)*0.6];
    a_cb.data_bounds=[0 umin;1 umax];
    a_cb.foreground=a.foreground;
    a_cb.background=f.background;
    a_cb.axes_visible="on";
    a_cb.y_location = "right";
    a_cb.tight_limits ="on";

    //It is not possible to set no ticks for x (should be fixed)
    a_cb.x_ticks=tlist(["ticks","locations","labels"],-1,"");
    ytics = linspace(umin, umax, nb_grad);
    a_cb.y_ticks=tlist(["ticks","locations","labels"],ytics', " "+msprintf(fmt+"\n",ytics'));
    a_cb.auto_ticks = ["off","off","on"];
    a_cb.box = "on";
    a_cb.margins=[0 0.75 0 0];

    //draw the colorbar
    y = linspace(umin,umax,nb_colors)
    col=[colminmax(1):colminmax(2)]
    Sgrayplot([0 1],y,[col;col],colminmax=colminmax)

    //reset the initial values
    sca(a_pl) //current axes
    if id then f.immediate_drawing="on",end //drawing mode

endfunction






