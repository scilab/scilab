function ged_insert(k,win)
  //xset, xget used because ged should handle both old and new style

  global ged_current_figure
  global ged_cur_fig_handle
  
  ged_current_figure=xget('window')
  xset('window',win) 
  isold=get('figure_style')=='old'
  if isold&k>3 then 
    message('this menu does not apply to old style graphics')
    xset('window',ged_current_figure)
    clearglobal ged_current_figure
    clear ged_current_figure
    return
  end
  
  scf(win);
  ged_cur_fig_handle=gcf();
  
  mess1 = "Press the right mouse button (during a while) to stop the line creation";
  mess2 = "Press any mouse button to complete the object drawing"
  entities=['Rectangle','Segment','Polyline','Arrow','Double Arrow','Text','Circle']

  f=gcf();pix=f.pixmap;f.pixmap='on'
  rep(3)=-1
  select k
  case 6 then //Rectangle
    [btn,xc,yc]=xclick()
    xrect(xc,yc,0,0)
    show_pixmap()
    r=gce();r.foreground=-1;
    r.clip_state='off';
    xinfo(mess2)
    while rep(3)==-1 do
      rep=xgetmouse()
      r.data=[mini(xc,rep(1)),maxi(yc,rep(2)),abs(xc-rep(1)),abs(yc-rep(2))]
      show_pixmap()
      xinfo(mess2)
    end 
  case 1 then //Single Line
    [btn,xc,yc]=xclick()
    xpoly([xc;xc],[yc;yc])
    show_pixmap()
    r=gce();r.foreground=-1;
    r.clip_state='off';
    xinfo(mess2)
    while rep(3)==-1 do
      rep=xgetmouse()
      r.data=[xc,yc;rep(1),rep(2)]
      show_pixmap()
      xinfo(mess2)
    end 
  case 2 then //Polyline (stroken line)
    [btn,xc,yc]=xclick()
    xpoly([xc;xc],[yc;yc])
    show_pixmap()
    r=gce();r.foreground=-1;
    r.data(:,3)=0.;
    r.clip_state='off';
    xinfo(mess1)
     while %t
      while rep(3)==-1 do
	rep=xgetmouse()
	r.data($,:)= [rep(1),rep(2),0]
	show_pixmap()
	xinfo(mess1)
      end 
      if rep(3)==2 then break,end
      rep(3)=-1;
      r.data=[r.data;r.data($,:)]
    end
  case 3 // Arrow (single arrow)
    [btn,xc,yc]=xclick()
    xpoly([xc;xc],[yc;yc])
    show_pixmap()
    r=gce();r.foreground=-1;
    r.data(:,3)=0.;
    r.polyline_style = 4;
    r.arrow_size_factor=5; // change the factor to have a nice arrow
    r.clip_state='off';
    xinfo(mess2)
    yc = [];
    while rep(3)==-1 do
      rep=xgetmouse()
      r.data($,:)= [rep(1),rep(2),0]
      show_pixmap()
      xinfo(mess2)
    end 
  case 4 // Double Arrow
    [btn,xc,yc]=xclick()
  
    xpoly([xc;xc],[yc;yc])
    r1=gce();r.foreground=-1;
    r1.data(:,3)=0.;
    r1.polyline_style = 4;
    r1.arrow_size_factor=5; // change the factor to have a nice arrow
    r1.clip_state='off';
    
    // second polyline (for the seconf arrow from end to origin)
    xpoly([xc;xc],[yc;yc])
    //xpoly([r.data(2,1);r.data(1,1)],[r.data(2,2);r.data(1,2)])
    r2=gce();r.foreground=-1;
    r2.data(:,3)=0.;
    r2.polyline_style = 4;
    r2.arrow_size_factor=5; // change the factor to have a nice arrow
    r2.clip_state='off';
    
    xinfo(mess2)
    show_pixmap()
    while rep(3)==-1 do
      rep=xgetmouse()
      r1.data($,:)= [rep(1),rep(2),0]
      r2.data(1,:)= [rep(1),rep(2),0]
      show_pixmap()
      xinfo(mess2)
    end 
  
  case 5 then //Text
    
  case 7 then //Circle
    [btn,xc,yc]=xclick()
    xarc(xc,yc,0,0,0,64*360)
    show_pixmap()
    r=gce();r.foreground=-1;
    r.clip_state='off';
    xinfo(mess2)
    while rep(3)==-1 do
      rep=xgetmouse()
      r.data=[mini(xc,rep(1)),maxi(yc,rep(2)),abs(xc-rep(1)),abs(yc-rep(2)),0,64*360]
      show_pixmap()
      xinfo(mess2)
    end 

  end
  f.pixmap=stripblanks(pix)
  
  xset('window',ged_current_figure)
endfunction

