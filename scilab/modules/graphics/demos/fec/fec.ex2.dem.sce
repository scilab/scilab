// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO -
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_fec_example2()

    mode(0);

    path=get_absolute_file_path("fec.ex2.dem.sce");
    exec(path+"fec_demo.sci",-1);

    curFig             = scf(100001);
    clf(curFig,"reset");

    show_window();

    xset("colormap",jetcolormap(128));

    drawlater();
    amdbaR(path+"MESH");
    meshvisu();
    xtitle("a mesh");
    a=gca();
    a.title.font_size=3;
    drawnow();

    //xclick();
    messagebox("click to continue","modal");

    clf(curFig,"reset");
    drawlater();
    emc2C(1,6,path+"MESH.VAL");
    xtitle("display a fem solution");
    a=gca(); a.title.font_size=3;
    drawnow();
    //xclick();
    messagebox("click to continue","modal");

    clf();
    drawlater();
    emc2C(1,6,path+"MESH.VAL",[-2,-2,2,2]);
    emc2V(2,3,6,20,path+"MESH.VAL",[-2,-2,2,2]);
    xtitle("display a zoom of the solution together with the velocity field");
    a=gca();
    a.title.font_size=3;
    drawnow();

    mode(-1);
    demo_viewCode("fec.ex2.dem.sce");

endfunction

demo_fec_example2();
clear demo_fec_example2;
