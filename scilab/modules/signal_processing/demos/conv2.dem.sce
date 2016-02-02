// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge STEER
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function demo_conv2()
    //blur filter example
    function H=gaussian2d(radius,sigma)
        x=linspace(-1,1,10)*radius
        H=exp(-(ones(x')*x.^2+(x').^2*ones(x))/(2*sigma.^2))/(2*%pi*sigma.^2);
    endfunction

    load(get_absolute_file_path("conv2.dem.sce")+"image.dat")

    f=scf(1000);clf;
    f.axes_size=[800 400];
    demo_viewCode("conv2.dem.sce");

    f.color_map=graycolormap(128);
    drawlater()
    subplot(121)
    Matplot(I*128);
    a=gca();a.margins=[0 0 0.125 0];
    a.tight_limits="on";
    a.axes_visible="off";
    title("Original image")

    subplot(122)
    I1=conv2(I,gaussian2d(3,0.84),"same");
    I1=I1*128/max(I1);
    Matplot(I1);
    a=gca();a.margins=[0 0 0.125 0]
    a.tight_limits="on";
    a.axes_visible="off";
    title("Blurred  image")
    drawnow()
endfunction
demo_conv2()
clear demo_conv2
