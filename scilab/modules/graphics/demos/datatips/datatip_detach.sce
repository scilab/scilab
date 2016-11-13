// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2016 - Scilab Enterprises
// Copyright (C) 2016 - Caio SOUZA
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function datatip_detach_demo()
    h = scf(100001);
    clf(h,"reset");
    fig=gcf();

    title(_("Datatips detached from curve"),"fontsize",3);

    x = 0:0.1:3*%pi;
    y = sin(x);
    subplot(2,1,1);
    plot2d(x,y);
    e=gce();
    p=e.children(1);
    l = int(length(x) / 4);
    d1 = datatipCreate(p, l);
    d1.foreground = 3
    d2 = datatipCreate(p, 2*l);
    d2.foreground = 2
    d3 = datatipCreate(p, 3*l);
    d3.foreground = 5;

    subplot(2,1,2);
    plot2d(x,y);
    e=gce();
    p=e.children(1);
    l = int(length(x) / 4);
    d1 = datatipCreate(p, l);
    d1.detached_position = [ 0.5,-0.5,0];
    d1.foreground = 3
    d1.line_style = 7
    d2 = datatipCreate(p, 2*l);
    d2.detached_position = [ 3.4,0.5,0];
    d2.foreground = 2
    d2.line_style = 3
    d3 = datatipCreate(p, 3*l);
    d3.detached_position = [ 8,-0.5,0];
    d3.foreground = 5;
    d3.line_style = 1;
endfunction

datatip_detach_demo()
clear datatip_detach_demo
