// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

//chemical process (stiff)

function demo_ode_chstiff()

    // Equations definition
    deff("[yd]=chem(t,y)",[
    "yd(1)=-0.04*y(1) + 1d4*y(2)*y(3);";
    "yd(3)= 3d7*y(2)*y(2);";
    "yd(2)= -yd(1) - yd(3);"])

    // finding points such that y1=1.e-4 or y3=1.e-2

    // Integration
    t     = [1.d-5:0.02:.4 0.41:.1:4 40 400 4000 40000 4d5 4d6 4d7 4d8 4d9 4d10];
    rtol  = 1.d-4;atol=[1.d-6;1.d-10;1.d-6];
    y     = ode([1;0;0],0,t,rtol,atol,chem);

    // Visualisation

    my_handle = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("ode_chstiff.dem.sce");

    drawlater()
    plot2d(t',(diag([1 10000 1])*y)',style=(1:3))
    ax=gca();ax.log_flags = "lnn";ax.box="on";ax.margins(4)=0.2;
    title(_("chemical process (implicit)"))
    l=legend(["y1";"10^4 y2";"y3"]);l.legend_location = "lower_caption"
    drawnow()

    messagebox("Click Ok to continue.","modal");

    // Add surface condition
    nt = prod(size(t));
    deff("[y]=Surf(t,x)","y=[x(1)-1.e-4;x(3)-1.e-2]");

    // First root
    [y,rd,w,iw] = ode("root",[1;0;0],0,t,rtol,atol,chem,2,Surf);rd;

    while rd<>[] then

        [nw,ny]=size(y);
        k=find(rd(1)>t(1:nt-1)&rd(1)<t(2:nt));

        // Visualisation
        //write(%io(2),[rd(1);y(:,ny)]','(''t='',e10.3,'' y='',3(e10.3,'',''))');
        plot2d("ln", rd(1), (diag([1 10000 1])*y(:,ny))',style=[-3,-3,-3]);

        // Next root
        [y,rd,w,iw]=ode("root",[1;0;0],rd(1),t(k+1:nt),rtol,atol,chem,2,Surf,w,iw);
    end

endfunction

demo_ode_chstiff();
clear demo_ode_chstiff;
