function legends(leg, style, opt, with_box)
//
// PURPOSE
//    draw legends on a plot
// AUTHORS
//    F. Delebecque + slight modif from B. Pincon 
//   

  
  rhs=argn(2)
 
  if rhs < 2 then, error("bad number of arguments"), end
  if type(leg) ~= 10 then, 
    error("first arg may be a vector of strings"), 
  end
  nleg=size(leg,'*')
  if type(style) ~= 1&type(style) ~= 9 then, 
    error("second argument type may be 1 or 9  "), 
  end  
  if ~exists("opt","local") then
    opt=5
  elseif typeof(opt)=="string" then
    select opt
      case "ur" then, opt=1,
      case "ul" then, opt=2,
      case "ll" then, opt=3,       
      case "lr" then, opt=4,              
      case "?"  then, opt=5,
    else
      error("bad value for argument opt")
    end
  end
  if ~exists("with_box","local") then, with_box=%t, end

  
  if or(size(style)==1) then   style=matrix(style,1,-1),end
  ns=size(style,2)

//  if ns<=nleg then
//     style(1,ns+1:nleg)=-1
//     ns=nleg
//   else
//     ns=nleg
//   end



  fs=get('figure_style')=='old'
  
  [r1,r2,logflag,arect]=xgetech()  //preserve current graphic context
  if ~fs then 
    f=gcf()
    vis=f.visible;
    old_ax=gca(),
    r2=old_ax.data_bounds;
    arect=old_ax.margins;
    r1=old_ax.axes_bounds;
  else
    [r1,r2,logflag,arect]=xgetech() 
  end  
  
  
  
  //create small axes on the top left corner (the axes is choosen very
  //small to avoid it can be selected for rotation in new graphic mode
  //case (a little tricky)
  xsetech(wrect=[r1(1),r1(2),r1(3)/1000,r1(4)/1000],frect=[0 0 1,1]/1000,arect=[0,0,0,0])
  xmin=arect(1);xmax=1-arect(2);ymin=-1+arect(4);ymax=-arect(3);

  if fs then 
    xclip()//no clipping
  else
    cur_ax=gca(),
    cur_ax.clip_state='off';

  end  

  dy=ymax-ymin
  drx=(xmax-xmin)/20 //length of the line

  e=drx/2  // horizontal space between two columns

  bbx=[]
  for k=1:nleg
    r=xstringl(0,0,leg(k));
    bbx=[bbx;r(3:4)]; //[width height]
  end

  coef = 0.5
  height=sum(bbx(:,2))
  dh = coef*height/nleg
  height = height+(nleg-1)*dh + 2*dy/60
  
  //height=sum(bbx(:,2))+2*dy/60
  width=max(bbx(:,1))+drx*1.2+2*drx/5

  //upper left coordinates
  if size(opt,'*')>1 then 
    pos=opt;opt=0 ;
  elseif opt<1|opt>5 then 
     error('opt can take value in 1 2 3 4 5')
  end
  select opt
  case 1 then
    pos=[xmax-width-drx/5,ymax-dy/60]
  case 2 then
    pos=[xmin+drx/5,ymax-dy/60]
  case 3 then
    pos=[xmin+drx/5,ymin+height+dy/60]
  case 4 then 
    pos=[xmax-width-drx/5,ymin+height+dy/60]
  case 5 then
    rect=dragrect([xmax-width-drx/5,ymax-dy/60,width,height])
    pos=rect(1:2)
  end
 
  x=pos(1)+drx/5
  y=pos(2)-dy/60
  
  if fs then
    if with_box then xrect(pos(1),pos(2),width,height),end
    linestyle=xget('line style')
    clr=xget('color')
    for k=1:nleg
      if k<=size(style,2) then
	if style(1,k)<= 0 then
	  if size(style,1)==2 then  xset("color",style(2,k));end
	  xpolys(x+drx/2,y-bbx(k,2)/2,style(1,k))
	  xset('color',clr)
	else
	  if size(style,1)==2 then  xset("line style",style(2,k));end
	  xsegs([x;x+drx],[y;y]-bbx(k,2)/2,style(1,k))
	end
      end
      xstring(x+drx*1.2,y-bbx(k,2),leg(k))
      y=y-bbx(k,2)-dh
    end
    //reset saved graphic context
    xset('line style',linestyle)
    xset('color',clr)
    xsetech(wrect=r1,frect=r2,arect=arect,logflag=logflag)
  else
    drawlater()
    R=[]
    if with_box then xrect(pos(1),pos(2),width,height),R=gce();end
    a=gca()
    a.clip_state='off';
    for k=1:nleg
      if k<=size(style,2) then
	if type(style)==9 then
	  h=style(k)
	  select h.type
	    case "Polyline"
	    if h.polyline_style==5 then //patch
	      xfpoly([x;x+drx;x+drx;x;x],[y-bbx(k,2);y-bbx(k,2);y;y;y-bbx(k,2)]);r=gce();
	      r.foreground=h.foreground;
	      r.thickness=h.thickness;
	      r.polyline_style=h.polyline_style;
	      r.line_style=h.line_style;
	    else
	      if stripblanks(h.mark_mode)=='off'
		xpoly([x;x+drx],[y;y]-bbx(k,2)/2,'lines');r=gce();
		r.foreground=h.foreground;
		r.thickness=h.thickness;
		r.polyline_style=h.polyline_style;
		r.line_style=h.line_style;
	      else
		xpoly(x+drx/2,y-bbx(k,2)/2);r=gce();
		r.foreground=h.foreground;
		r.thickness=h.thickness;
		r.mark_style=h.mark_style;
		r.mark_size=h.mark_size;
	      end
	    end
	  else
	    error('Only handle on polyline,are allowed')
	  end
	else
	  if style(1,k)<= 0 then
	    xpoly(x+drx/2,y-bbx(k,2)/2)
	    r=gce(),
	    r.mark_mode='on'
	    r.mark_style=-style(1,k)
	    if size(style,1)==2 then r.foreground=style(2,k);end
	  else
	    xpoly([x;x+drx],[y;y]-bbx(k,2)/2,'lines')
	    r=gce(),
	    r.foreground=style(1,k)
	    if size(style,1)==2 then r.line_style=style(2,k);end
	  end
	end
      end
      R=[R,r]
      xstring(x+drx*1.2,y-bbx(k,2),leg(k))
      r=gce()
      R=[R,r]
      y=y-bbx(k,2)-dh
    end
    glue(R)
    R=gce()
    draw(R)
 
    set('current_axes',old_ax),
    drawnow()
    f.visible=vis;
  end
endfunction
