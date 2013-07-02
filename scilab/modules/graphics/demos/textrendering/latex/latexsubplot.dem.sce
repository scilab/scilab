// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2009 - DIGITEO - Yann COLLETTE
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_latexsubplot()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("latexsubplot.dem.sce");

    // DEMO START

    x = 1:0.25:10;
    y1 = x + sin(4*%pi*x / 10);
    y2 = 1 + cos(4*%pi*x / 10) * (4*%pi/10);

    subplot(2,1,1);
    plot(x,y1,"k-");
    xstring(5,3,'$\leftarrow f \left( x \right) = x + \sin \left( \frac{x \cdot 4 \cdot \pi}{10} \right)$');

    subplot(2,1,2);
    plot(x,y2,"k-");
    xstring(4.5,1.4,'$\leftarrow \frac{df \left( x \right)}{dx} = 1 + \frac{4 \cdot \pi}{10} \cdot \cos \left( \frac{x \cdot 4 \cdot \pi}{10} \right)$');

    // DEMO END
endfunction

demo_latexsubplot();
clear demo_latexsubplot;