//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

function demo_scilab()

  // Définition des variables

x=[1 2 3 4 5 6 7 8 9 10 11 12];
y=[ 13702 0 0 ; ..
    13758 0 0 ; ..
    16944 0 0 ; ..
    16324 0 0 ; ..
    15399 0 0 ; ..
    14988 0 0 ; ..
    10275 0 0 ; ..
    10195 0 0 ; ..
    14899 0 0 ; ..
    18884 0 0 ; ..
    16919 0 0; ..
    14759 0 0];

// bar()
f = gcf() ;
f.pixmap = 'on' ;

bar(x,y);

// Ajustements

axes = gca();
axes.data_bounds = [ 0 0 ; 12.5 25000 ];
axes.x_ticks.labels = ['Jan';'Feb';'Mar';'Apr';'May';'Jun';'Jul';'Aug';'Sep';'Oct';'Nov';'Dec'];
axes.grid=[-1 0];
axes.y_label.text = "Number of downloads" ;
t = axes.title ;
t.text = "Scilab download statistics (April 2007)" ;
t.font_size = 5 ;

years = axes.children.children;
year2005 = years(3);
year2006 = years(2);
year2007 = years(1);

year2005.background = 32;
year2006.background = 5;
year2007.background = 33;

year2005.bar_width = 0.25;
year2006.bar_width = 0.25;
year2007.bar_width = 0.25;

legends(['Year 2005';'Year 2006';'Year 2007'],[32;5;33],[9.5;19500]);

histogramme = gcf();
histogramme.background = 17;

legend_axes = histogramme.children(1);

legend_axes.children.children(2).thickness = 4;
legend_axes.children.children(4).thickness = 4;
legend_axes.children.children(6).thickness = 4;

show_pixmap() ;


  realtimeinit(1.0) ;
  for i=1:7
    realtime(i) ;
  end ;

x=[1 2 3 4 5 6 7 8 9 10 11 12];
y=[ 13702 16902 0 ; ..
    13758 19959 0 ; ..
    16944 23254 0 ; ..
    16324 18323 0 ; ..
    15399 19648 0 ; ..
    14988 17160 0 ; ..
    10275 10996 0 ; ..
    10195 14316 0 ; ..
    14899 18410 0 ; ..
    18884 23924 0 ; ..
    16919 21535 0; ..
    14759 19979 0];


// bar()
clf() ;

bar(x,y);

axes = gca();
axes.data_bounds = [ 0 0 ; 12.5 25000 ];
axes.x_ticks.labels = ['Jan';'Feb';'Mar';'Apr';'May';'Jun';'Jul';'Aug';'Sep';'Oct';'Nov';'Dec'];
axes.grid=[-1 0];
axes.y_label.text = "Number of downloads" ;
t = axes.title ;
t.text = "Scilab download statistics" ;
t.font_size = 5 ;

years = axes.children.children;
year2005 = years(3);
year2006 = years(2);
year2007 = years(1);

year2005.background = 32;
year2006.background = 5;
year2007.background = 33;

year2005.bar_width = 0.25;
year2006.bar_width = 0.25;
year2007.bar_width = 0.25;

legends(['Year 2005';'Year 2006';'Year 2007'],[32;5;33],[9.5;19500]);

histogramme = gcf();
histogramme.background = 17;

legend_axes = histogramme.children(1);

legend_axes.children.children(2).thickness = 4;
legend_axes.children.children(4).thickness = 4;
legend_axes.children.children(6).thickness = 4;

show_pixmap() ;

  realtimeinit(1.0) ;
  for i=1:7
    realtime(i) ;
  end ;

x=[1 2 3 4 5 6 7 8 9 10 11 12];
y=[ 13702 16902 22765 ; ..
    13758 19959 21321 ; ..
    16944 23254 24971 ; ..
    16324 18323 0 ; ..
    15399 19648 0 ; ..
    14988 17160 0 ; ..
    10275 10996 0 ; ..
    10195 14316 0 ; ..
    14899 18410 0 ; ..
    18884 23924 0 ; ..
    16919 21535 0; ..
    14759 19979 0];


// bar()
clf() ;
bar(x,y);

// Ajustements

axes = gca();
axes.data_bounds = [ 0 0 ; 12.5 25000 ];
axes.x_ticks.labels = ['Jan';'Feb';'Mar';'Apr';'May';'Jun';'Jul';'Aug';'Sep';'Oct';'Nov';'Dec'];
axes.grid=[-1 0];
axes.y_label.text = "Number of downloads" ;
t = axes.title ;
t.text = "Scilab download statistics" ;
t.font_size = 5 ;

years = axes.children.children;
year2005 = years(3);
year2006 = years(2);
year2007 = years(1);

year2005.background = 32;
year2006.background = 5;
year2007.background = 33;

year2005.bar_width = 0.25;
year2006.bar_width = 0.25;
year2007.bar_width = 0.25;

legends(['Year 2005';'Year 2006';'Year 2007'],[32;5;33],[9.5;19500]);

histogramme = gcf();
histogramme.background = 17;

legend_axes = histogramme.children(1);

legend_axes.children.children(2).thickness = 4;
legend_axes.children.children(4).thickness = 4;
legend_axes.children.children(6).thickness = 4;

show_pixmap() ;

  realtimeinit(1.0) ;
  for i=1:10
    realtime(i) ;
  end ;
  
endfunction
