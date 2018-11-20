// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2010, 2018 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_polarplot()
    my_handle             = scf(100001);

    // DEMO START

    my_plot_desc          = "polarplot";
    my_handle.figure_name = my_plot_desc;
    tmp = gettext("Please press enter into the Scilab console to see more polarplot examples")
    messagebox(tmp,"modal");

    nextMessage = gettext("Press enter to go on...");
    t=linspace(0,1,100)*%pi*2;
    drawlater
    clf
    demo_viewCode("polarplot.dem.sce");

    // only 1 quadrant:
    subplot(2,2,1)
    polarplot(t/4+%pi/2,sin(t/10))
    subplot(2,2,2)
    polarplot(t/4,sin(t/10))
    subplot(2,2,3)
    polarplot(t/4+%pi,sin(t/10))
    subplot(2,2,4)
    polarplot(t/4-%pi/2,sin(t/10))
    show_window(my_handle)
    drawnow
    input(nextMessage,"s");

    // 2 quadrants:
    drawlater
    clf
    demo_viewCode("polarplot.dem.sce");
    xsetech([0 0 0.6 0.5])
    polarplot(t/2,sin(t/10))
    xsetech([0 0.5 0.6 0.5])
    polarplot(t/2+%pi,sin(t/10))
    subplot(1,3,3)
    polarplot(t/2-%pi/2,sin(t/10))
    show_window(my_handle)
    drawnow
    input(nextMessage,"s");

    // else : full circle:
    drawlater
    clf
    demo_viewCode("polarplot.dem.sce");
    subplot(1,2,1)
    polarplot(t,sin(t/10))
    title(gettext("Data on 3 or 4 quadrants"))

    subplot(1,2,2)
    polarplot([sin(2*t') sin(4*t')],[cos(4*t') cos(2*t')],[1,2])
    title(gettext("Several curves may be plotted at the same time"))
    show_window(my_handle)
    drawnow
    input(nextMessage,"s");

    // Various radii scales:
    t=linspace(0,1,200)*%pi*2;
    drawlater
    clf
    demo_viewCode("polarplot.dem.sce");
    subplot(2,3,1)
    polarplot(t/10-%pi,4600*sin(t/10))    // Q3
    title(gettext("Big radii are supported..."))

    subplot(2,3,4)
    polarplot(t/4+%pi/2,1.4e-5*sin(t/10))  // Q2
    title(gettext("... as well as very tiny ones"))

    xsetech([0.33 0 0.7 0.5])
    polarplot(t/2,3e5*sin(t/10))          // Q1+2
    title(gettext("even huge radii, with properly formatted labels..."))

    // with rect=
    xsetech([0.33 0.5 0.7 0.48])
    polarplot(t/4-1,sin(t/10)-0.3, rect=[-0.3 -0.02 0.15 0.3])
    xlabel(gettext("A partial viewport may be set with clipping"))

    show_window(my_handle)
    drawnow

    // DEMO END
endfunction

demo_polarplot();
clear demo_polarplot;
