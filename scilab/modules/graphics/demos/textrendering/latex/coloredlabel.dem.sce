// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Calixte DENIZET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_coloredlabel()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("coloredlabel.dem.sce");

    // DEMO START

    x1 = -2:0.1:2;
    x2 = -1:0.1:1;

    plot2d(x1,2/sqrt(%pi)*exp(-x1.^2));
    xpoly([-1 x2 1],[0 2/sqrt(%pi)*exp(-x2.^2) 0],"lines",1);
    p=get("hdl");
    p.polyline_style = 5;
    p.foreground = 20;

    xtitle("$\textstyle\mbox{Graph of }f:t\in[-2;2]\longmapsto\frac2{\sqrt\pi}\mathrm{e}^{-t^2}\in\mathbb{R}$","$t$")

    xstring(-0.55,0.2,"$\fcolorbox{black}{Tan}{\frac2{\sqrt\pi}\int_{-1}^{1}\mathrm{e}^{-t^2}\,\mathrm{d}t\approx"+string(erf(1)-erf(-1))+"}$");

    xstring(1.8,0.04,"$\mathscr{C}_f$");
    // DEMO END

endfunction

demo_coloredlabel();
clear demo_coloredlabel;