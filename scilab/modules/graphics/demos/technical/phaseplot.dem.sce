// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_phaseplot()
    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("phaseplot.dem.sce");
    my_plot_desc = "phaseplot";
    my_handle.figure_name = my_plot_desc;

    // DEMO START
    s = poly(0,'s')
    h1 = syslin('c',(s^2+2*0.9*10*s+100)/(s^2+2*0.3*10.1*s+102.01))
    h2 = syslin('c',(s^2+2*0.1*15.1*s+228.01)/(s^2+2*0.9*15*s+225))
    phaseplot([h1;h2],0.01,100,..
              ["$\frac{s^2+18 s+100}{s^2+6.06 s+102.1}$";
               "$\frac{s^2+3.02 s+228.01}{s^2+27 s+225}$"])
    title('Phaseplot')
    // DEMO END
endfunction

demo_phaseplot();
clear demo_phaseplot;
