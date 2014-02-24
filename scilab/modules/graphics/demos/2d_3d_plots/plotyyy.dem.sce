// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 Samuel GOUGEON
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_plotyyy()

    // DEMO START
    // A plotyyy() example: //http://bugzilla.scilab.org/show_bug.cgi?id=6070

    my_handle = scf(100001);
    clf(my_handle);

    // Preparing data
    x  = linspace(1,30,200);
    y2 = exp(x/6).*(sin(x)+1.5);
    y3 = 1+x.^2;

    drawlater()
    demo_viewCode("plotyyy.dem.sce");

    // Axis y1
    y1=sin(x/2);
    plot2d(x,y1)
    xtitle([gettext("Plot with 3 different Y scales at shared X");" "],..
    gettext("Common X axis"),gettext("Scale #1"));

    // Axis y2
    c=color("blue");
    na=newaxes();
    na.foreground=c;
    na.font_color=c;
    plot2d(x,y3,logflag="nl",style=c)
    na.children(1).children(1).thickness=2;
    na.filled="off";
    na.axes_visible(1)="off";
    na.axes_reverse(2)="on";
    na.y_location="middle";

    // Axis y3
    c=color("red");
    na=newaxes();
    na.foreground=c;                        // Axis and ticks color
    na.font_color=c;                        // Labels's color
    plot2d(x,y2,style=c);
    ylabel("Scale n°3","color",[1 0 0])
    na.filled="off";                        // Transparent background, letting the first plot appearing
    na.axes_visible(1)="off";               // Masking the x axis (useless overlay)
    na.y_location="right";                  // Y axis on the right side
    na.children(1).children(1).thickness=2; // Curve thickness

    drawnow()

    // DEMO END

endfunction

demo_plotyyy();
clear demo_plotyyy;
