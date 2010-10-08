// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2007-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_xgeom()

  my_handle             = scf(100001);
  clf(my_handle,"reset");
  demo_viewCode("xgeom.dem.sce");

  // DEMO START

  my_plot_desc          = "plot3d and use of xgeom";
  my_handle.figure_name = my_plot_desc;

  r = (%pi):-0.01:0;
  x = r.*cos(10*r);
  y = r.*sin(10*r);

  deff("[z]=Surf(x,y)","z=sin(x)*cos(y)");
  t=%pi*(-10:10)/10;

  // Plot the surface'
  fplot3d(t,t,Surf,35,45,"X@Y@Z",[19,2,3]);

  // Add 2d graphics on 3d graphic'
  z=(sin(x).*cos(y));
  [x1,y1] = geom3d(x,y,z);
  xpoly(x1,y1,"lines");

  // Adjust some plotting parameters'
  BackgroundColorId           = color(70,174,255);
  current_axe                 = gca();
  plot_3d                     = current_axe.children(2);
  plot_3d.hiddencolor         = 32;
  polyline                    = current_axe.children(1)';;
  polyline.foreground         = 8;
  current_axe.rotation_angles = [70,47];
  current_axe.background      = BackgroundColorId;

  // A second 2d graphics'
  [x1,y1] = geom3d([0,0],[0,0],[5,0]);
  xsegs(x1,y1);
  xstring(x1(1),y1(1),"The point (0,0,0)");

  xtitle(my_plot_desc," "," "," ");

  // DEMO END
endfunction


demo_xgeom();
clear demo_xgeom;