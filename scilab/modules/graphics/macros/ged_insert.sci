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

function ged_insert(k,win)
    //xset, xget used because ged should handle both old and new style

    [lhs,rhs]=argn(0);

    if rhs<2,
        error(msprintf(gettext("%s: Wrong number of input argument(s): At least %d expected.\n"), "ged_insert", 2));
    end

    global active men

    //check if user changes is mind
    active=active+1;men=k
    if  active>1 then return,end

    ged_current_figure=xget("window")
    xset("window",win)

    scf(win);
    ged_cur_fig_handle=gcf();

    mess1 = "Press the right mouse button (during a while) to stop the line creation";
    mess2 = "Press any mouse button to complete the object drawing"
    entities=["Rectangle","Segment","Polyline","Arrow","Double Arrow", ...
    "Text","Circle"]

    default_axes = gca(); // get the default axes where we start
    f=gcf();
    job="annotation"
    if job=="annotation" then axes = get_insertion_axes(job,f),end

    [k,xc,yc]=ged_click()

    drawlater();
    default_axes = gca(); // get the default axes where we start
    rep(3)=-1
    select k
    case -100 then //window has been closed
        ged_insert_end(),return
    case 1 then //Single Line
        axes = get_insertion_axes(job,f,default_axes,xc,yc)
        [xc,yc] = xchange(xc,yc,"f2i"); // I pass to pixel
        sca(axes);  // I change axes and therefore change the scale
        [xc,yc] = xchange(xc,yc,"i2f"); // in the new scale I recompute the corresponding pixel values
        xpoly([xc;xc],[yc;yc])
        drawnow();
        r=gce();r.foreground=-1;
        r.clip_state="off";
        xinfo(mess2)
        while rep(3)==-1 do
            rep=xgetmouse(0,[%t %t])
            r.data=[xc,yc;rep(1),rep(2)]
            xinfo(mess2)
        end
    case 2 then //Polyline (stroken line)
        axes = get_insertion_axes(job,f,default_axes,xc,yc)
        [xc,yc] = xchange(xc,yc,"f2i");
        sca(axes);
        [xc,yc] = xchange(xc,yc,"i2f");
        xpoly([xc;xc],[yc;yc])
        drawnow();
        r=gce();r.foreground=-1;
        r.data(:,3)=0.;
        r.clip_state="off";
        xinfo(mess1)
        while %t
            rep(3)=-1;
            while rep(3)==-1 do
                rep=xgetmouse(0)
                r.data($,:)= [rep(1),rep(2),0]
                xinfo(mess1)
            end
            if or(rep(3)==[2 5 -100]) then break,end
            r.data=[r.data;r.data($,:)]
        end
    case 3 // Arrow (single arrow)
        axes = get_insertion_axes(job,f,default_axes,xc,yc)
        [xc,yc] = xchange(xc,yc,"f2i");
        sca(axes);
        [xc,yc] = xchange(xc,yc,"i2f");
        xpoly([xc;xc],[yc;yc])
        drawnow();
        r=gce();r.foreground=-1;
        r.data(:,3)=0.;
        r.polyline_style = 4;
        r.arrow_size_factor=10; // change the factor to have a nice arrow
        r.clip_state="off";
        xinfo(mess2)
        yc = [];
        while rep(3)==-1 do
            rep=xgetmouse(0,[%t %t])
            r.data($,:)= [rep(1),rep(2),0]
            xinfo(mess2)
        end
        //   case 4 // Double Arrow
        //      axes = get_insertion_axes(job,f,default_axes,xc,yc)
        //     [xc,yc] = xchange(xc,yc,'f2i');
        //     sca(axes);
        //     [xc,yc] = xchange(xc,yc,'i2f');
        //     xpoly([xc;xc],[yc;yc])
        //     r1=gce();r.foreground=-1;
        //     r1.data(:,3)=0.;
        //     r1.polyline_style = 4;
        //     r1.arrow_size_factor=5; // change the factor to have a nice arrow
        //     r1.clip_state='off';
        //
        //     // second polyline (for the seconf arrow from end to origin)
        //     xpoly([xc;xc],[yc;yc])
        //     //xpoly([r.data(2,1);r.data(1,1)],[r.data(2,2);r.data(1,2)])
        //     r2=gce();r.foreground=-1;
        //     r2.data(:,3)=0.;
        //     r2.polyline_style = 4;
        //     r2.arrow_size_factor=5; // change the factor to have a nice arrow
        //     r2.clip_state='off';
        //
        //     xinfo(mess2)
        //     while rep(3)==-1 do
        //       rep=xgetmouse(0,[%t %t])
        //       drawlater();
        //       r1.data($,:)= [rep(1),rep(2),0]
        //       r2.data(1,:)= [rep(1),rep(2),0]
        //       drawnow();
        //       xinfo(mess2)
        //     end

    case 4 then //Text
        // open a dialog to enter the text
        text = x_dialog("Enter the new text here.","") ;
        if ( text <> [] & text <> "" ) then
            // do nothing if cancel button has been pressed or no text entered
            // get the position of the text
            xstring( xc, yc, text ) ;
        end
        drawnow();
    case 5 then //Rectangle
        axes = get_insertion_axes(job,f,default_axes,xc,yc)
        [xc,yc] = xchange(xc,yc,"f2i");
        sca(axes);
        [xc,yc] = xchange(xc,yc,"i2f");
        xrect(xc,yc,0,0)
        drawnow();
        r=gce();r.foreground=-1;
        r.clip_state="off";
        xinfo(mess2)
        while rep(3)==-1 do
            rep=xgetmouse(0,[%t %t])
            r.data=[min(xc,rep(1)),max(yc,rep(2)),abs(xc-rep(1)),abs(yc-rep(2))]
            xinfo(mess2)
        end
    case 6 then //Circle
        axes = get_insertion_axes(job,f,default_axes,xc,yc)
        [xc,yc] = xchange(xc,yc,"f2i");
        sca(axes);
        [xc,yc] = xchange(xc,yc,"i2f");
        xarc(xc,yc,0,0,0,64*360)
        drawnow();
        r=gce();r.foreground=-1;
        r.clip_state="off";
        xinfo(mess2)
        while rep(3)==-1 do
            rep=xgetmouse(0,[%t %t])
            r.data=[min(xc,rep(1)),max(yc,rep(2)),abs(xc-rep(1)),abs(yc-rep(2)),0,64*360]
            xinfo(mess2)
        end
    end
    ged_insert_end()
