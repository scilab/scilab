// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
// Copyright (C) 2009 - DIGITEO - Michael BAUDIN
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_simple()

    my_handle             = scf(100001);
    clf(my_handle,"reset");
    demo_viewCode("simple.dem.sce");

    // DEMO START

    function y = myquad ( x )
        y = x(1)^2+x(2)^2
    endfunction

    N = 50
    xdata = linspace(-1,1,N);
    ydata = linspace(-1,1,N);

    for i=1:N
        for j=1:N
            zdata(i,j) = myquad( [xdata(i) ydata(j)] );
        end
    end

    contour ( xdata , ydata , zdata , [0.1 0.5 1.0 1.5] )
    gg = gce();
    gg.title.text = "$f(\mathbf{x}) = x_1^2+x_2^2$";

    // DEMO END
endfunction

demo_simple();
clear demo_simple;