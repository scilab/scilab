//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// a random polygon and a bezier curve

function beziertest

    my_handle = scf(100001);
    clf(my_handle,"reset");

    plot2d(-0.2,-0.2,0,"011"," ",[-0.2,-0.2,1.2,1.2]);
    title("Bezier Test : random polygon and bezier curve","fontsize",3);
    rand("uniform");
    p = rand(2,5);
    t = linspace(0,1,300);
    s = bezier(p,t);

    plot2d(p(1,:),p(2,:),1);
    plot2d(s(1,:),s(2,:),2);

    demo_viewCode("beziertest.sce");

endfunction

beziertest();
clear beziertest;
