// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function datatip2

    my_handle = scf(100001);
    clf(my_handle,"reset");
    fig=gcf();

    title(_("datatips on 3D curves"),"fontsize",3);


    r=1;
    k=tan(%pi/27);
    t=linspace(-40,40,1000);
    x=r*cos(t)./cosh(k*t);
    y=r*sin(t)./cosh(k*t);
    z=r*tanh(k*t);
    param3d(x,y,z);c=gce();c.thickness=2;C.foreground=color("blue")

    ax=gca();ax.rotation_angles=[70 50];ax.axes_visible="off";
    drawnow()

    c.display_function_data = t;
    datatipSetStyle(c,2,%f) //directional arrow no box

    function s=mydisp(curve,pt,index),
        ud=datatipGetStruct(curve);
        if index<>[] then
            t=ud.t(index);
        else //interpolated
            [d,ptp,i,c]=orthProj(curve.data,pt);
            t=ud.t(i)+(ud.t(i+1)-ud.t(i))*c;
        end
        s=msprintf("%.2g", t);
    endfunction

    for k=[350 400 450 500 520  550 600]
        dt=datatipCreate(c,k);
    end
    //datatipSetDisplay(c,mydisp) // Change the default datatip label

    datatipManagerMode(fig,"on") //activate the interactive editor

    demo_viewCode("datatip2.sce");

endfunction

datatip2();
clear datatip2;
