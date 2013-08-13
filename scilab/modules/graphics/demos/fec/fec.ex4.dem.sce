// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_fec4()
    mode(0);

    path=get_absolute_file_path("fec.ex4.dem.sce");
    exec(path+"fec_demo.sci",-1);

    curFig             = scf(100001);
    clf(curFig,"reset");

    // compute the mesh
    N=20;n=1:N;
    x=cos(n*2*%pi/N);
    y=sin(n*2*%pi/N);

    noeul=[(1:(N))', x', y',zeros(N,1);(N+1),0,0,0];

    trianl=[];
    for i=1:(N-1), trianl=[trianl;i,i,i+1,N+1,0]; end
    trianl=[trianl;N,N,1,N+1,0];

    // Set the colormap

    xset("colormap",jetcolormap(128));

    drawlater();
    colorbar(1,N+1);
    rect=[-1.2,-1.2,1.2,1.2];
    fec(noeul(:,2),noeul(:,3),trianl,(1:N+1)',"030"," ",rect);
    meshvisu(3,rect);
    a=gca();
    a.title.text="finite element display with colorbar";
    a.title.font_size=3;
    drawnow();

    demo_viewCode("fec.ex4.dem.sce");
    mode(0);

endfunction

demo_fec4()
clear demo_fec4;
