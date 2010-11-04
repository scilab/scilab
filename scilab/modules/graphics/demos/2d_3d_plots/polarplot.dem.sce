// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Samuel GOUGEON
// Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function demo_polarplot()
  my_handle             = scf(100001);
  clf(my_handle,"reset");
  demo_viewCode("polarplot.dem.sce");

// DEMO START

  my_plot_desc          = "polarplot";
  my_handle.figure_name = my_plot_desc;

  messagebox(gettext("Please press enter into the Scilab console to see more polarplot examples"));
  nextMessage=gettext("Press enter to go on...");
  t=linspace(0,1,100)*%pi*2;
// only 1 quadrant:
  clf, polarplot(t/4,sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/4+%pi/2,sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/4+%pi,sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/4-%pi/2,sin(t/10)), input(nextMessage,"s");
// 2 quadrants:
  clf, polarplot(t/2,sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/2+%pi/2,sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/2+%pi,sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/2-%pi/2,sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/4-1,sin(t/10)), input(nextMessage,"s");
// with rect=
  clf, subplot(2,1,1), polarplot(t/4-1,sin(t/10)-0.3,rect=[-0.18 -0.02 0.27 0.3]), input(nextMessage,"s");

// else : full circle:
  clf, polarplot(t/2-1,sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t,sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t-1,sin(t/10)), input(nextMessage,"s");
// with negative radii:
  clf, polarplot(t,sin(t/10)-0.5), input(nextMessage,"s");
  clf, polarplot(t,13*sin(t/3)-7), input(nextMessage,"s");
  t=linspace(0,1,200)*%pi*2;

  clf, polarplot(t/10-%pi,4600*sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/2-%pi/2,4600*sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/2,4600*sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/2,3e5*sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/4,3e5*sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/4,1.4e5*sin(t/10)), input(nextMessage,"s");
  clf, polarplot(t/4,1.4e-5*sin(t/10)), input(nextMessage,"s");

  clf, t=linspace(0,1,100)*%pi*2; r=linspace(0.0,0.7,100); polarplot(t,r), input(nextMessage,"s");
  clf, t=linspace(0,1,100)*%pi*2; polarplot(t/10-%pi,sin(t/10)+0.1), input(nextMessage,"s");
  clf, t= 0:.01:2*%pi; polarplot([sin(7*t') sin(6*t')],[cos(8*t') cos(8*t')],[1,2]), input(nextMessage,"s");

  xtitle(my_plot_desc," "," "," ");

// DEMO END
endfunction

demo_polarplot();
clear demo_polarplot;
