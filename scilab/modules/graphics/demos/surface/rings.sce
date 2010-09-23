//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// rings()
// =============================================================================

function rings()

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);
    my_handle = scf(100001);

    rr = 0.2;
    t  = linspace(0,2*%pi,10);
    s  = linspace(0,2*%pi,41); n=length(s);
    r  = dup(1+cos(t)*rr,n)'; m=length(t);
    x  = dup(cos(s),m).*r; y=dup(sin(s),m).*r;
    z  = dup(sin(t)*rr,n)';
    X  = [x;(x+1.3);(x-1.3)];
    Y  = [y;-z;-z];
    Z  = [z;y;y];

    clf(my_handle,"reset");
    demo_viewCode("rings.sce");

    plot3d2(X,Y,Z,[m,2*m]);

endfunction

rings();
clear rings;