endfunction



function axes = get_insertion_axes(job,f,default_axes,xc,yc)
    if job=="annotation" then //special axes used
        axes=f.children(1)
        if axes.user_data=="annotation" then return,end
        //  create a new specialized axes at left bottom of the window
        //it should be better if the annotation axes be directly attached to
        //the figure (to be done)
        axes=newaxes();
        axes.user_data="annotation"
        axes.axes_bounds=[0,1,0.15,0.15];
        axes.margins=[0 0 0 0];
        axes.axes_visible = ["off","off","off"]
        axes.background=f.background;
    else //get the axes containing the point xc,yc
        //the point  xc,yc is given in user coordinate relative to the
        //default_axes axes entity.
        nb_axes = size(f.children,"*")
        // for now I consider that all children of a figure are of type Axes
        axes_size = f.axes_size // given in pixels
        axes_size = [axes_size axes_size];
        [x,y]=xchange(xc,yc,"f2i")
        found=%f
        for i=1:nb_axes
            axes = f.children(i);
            cur_axes_bounds = axes.axes_bounds;
            rect = cur_axes_bounds.*axes_size; // rectangle in pixels (margins inside)
            rect(3) = rect(3) + rect(1);
            rect(4) = rect(4) + rect(2);
            if (x>rect(1) & x<rect(3) & y>rect(2) & y<rect(4)) then  found=%t,break,end
        end
        if ~found then axes=[],end
    end
endfunction

function  ged_insert_end()
    if or(win==winsid()) then //  the window still exists
        sca(default_axes); // resume the default axes
        f=gcf()
        clearglobal active men
    end
    if win<>ged_current_figure then xset("window",ged_current_figure),end

endfunction
function [men,xc,yc]=ged_click()
    global  men
    [btn,xc,yc]=xclick()
    if btn==-100 then men=-100,end
    for k=1:3,end
    job=men
endfunction

