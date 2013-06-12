//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file is distributed under the same license as the Scilab package.
//

function demo_contourf2()

    my_handle             = scf(100001);
    clf(my_handle,"reset");



    function z=peaks(x,y)
        x1=x(:).*.ones(1,size(y,"*"));
        y1=y(:)'.*.ones(size(x,"*"),1);
        z =  (3*(1-x1).^2).*exp(-(x1.^2) - (y1+1).^2) ...
        - 10*(x1/5 - x1.^3 - y1.^5).*exp(-x1.^2-y1.^2) ...
        - 1/3*exp(-(x1+1).^2 - y1.^2)
    endfunction

    function z=peakit()
        // pour avoir un truc voisin de matlab avec 80 points
        x=-4:0.1:4;y=x;z=peaks(x,y);
    endfunction

    z=peakit();
    levels=[-6:-1,-logspace(-5,0,10),logspace(-5,0,10),1:8];
    m=size(levels,"*");
    n = fix(3/8*m);
    r = [(1:n)'/n; ones(m-n,1)];
    g = [zeros(n,1); (1:n)'/n; ones(m-2*n,1)];
    b = [zeros(2*n,1); (1:m-2*n)'/(m-2*n)];
    h = [r g b];
    xset("colormap",h);
    xset("fpf"," ");
    clf();

    contourf([],[],z,[-6:-1,-logspace(-5,0,10),logspace(-5,0,10),1:8],0*ones(1,m))
    xset("fpf","");
    halt(_("Press Return to continue ... \n"));

    if is_handle_valid(my_handle) == %f then
        return;
    end

    clf();
    contourf([],[],z,[-6:-1,-logspace(-5,0,10),logspace(-5,0,10),1:8]);
    demo_viewCode("contourf2.dem.sce");

endfunction

demo_contourf2();
clear demo_contourf2;
