//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function demo_cplxroot()

    exec("SCI/modules/graphics/demos/surface/Macros.sci",-1);

    my_handle = scf(100001);
    clf(my_handle,"reset");
    cplxroot(2,20,130,45);
    demo_viewCode("cplxroot.sce");

endfunction

demo_cplxroot();
clear demo_cplxroot;