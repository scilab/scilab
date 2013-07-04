// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Samuel GOUGEON
// Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_polarplot()
    my_handle             = scf(100001);
    clf(my_handle);

    // DEMO START

    my_plot_desc          = "polarplot";
    my_handle.figure_name = my_plot_desc;
    tmp = gettext("Please press enter into the Scilab console to see more polarplot examples")
    messagebox(tmp,"modal");
    nextMessage = gettext("Press enter to go on...");
    t=linspace(0,1,100)*%pi*2;

    // only 1 quadrant:
    clf, polarplot(t/4,sin(t/10))
    title(gettext("polarplot(): Only quadrant(s) reached by data are displayed"))
    show_window(my_handle)
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf, polarplot(t/4+%pi/2,sin(t/10))
    title(gettext("Still a single quadrant...")), show_window(my_handle)
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf, polarplot(t/4+%pi,sin(t/10)), show_window(my_handle)
    input(nextMessage,"s")

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf, polarplot(t/4-%pi/2,sin(t/10)), show_window(my_handle)
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    // 2 quadrants:
    clf, polarplot(t/2,sin(t/10)), show_window(my_handle)
    title(gettext("on 2 quadrants..."))
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf, polarplot(t/2+%pi/2,sin(t/10)), show_window(my_handle)
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf, polarplot(t/2+%pi,sin(t/10)), show_window(my_handle)
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf, polarplot(t/2-%pi/2,sin(t/10)), show_window(my_handle)
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    // else : full circle:
    clf, polarplot(t,sin(t/10))
    title(gettext("Data on the full circle ..."))
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    // with rect=
    clf, polarplot(t/4-1,sin(t/10)-0.3,rect=[-0.18 -0.02 0.27 0.3])
    title(gettext("A partial viewport may be set with clipping"))
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    // with negative radii:
    r = sin(t/10)-0.5;
    clf, polarplot(t,sin(t/10)-0.5), show_window(my_handle)
    title(msprintf(gettext("Negative radii are supported:")+" r_min = %.2f",min(r)))
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    t=linspace(0,1,200)*%pi*2;
    clf, polarplot(t/10-%pi,4600*sin(t/10)), show_window(my_handle)
    title(gettext("Big radii are supported..."))
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf, polarplot(t/2,4600*sin(t/10)), show_window(my_handle)
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf,   xstring(0,0,"$\magnification{1600}\ $"); // for LaTeXed labels
    polarplot(t/2,3e5*sin(t/10)), show_window(my_handle)
    title(gettext("even huge radii, with properly formatted labels..."))

    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf, polarplot(t/4,3e5*sin(t/10)), show_window(my_handle)
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf, polarplot(t/4,1.4e-5*sin(t/10)), show_window(my_handle)
    title(gettext("... as well as very tiny radii"))
    input(nextMessage,"s");

    if is_handle_valid(my_handle) == %f then
        return
    end

    clf, t= 0:.01:2*%pi;
    polarplot([sin(2*t') sin(4*t')],[cos(4*t') cos(2*t')],[1,2])
    title(gettext("Several curves may be plotted at the same time"))
    show_window(my_handle)

    if is_handle_valid(my_handle) == %f then
        return
    end

    demo_viewCode("polarplot.dem.sce");


    // DEMO END
endfunction

demo_polarplot();
clear demo_polarplot;
