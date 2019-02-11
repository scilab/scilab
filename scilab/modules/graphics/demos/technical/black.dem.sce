// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_black()
    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("black.dem.sce");
    my_plot_desc = "black";
    my_handle.figure_name = my_plot_desc;

    // DEMO START
    //Black diagram
    s = poly(0,'s');
    sl = syslin('c',5*(1+s)/(.1*s.^4+s.^3+15*s.^2+3*s+1))

    subplot(1,2,1)
    black(sl, 0.01, 10);

    subplot(1,2,2)
    //Black diagram with Nichols chart as a grid
    Plant = syslin('c',16000/((s+1)*(s+10)*(s+100)));
    //two degree of freedom PID
    tau = 0.2;xsi=1.2;
    PID = syslin('c',(1/(2*xsi*tau*s))*(1+2*xsi*tau*s+tau.^2*s.^2));
    black([Plant;Plant*PID ],0.01,100,["Plant";"Plant and PID corrector"]);

    //move the caption in the lower right corner
    gca().children(1).legend_location = "in_lower_right";
    nicholschart(colors=color('light gray')*[1 1])
    // DEMO END
endfunction

demo_black();
clear demo_black;
