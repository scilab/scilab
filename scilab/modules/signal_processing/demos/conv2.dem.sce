// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - INRIA - Serge STEER
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
