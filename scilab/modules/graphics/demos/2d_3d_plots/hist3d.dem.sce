// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2012 - Samuel GOUGEON 
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_hist3d()

  my_handle             = scf(100001);
  clf(my_handle,"reset");
  demo_viewCode("hist3d.dem.sce");
  
  // DEMO START
  my_plot_desc          = "hist3d: Average monthly temperatures in french cities";
  my_handle.figure_name = my_plot_desc;
  
  towns = ["Agen" "Bastia" "Chamonix" "Cognac" "Hyères" "Le Mans" "Le Puy" ..
           "Lille" "Lorient" "Mende" ]
  months = [_('January') _('Febuary') _('March') _('April') _('May') _('June')..
            _('July') _('August') _('September') _('October') _('November') ..
            _('December') _('Average') ]
  months = "$\magnification{1300}\rotatebox{90}{\mbox{" + months + "}}$"

  T = [5.7 5.9 9.8 12.1 16.0 19.4 21.8 21.5 18.4 14.7 9.3 5.2 13.5
       9.4 9.4 11.2 13.3 17.3 21.0 24.1 24.5 21.2 17.6 13.3 10.4 16.1
       -2.3 -0.8 3.0 6.6 11.2 14.4 15.5 15.9 12.5 8.6 2.7 -1.6 7.2
       6.1 5.9 9.7 11.9 15.8 19.0 21.1 20.9 17.9 14.4 9.3 5.6 13.3
       8.7 8.9 11.0 13.1 16.7 20.3 23.2 23.3 20.2 16.8 12.6 9.8 15.4
       5.0 5.5 8.3 10.7 14.5 17.8 19.9 19.7 18.5 12.8 8.1 5.4 12.8
       1.0 1.3 4.8 7.2 11.3 14.3 17.6 17.1 13.5 9.9 4.6 1.8 8.8
       3.7 4.1 7.1 9.8 13.5 16.2 18.6 18.5 15.5 11.7 7.2 4.2 10.8
       6.6 6.7 8.6 10.3 13.6 16.2 18.1 18.0 16.0 13.0 9.4 7.0 12.0
       0.6 1.3 3.7 5.5 9.6 13.1 16.2 16.0 12.8 8.8 3.8 1.8 7.8
      ];
  hist3d(T)
  ax = gca()
  ax.y_ticks = tlist(['ticks' 'locations' 'labels'], (0:12)+0.5, months)
  ax.x_ticks = tlist(['ticks' 'locations' 'labels'], (0:9)+0.5, towns)
  zlabel("T [°C]")
  xtitle(_("Average monthly temperatures in french cities"),"","")
  ax.title.font_size = 3
  ax.rotation_angles = [28 19] 
  xtitle(my_plot_desc," "," "," ");
  // DEMO END
  
endfunction

demo_hist3d();
clear demo_hist3d;

