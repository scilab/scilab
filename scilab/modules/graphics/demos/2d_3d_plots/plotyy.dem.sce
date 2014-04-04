// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 Samuel GOUGEON
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// See http://bugzilla.scilab.org/show_bug.cgi?id=6070

function demo_plotyy()

    // DEMO START

    fs=3; // Common font size
    scf(100001);
    clf()
    drawlater()
    a=gda();
    dfs=a.font_size; // For further restoring
    a.font_size=fs;
    demo_viewCode("plotyy.dem.sce");

    // --------------------------------------------------------------------------

    // Preparing data
    x=linspace(45,500,200);
    // --------------------------------------------------------------------------
    // Axe y1:
    y1=sind(x);
    plot2d(x,y1);
    a=gca();
    a.tight_limits="on";
    a.font_size=fs;
    ylabel([" ";"$\sin(\alpha)$"])
    m1 = a.margins;
    pa = a;
    // --------------------------------------------------------------------------

    // Overall title and common X axe
    xtitle(gettext("Graphics with 2 different Y-scales"),"$\mbox{Common angle }\alpha\ [\deg]$");
    // --------------------------------------------------------------------------

    // Axe y2:
    y2=(1.1+cosd(x))*800;
    a=newaxes();    // Creates a new default system of axes on the same
    // area of the window, and makes it the current one
    a.filled="off"; // the background of the graphical area should be
    // transparent, to leave the previous plot apparent
    red=color("red");
    a.foreground=red;                               // Axis and ticks color
    a.font_color=red;                               // sets color for ticks labels
    plot2d(x,y2,logflag="nl",axesflag=3,style=red); // plots new data. axesflag=3
    a.axes_visible(1)="off";                        // cancels x axis (else red)
    a.tight_limits="on";

    a.y_label.font_foreground=red;  // sets color for y-label
    ylabel("$(1.1+\cos(\alpha))*800$")
    //means X at the bottom, and Y at the right

    m2 = a.margins;
    a.margins = [m1(1) m2(2) m1(3) m1(4)];
    pa.margins = a.margins;

    xgrid(color("grey"))
    drawnow()
    // --------------------------------------------------------------------------
    a=gda();
    a.font_size=dfs; // restoring
    clear x y1 y2 a red fs dfs

    // DEMO END

endfunction

demo_plotyy();
clear demo_plotyy;