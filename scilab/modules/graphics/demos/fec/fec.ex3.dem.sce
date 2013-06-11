// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_fec_example3()

    mode(0);

    curFig             = scf(100001);
    clf(curFig,"reset");

    Sfgrayplot();
    a=gca();
    a.title.font_size=3;

    mode(-1);

    demo_viewCode("fec.ex3.dem.sce");

endfunction

demo_fec_example3();
clear demo_fec_example3;
