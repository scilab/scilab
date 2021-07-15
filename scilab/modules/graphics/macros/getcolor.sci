// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2017, 2021 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [c] = getcolor(Title,cini)

    [lhs,rhs] = argn(0)

    if rhs==0 then
        Title = "";
        cini = 1;
    elseif rhs==1 then
        if type(Title)~=10 then
            msg = gettext("%s: Wrong type for input argument #%d: string expected.\n")
            error(msprintf(msg, "getcolor", 1));
        end
        if size(Title, "*")~=1 then
            msg = gettext("%s: Wrong size for input argument #%d: string expected.\n")
            error(msprintf(msg, "getcolor", 1));
        end
        cini = 1;
    elseif rhs==2 then
        if type(Title)~=10 then
            msg = gettext("%s: Wrong type for input argument #%d: string expected.\n")
            error(msprintf(msg, "getcolor", 1));
        end
        if size(Title, "*")~=1 then
            msg = gettext("%s: Wrong size for input argument #%d: string expected.\n")
            error(msprintf(msg, "getcolor", 1));
        end
        if type(cini)~=1 then
            msg = gettext("%s: Wrong type for input argument #%d: A real expected.\n")
            error(msprintf(msg, "getcolor", 2));
        end
        if size(cini, "*")~=1 then
            msg = gettext("%s: Wrong size for input argument #%d: A real expected.\n")
            error(msprintf(msg, "getcolor", 2));
        end
    else
        msg = gettext("%s: Wrong number of input arguments: %d to %d expected.\n")
        error(msprintf(msg, "getcolor", 2, 0, 2));
    end

    if winsid()~=[] then
        curFig = gcf();
        cmap = curFig.color_map;
        curwin = curFig.figure_id;
    else
        cmap = []
        curwin = []
    end

    // create the window for getcolor
    fig = figure("visible","off","background",-2); //scf();
    win = fig.figure_id;

    sdf;    // quite agressive. Not sure it is actually useful
    sda;    // same remark
    if cmap~=[] then
        fig.color_map = cmap;
        fig.figure_name = gettext("Current colormap")
    else
        cmap = fig.color_map;
        fig.figure_name = gettext("Default colormap")
    end;

    N = size(cmap,1);
    wdim = [1,1];
    m = ceil(sqrt(N));
    n = ceil(N/m);
    [W, H] = (max(m*35,320), n*40)  // minW=320 for infos in status bar
    fig.axes_size = [W,H];
    // position = center of screen:
    c = get(0,"screensize_px")
    fig.figure_position = [c(3)-W, c(4)-fig.figure_size(2)]/2

    toolbar(win, "off")
    delmenu(win, gettext("&File"))
    delmenu(win, gettext("&Tools"))
    delmenu(win, gettext("&Edit"))
    delmenu(win, gettext("&?"))

    dx = wdim(1)/m;
    dy = wdim(2)/n;
    x = -dx;
    y = wdim(2);
    R = [0;0;dx*0.95;dy*0.95];
    rects = [];
    for k = 1:N
        if modulo(k,n)==1 then
            x = x+dx;
            y = wdim(2);
        end;
        rects = [rects,R+[x;y;0;0]];
        y = y-dy;
    end

    if Title~="" then
        xsetech([-1/8,0,1+1/6,1+1/6-1/8],[0,0,wdim(1),wdim(2)]);
    else
        xsetech([-1/8,-1/8,1+1/6,1+1/6],[0,0,wdim(1),wdim(2)]);
    end
    // rectangles with the colors
    xrects(rects,1:N);

    // frame around the colors
    r = m*n-N;
    ddx = 0.05*dx;
    ddy = 0.05*dy;
    if abs(r)<0.1 then
        xpoly([-ddx,1,1,-ddx],[0,0,1+ddy,1+ddy],"lines",1);
    else
        xpoly([-ddx, 1-dx, 1-dx, 1   ,   1  ,-ddx],..
              [  0 ,   0 , r*dy, r*dy, 1+ddy,1+ddy], "lines",1);
    end

    // title
    if Title~="" then
        title(Title, "fontsize",3, "font_style",6)
    end

    k1 = min(max(cini,1),N);
    xrects(rects(eye(),k1),-k1);
    e = gce();
    rector = e.children;
    rector.thickness = 4;
    rector.foreground = color(255*(1-cmap(k1,1)),255*(1-cmap(k1,2)),255*(1-cmap(k1,3)));

    //add a menu and its callback
    done = %f;
    Ok = "execstr("+gettext("Ok")+"_"+string(win)+"(1))";
    Cancel = "execstr("+gettext("Cancel")+"_"+string(win)+"(1))";

    addmenu(win, gettext("Ok"));
    addmenu(win, gettext("Cancel"));
    set(gca(),"tight_limits","on","axes_bounds",[0 0 1 1],"margins",[.05 .05 .15 .05]);
    fig.visible = "on";

    c_i = 0;
    c = cini;
    windowCloseButton = -1000;
    while %t
        str = "";
        [c_i,cx,cy,cw,str] = xclick();
        if (c_i == windowCloseButton) then
            // window has been closed
            k = [];
            c = [];
            break;
        elseif (c_i== -2) then
            if str==Ok then k = k1; c = k; break;end;
            if str==Cancel then k = []; c = []; break;end;
        end

        mc = int(cx/dx)+1;
        nc = n-int(cy/dy);
        k = (mc-1)*n+nc;
        if or(c_i==[0,3])&k<=N&k>0 then
            if k1~=0 then
                move(rector,[rects(1,k)-rects(1,k1),rects(2,k)-rects(2,k1)]);
                rector.foreground = color(255*(1-cmap(k,1)),255*(1-cmap(k,2)),255*(1-cmap(k,3)));
            end;
            k1 = k;
            name = rgb2name(cmap(k,eye())*255);
            txt = msprintf("RGB(%d)=[%d %d %d]",k,cmap(k,1:3)*255);
            if name~=[]
                txt = txt + " : """ + name(1) + """"
            end
            fig.info_message = txt
        end
    end

    if (c_i <> windowCloseButton) then
        delete(fig);
    end

    if curwin~=[] then
        scf(curwin);
    end
endfunction

