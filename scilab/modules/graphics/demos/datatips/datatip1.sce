// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer <serge.steer@inria.fr>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at;
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function datatip1

    my_handle = scf(100001);
    clf(my_handle,"reset");
    fig=gcf();fig.axes_size=[760 460];

    title(_("datatips on 2D curves"),"fontsize",3);

    x1=linspace(-1,1,300)';
    plot(x1,x1.^3,x1,sinc(10*x1));
    e=gce();
    p1=e.children(1);//the handle on the sinc(10*x1) curve
    p2=e.children(2); //the handle on the x1^3  curve

    //Create 2 datatips on the sinc curve
    datatipCreate(p1,50); //by index
    datatipCreate(p1,[0.8 0.5]);//by nearest mesh point

    //Create 2 datatips on the x^3 curve
    //datatipSetStyle(p2,2,%f) //directional arrow no box
    //function s=mydisp(curve,pt,index),s=msprintf("%.2g",pt(1)),endfunction
    //pause;
    //datatipSetDisplay(p2,mydisp) // Change the default datatip label
    datatipCreate(p2,[0.1,0]);
    datatipCreate(p2,[0.8 0.4]);
    datatipSetStyle(p2,2,%f) //directional arrow no box

    datatipManagerMode(fig,"on") //activate the interactive editor


    demo_viewCode("datatip1.sce");

endfunction

datatip1();
clear datatip1;
