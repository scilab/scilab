// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function legends(leg, style, opt, with_box, font_size )
    //
    // PURPOSE
    //    draw legends on a plot
    // AUTHORS
    //    F. Delebecque + slight modif from B. Pincon
    //  modified again by Eric Dubois and Jean-Baptiste Silvy 18/01/07

    rhs=argn(2)

    if rhs < 2 then
        error(msprintf(gettext("%s: Wrong number of input arguments: At least %d expected.\n"), "legends", 2));
    end
    if type(leg) ~= 10 then,
        error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"), "legends", 1));
    end
    nleg=size(leg,"*")
    if type(style) ~= 1 & type(style) ~= 9 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: Real array or graphic handle array expected.\n"), "legends", 2));
    end
    if ~exists("opt","local") then
        opt=5
    elseif typeof(opt)=="string" then
        select opt
        case "ur" then, opt=1,
        case "ul" then, opt=2,
        case "ll" then, opt=3,
        case "lr" then, opt=4,
        case "?"  then, opt=5,
        case "below" then, opt=6,
        else
            error(msprintf(gettext("%s: Wrong value for input argument #%d: ''%s'', ''%s'', ''%s'', ''%s'', ''%s'' or ''%s'' expected.\n"),"legends", 3, "ur", "ul", "ll", "lr", "?", "below"));
        end
    end
    if ~exists("with_box","local") then, with_box=%t, end
    if ~exists("font_size","local") then
        font_size = 1 ; // default size
    end

    ns=size(style)
    if ns ~= [2 1] & or(ns==1) then, style=matrix(style,1,-1),end
    ns=size(style,2)

    //preserve current graphic context
    f=gcf()
    vis=f.immediate_drawing;
    old_ax=gca(),
    arect=old_ax.margins;
    r1=old_ax.axes_bounds;

    //create small axes on the top left corner (the axes is chosen very
    //small to avoid it can be selected for rotation in new graphic mode
    //case (a little tricky)
    xsetech(wrect=[r1(1),r1(2),r1(3)/1000,r1(4)/1000],frect=[0 0 1,1]/1000,arect=[0,0,0,0])
    xmin=arect(1);xmax=1-arect(2);ymin=-1+arect(4);ymax=-arect(3);
    cur_ax=gca(),
    cur_ax.clip_state="off";

    dy = ymax-ymin ;
    yOffset = dy / 60 ;

    drx=(xmax-xmin)/20 //length of the line
    xOffset = drx/5 ;

    bbx=[]
    for k=1:nleg
        r = stringbox( leg(k), 0, 0, 0, old_ax.font_style, font_size ) ;
        bbx=[bbx;r(1,3) - r(1,1) , r(2,3) - r(2,1)]; //[width height]
    end

    height = sum(bbx(:,2))
    width  = max(bbx(:,1)) + drx * 1.6 ; // 1.6 * drx = 1.2 * drx + 2 * drx / 5 ;

    //upper left coordinates
    if size(opt,"*")>1 then
        // fix for bug 1237 (Bruno 9 march 2005)
        if old_ax.tight_limits == "on" then  // data_bounds' corresponds to the old frec
            r2 = old_ax.data_bounds'
        else
            r2 = [old_ax.x_ticks.locations(1),old_ax.y_ticks.locations(1),...
            old_ax.x_ticks.locations($),old_ax.y_ticks.locations($)]
        end
        pos(1) = xmin + ((opt(1)-r2(1))/(r2(3)-r2(1)))*(1-arect(1)-arect(2))
        pos(2) = ymin + ((opt(2)-r2(2))/(r2(4)-r2(2)))*(1-arect(3)-arect(4))
        // end bugfix
        opt=0 ;
    elseif opt<1 | opt>6 then
        error(msprintf(gettext("%s: Wrong value for input argument %s: Must be in the interval [%d, %d]."),"legends", "opt", 1, 6)); // if opt is entered by a string by the user it is in the range
    end
    select opt
    case 1 then
        pos=[xmax-width-xOffset,ymax-yOffset]
    case 2 then
        pos=[xmin+xOffset,ymax-yOffset]
    case 3 then
        pos=[xmin+xOffset,ymin+height+yOffset]
    case 4 then
        pos=[xmax-width-xOffset,ymin+height+yOffset]
    case 5 then
        rect=dragrect([xmax-width-xOffset,ymax-yOffset,width,height])
        pos=rect(1:2)
    case 6 then
        // decrease the size of the graph to leave place for the legend
        old_ax.margins=[xmin;arect(2);arect(4);arect(3)+height + 2 * yOffset ]
        pos=[(xmin+xmax)/2-width/2, ymin + height - 2 * yOffset ]
    end

    x=pos(1)+xOffset
    y=pos(2)

    f.immediate_drawing = "off",
    a=gca() ;
    a.foreground=old_ax.foreground ;
    a.background=old_ax.background ;
    a.font_color=old_ax.font_color ;
    a.font_size =old_ax.font_size  ;
    a.font_style=old_ax.font_style ;

    R=[]
    if with_box then
        xpol = [pos(1), pos(1)+width, pos(1)+width, pos(1)];
        ypol = [pos(2), pos(2), pos(2)-height, pos(2)-height];
        xfpoly(xpol, ypol,1)
        R = gce();
        R.foreground=a.foreground;
        R.background=a.background;
    end
    for k=1:nleg
        if k<=size(style,2) then
            if type(style)==9 then
                h=style(k)
                select h.type
                case "Polyline"
                    if h.polyline_style==5 then //patch
                        xfpoly([x;x+drx;x+drx;x;x],[y-bbx(k,2);y-bbx(k,2);y;y;y-bbx(k,2)]);
                        r=gce();
                        r = unglue(r); // one xfpoly returns 2 polylines -> tmp bug to fix later F.Leray
                        r.foreground=h.foreground;
                        r.thickness=h.thickness;
                        r.polyline_style=h.polyline_style;
                        r.line_style=h.line_style;
                    else
                        if stripblanks(h.mark_mode)=="off"
                            xpoly([x;x+drx],[y;y]-bbx(k,2)/2,"lines");r=gce();
                            r.foreground=h.foreground;
                            r.thickness=h.thickness;
                            r.polyline_style=h.polyline_style;
                            r.line_style=h.line_style;
                        else
                            xpoly(x+drx/2,y-bbx(k,2)/2);r=gce();
                            r.foreground=h.foreground;
                            r.thickness=h.thickness;
                            r.mark_style=h.mark_style;
                            r.mark_size=h.mark_size;
                        end
                    end
                else
                    error(msprintf(gettext("%s: Wrong type for input argument #%d: Polyline handles expected.\n"),"legends",2));
                end
            else
                if style(1,k)<= 0 then
                    xpoly(x+drx/2,y-bbx(k,2)/2)
                    r=gce(),
                    r.mark_mode="on"
                    r.mark_style=-style(1,k)
                    if size(style,1)==2 then r.mark_foreground=style(2,k);end
                else
                    xpoly([x;x+drx],[y;y]-bbx(k,2)/2,"lines")
                    r=gce(),
                    r.foreground=style(1,k)
                    if size(style,1)==2 then r.line_style=style(2,k);end
                end
            end
        end
        R=[R,r']
        xstring(x + drx*1.2 + bbx(k,1)/2,y-bbx(k,2)/2,leg(k))

        r=gce()
        r.font_size = font_size ;
        r.alignment = "center" ;
        r.text_box_mode = "centered" ; // the string is centered on (x,y)
        R=[R,r]
        y=y-bbx(k,2) ;
    end
    glue(R)
    a=gca();
    a.data_bounds = [0,0;0.001,0.001];

    set("current_axes",old_ax),

    // if immediate_drawing was "on", then the figure will redraw itself.
    f.immediate_drawing = vis;

endfunction
