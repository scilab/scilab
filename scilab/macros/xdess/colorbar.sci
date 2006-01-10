function colorbar(umin, umax, colminmax)

  //  PURPOSE
  //     Draw a colorbar for a plot3d, fec, Sgrayplot, etc...
  //
  //  PARAMETERS
  //     umin : min value of the plot
  //     umax : max value of the plot
  //     colminmax : (optional) a vector with 2 integer components
  //                 the first is the color number (of the current
  //                 colormap) associated with umin
  //                 the second the max color number ....
  //                 default : [1 nb_colors] where nb_colors is
  //                 the number of colors of the current colormap.
  //                 May be useful to deal with a part of the colormap
  //                 (for instance using fec or plot3d)
  //                 
  //  CAUTION
  //     this function may be used BEFORE a plot3d, fec, Sgrayplot, ...
  //     It is important because this function set and change the
  //     frame for the plot. This way the colorbar is not part of 
  //     the "associated" plot and so is not modified by a zoom or 
  //     a rotation of the plot.
  //
  //  AUTHOR
  //     Bruno Pincon
  //
  //  EXAMPLES
  //     see the help page
     
  if ~exists("colminmax","local") then 
     nb_colors = xget("lastpattern") // modif for bug 1122 (bruno 7 december 2004)
     colminmax = [1 nb_colors]
  else
     nb_colors = colminmax(2) - colminmax(1) + 1
  end
  
  nb_grad = 5
  // go on... 
  vec_ones = ones(1,nb_colors)

  x1 = -0.3 ; x2 = 0.1 ; 
  x_polys = [x1 ; x2 ; x2 ; x1] * vec_ones
  y1 = 0.1 ; y2 = 0.9
  y = linspace(y1,y2,nb_colors+1)
  y_polys = [y(1:$-1) ; y(1:$-1) ; y(2:$) ; y(2:$)] 
    
  xtics = x2*ones(1,nb_grad) ; dx_tics = 0.05 ; 
  ytics = linspace(y1, y2, nb_grad) ; dy_tics = 0

  valeurs = linspace(umin,umax,nb_grad);
  rect = xstringl(0, 0, string(umin))
  dy_cor = -rect(4)*0.5
  xmarks = xtics + 3*dx_tics ; ymarks = ytics + dy_cor; 

  
  if get('figure_style')=='old' then
    fg_color = xget("foreground")

    wr = xgetech()
    wrect_cb = [wr(1)+0.85*wr(3) , wr(2) , 0.15*wr(3) , wr(4)]
    wrect_pl = [wr(1) , wr(2) , 0.85*wr(3) , wr(4)]
    xsetech(wrect=wrect_cb,frect=[0 0 1 1], arect=0.125*[1 1 1 1])

    
    xfpolys(x_polys, y_polys, -(colminmax(1):colminmax(2)))
    xset("color", fg_color) ;
    xpoly([x1 x2 x2 x1],[y1 y1 y2 y2],"lines",1)
    xnumb(xmarks, ymarks , valeurs)
    xsegs([xtics ; xtics+dx_tics ],[ytics ; ytics+dy_tics],fg_color)
    
    xsetech(wrect=wrect_pl)
  else //new graphic mode
    //defer the drawing to avoid binking
    f=gcf()
    id=f.immediate_drawing=='on';
    if id then f.immediate_drawing=='off',end

    // get current axes and properties
    a=gca(); 
    fg_color=a.foreground
    wr=a.axes_bounds; //get the rectangle of the current axes
     
    //replace current axes by two axes: ones for the plot and one for the
    //colorbar
   
    a_cb=newaxes(); //create a axes for the colorbar and set itsproperties
    a_cb.axes_visible='off'
    a_cb.axes_bounds=[wr(1)+0.85*wr(3) , wr(2) , 0.15*wr(3) , wr(4)];
    
    a_cb.margins=0.125*[1 1 1 1];
    a_cb.data_bounds=[0 0;1 1];
    a_cb.foreground=a.foreground;
    a_cb.background=a.background;
    
    //draw the colorbar
    xfpolys(x_polys, y_polys, -(colminmax(1):colminmax(2)))
    xpoly([x1 x2 x2 x1],[y1 y1 y2 y2],"lines",1);
    //shoud use the y_axis graduation instead
    xnumb(xmarks, ymarks , valeurs)
    xsegs([xtics ; xtics+dx_tics ],[ytics ; ytics+dy_tics],a_cb.foreground)

    a_pl=a;
    a_pl.axes_bounds=[wr(1) , wr(2) , 0.85*wr(3) , wr(4)]
    sca(a_pl)
    //reset the initial value 
    if id then f.immediate_drawing=='on',end

  end
endfunction






