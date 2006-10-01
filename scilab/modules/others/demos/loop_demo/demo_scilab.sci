function demo_scilab()
  demo_help demo_scilab
  // Définition des variables

x=[1 2 3 4 5 6 7 8 9 10 11 12];
y=[ 0 0 0 ; ..
    0 0 0 ; ..
    8231 0 0 ; ..
    791 0 0 ; ..
    7111 0 0 ; ..
    6812 0 0 ; ..
    3919 0 0 ; ..
    3809 0 0 ; ..
    7450 0 0 ; ..
    9039 0 0 ; ..
    7708 0 0; ..
    6223 0 0];

// bar()
f = gcf() ;
f.pixmap = 'on' ;

bar(x,y);
SetPosition() ;
// Ajustements

axes = gca();
axes.data_bounds = [ 0 0 ; 12.5 20000 ];
axes.x_ticks.labels = ['Jan';'Feb';'Mar';'Apr';'May';'Jun';'Jul';'Aug';'Sep';'Oct';'Nov';'Dec'];
axes.grid=[-1 0];
axes.y_label.text = "Number of downloads" ;
t = axes.title ;
t.text = "Scilab download statistics" ;
t.font_size = 5 ;

years = axes.children.children;
year2003 = years(3);
year2004 = years(2);
year2005 = years(1);

year2003.background = 32;
year2004.background = 5;
year2005.background = 33;

year2003.bar_width = 0.25;
year2004.bar_width = 0.25;
year2005.bar_width = 0.25;

legends(['Year 2003';'Year 2004';'Year 2005'],[32;5;33],[9.5;19500]);

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
y=[ 0 7687 0 ; ..
    0 8003 0 ; ..
    8231 10450 0 ; ..
    791 9124 0 ; ..
    7111 11099 0 ; ..
    6812 10718 0 ; ..
    3919 13869 0 ; ..
    3809 10628 0 ; ..
    7450 13022 0 ; ..
    9039 16442 0 ; ..
    7708 15116 0; ..
    6223 11384 0];


// bar()
clf() ;
SetPosition() ;
bar(x,y);

axes = gca();
axes.data_bounds = [ 0 0 ; 12.5 20000 ];
axes.x_ticks.labels = ['Jan';'Feb';'Mar';'Apr';'May';'Jun';'Jul';'Aug';'Sep';'Oct';'Nov';'Dec'];
axes.grid=[-1 0];
axes.y_label.text = "Number of downloads" ;
t = axes.title ;
t.text = "Scilab download statistics" ;
t.font_size = 5 ;

years = axes.children.children;
year2003 = years(3);
year2004 = years(2);
year2005 = years(1);

year2003.background = 32;
year2004.background = 5;
year2005.background = 33;

year2003.bar_width = 0.25;
year2004.bar_width = 0.25;
year2005.bar_width = 0.25;

legends(['Year 2003';'Year 2004';'Year 2005'],[32;5;33],[9.5;19500]);

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
y=[ 0 7687 13588 ; ..
    0 8003 13597 ; ..
    8231 10450 16772 ; ..
    791 9124 16169 ; ..
    7111 11099 15267 ; ..
    6812 10718 14855 ; ..
    3919 13869 10147 ; ..
    3809 10628 9998 ; ..
    7450 13022 14445 ; ..
    9039 16442 18212 ; ..
    7708 15116 16238; ..
    6223 11384 14335];

// bar()
clf() ;
SetPosition() ;
bar(x,y);

// Ajustements

axes = gca();
axes.data_bounds = [ 0 0 ; 12.5 20000 ];
axes.x_ticks.labels = ['Jan';'Feb';'Mar';'Apr';'May';'Jun';'Jul';'Aug';'Sep';'Oct';'Nov';'Dec'];
axes.grid=[-1 0];
axes.y_label.text = "Number of downloads" ;
t = axes.title ;
t.text = "Scilab download statistics" ;
t.font_size = 5 ;

years = axes.children.children;
year2003 = years(3);
year2004 = years(2);
year2005 = years(1);

year2003.background = 32;
year2004.background = 5;
year2005.background = 33;

year2003.bar_width = 0.25;
year2004.bar_width = 0.25;
year2005.bar_width = 0.25;

legends(['Year 2003';'Year 2004';'Year 2005'],[32;5;33],[9.5;19500]);

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
