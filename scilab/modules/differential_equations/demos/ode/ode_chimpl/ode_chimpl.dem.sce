// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// Equation definition

function demo_ode_chimpl()

    function r=chemres(t,y,yd)
        r(1)=-0.04*y(1)+1d4*y(2)*y(3)-yd(1);
        r(2)=0.04*y(1)-1d4*y(2)*y(3)-3d7*y(2)*y(2)-yd(2)
        r(3)=y(1)+y(2)+y(3)-1;
    endfunction


    function p=chemad(t,y,p),p=p+diag([1 1 0]),endfunction

    function p=chemjac(t,y,yd)
        p=[-0.04,     1.d4*y(3)      ,  1.d4*y(2);
        0.04, -1d4*y(3)-6d7*y(2) , -1d4*y(2);
        1   ,     1              ,  1       ]
    endfunction

    // Integration

    y0   = [1;0;0];
    yd0  = [-0.04;0.04;0];
    t    = [1.d-5:0.02:.4 0.41:.1:4 40 400 4000 40000 4d5 4d6 4d7 4d8 4d9 4d10];
    rtol = 1d-4;atol=[1.d-6;1.d-10;1.d-6];
    y    = impl(y0,yd0,0,t,rtol,atol,chemres,chemad,chemjac);

    // visualization

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("ode_chimpl.dem.sce");

    drawlater()
    plot2d(t',(diag([1 10000 1])*y)',style=(1:3))
    ax=gca();
    ax.log_flags = "lnn";
    ax.box="on";
    ax.margins(4)=0.2;
    title(_("chemical process (implicit)"))
    l=legend(["y1";"10^4 y2";"y3"]);l.legend_location = "lower_caption"
    drawnow()

endfunction

demo_ode_chimpl();
clear demo_ode_chimpl;