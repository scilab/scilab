//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Jean-Baptiste Silvy
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
//
//

//
// This is the demonstration script of fec
// used when calling fec without any parameter.
//

function [] = %_fec()
    mode(0)
    exec("SCI/modules/graphics/demos/fec/fec_demo.sci",-1);
    clf();
    xset("colormap",jetcolormap(128));
    N=20;
    n=1:N;
    x=cos(n*2*%pi/N);
    y=sin(n*2*%pi/N);
    noeuds=N;
    noeul=[(1:(N))',x',y',0*ones(N,1)];
    noeul=[noeul;(N+1),0,0,0];
    trianl=[];
    for i=1:(N-1),trianl=[trianl;i,i,i+1,N+1,0];end
    trianl=[trianl;N,N,1,N+1,0];
    triang=N;
    rect=[-1.2,-1.2,1.2,1.2];
    drawlater();
    fec(noeul(:,2),noeul(:,3),trianl,(1:N+1)',"030"," ",rect);
    meshvisu(3,rect);
    drawnow();
    show_window();

    mode(-1)

endfunction
