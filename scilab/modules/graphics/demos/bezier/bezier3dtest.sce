//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

// Show a Beziercurve of dimension 3

function bezier3dtest ()
    my_handle = scf(100001);
    clf(my_handle,"reset");

    p=[-1,-1,-1;0,-1,-1;1,0,0;1,1,0;0,1,1;-1,1,0]'
    bezier3d(p);

    demo_viewCode("bezier3dtest.sce");
endfunction

bezier3dtest();
clear bezier3dtest;
