function R=legend(varargin)
  if get('figure_style')=='old' then
    error('This function cannot be used with old style graphics, see legends')
  end
  f=gcf()
  vis=f.immediate_drawing;
  narg=size(varargin)
  k0=1
  H=[]
  leg=[]
  Acur=gca();A=Acur;
  opt=1
  with_box=%t

  //  if type(varargin(1))==9 then //a handle
  //    H=varargin(1)
  //    if H.type=='Axes' then 
  //      A=H,
  //      //set("current_axes",A)
  //      k0=k0+1
  //      if k0<=narg&type(varargin(k0))==9 then //a handle
  //	H=varargin(k0);k0=k0+1
  //      end
  //    end
  //  end

  while type(varargin(k0))==9 then //a handle that could be an Axes, Agreg. or Polyline handle.
    tmpH=varargin(k0)
    if tmpH.type=='Axes' then
      A = tmpH;
    end
    H=[H;getvalidchildren(tmpH)'];
    //H=[getvalidchildren(tmpH)'; H];
    k0 = k0+1;
  end

  if H==[] then
    //walk subtree to get all proper children
    H=getvalidchildren(A)
  end

  //get all labels
  for k=k0:size(varargin)
    if type(varargin(k))<>10 then break,end
    vk=varargin(k)
    leg=[leg;vk(:)]
  end
  nleg=size(leg,'*')

  k0=k
  if k0<=narg&type(varargin(k0))==1 then opt=varargin(k0);k0=k0+1,end
  if k0<=narg&type(varargin(k0))==4 then with_box=varargin(k0);k0=k0+1,end

  arect=A.margins;
  r1=A.axes_bounds;
  //create small axes on the top left corner (the axes is choosen very
  //small to avoid it can be selected for rotation in new graphic mode
  //case (a little tricky)
  xsetech(wrect=[r1(1),r1(2),r1(3)/1000,r1(4)/1000],frect=[0 0 1,1]/1000,arect=[0,0,0,0])
  xmin=arect(1);xmax=1-arect(2);ymin=-1+arect(4);ymax=-arect(3);
  axleg=gca();
  axleg.clip_state='off';
  axleg.font_style=A.font_style;
  axleg.font_size=A.font_size;
  axleg.foreground=A.foreground
  axleg.background=A.background
  axleg.font_color=A.font_color

  dy=ymax-ymin
  drx=(xmax-xmin)/20 //length of the line

  e=drx/2  // horizontal space between two columns

  bbx=[]
  xset('font',A.font_style,A.font_size)
  for k=1:nleg
    //r=xstringl(0,0,leg(k));
    rect=stringbox(leg(k),0,0);
    // get the width and heigth of the bbox
    r = [ rect(1,3) - rect(1,2), rect(2,2) - rect(2,1)];
    //bbx=[bbx;r(3:4)]; //[width height]
    bbx=[bbx;r]; //[width height]
  end

  coef = 0.5
  height=sum(bbx(:,2))
  dh = coef*height/nleg
  height = height+(nleg-1)*dh + 2*dy/60

  //height=sum(bbx(:,2))+2*dy/60
  width=max(bbx(:,1))+drx*1.2+2*drx/5

  //upper left coordinates
  if size(opt,'*')>1 then 
    // fix for bug 1237 (Bruno 9 march 2005)
    if A.tight_limits == "on" then  // data_bounds' corresponds to the old frec
      re = A.data_bounds'
    else 
      re = [A.x_ticks.locations(1),A.y_ticks.locations(1),...
	    A.x_ticks.locations($),A.y_ticks.locations($)]
    end
    pos(1) = xmin + ((opt(1)-re(1))/(re(3)-re(1)))*(1-arect(1)-arect(2))
    pos(2) = ymin + ((opt(2)-re(2))/(re(4)-re(2)))*(1-arect(3)-arect(4))
    // end bugfix
    opt=0
  elseif opt<-1|opt>5 then 
    error('opt can take value in -1:5')
  end
  select opt
  case -1 then
    opt=1
  case 0 then

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


  drawlater()
  R=[]
  if with_box then 
    // a new modif (bruno 25 april 05): draw a filled rectangle
    // with a white background (so that the legend is not masked
    // by the grid or some curves). Don't know if I have used the
    // shortest way to do that !
    xpol = [pos(1), pos(1)+width, pos(1)+width, pos(1)];
    ypol = [pos(2), pos(2), pos(2)-height, pos(2)-height];     
    xfpoly(xpol, ypol,1)
    R = gce();
    R.foreground=A.foreground;
    R.background=A.background;
    
  end
  for k=1:nleg
    if k<=size(H,'*') then
      h=H(k)
      r = []; rb = [];  // rb is for mark 
      select h.type
      case "Polyline"
	if h.polyline_style==5 then //patch
	  xfpoly([x;x+drx;x+drx;x;x],[y-bbx(k,2);y-bbx(k,2);y;y;y-bbx(k,2)]);r=gce();
	  r = unglue(r); // one xfpoly returns 2 polylines -> tmp bug to fix later F.Leray
	  r.foreground=h.foreground;
	  r.thickness=h.thickness;
	  r.polyline_style=h.polyline_style;
	  r.line_style=h.line_style;
	else
	  // part modified by bruno (25 april 05) *********
	  line_mode = stripblanks(h.line_mode)
	  mark_mode = stripblanks(h.mark_mode)
	  if line_mode == "on" then
	    xpoly([x;x+drx],[y;y]-bbx(k,2)/2,'lines');
	    st=1//h.polyline_style;  //if st==3 then st=0,else st=0;end
	    r = gce();
	    r.line_mode = line_mode;
	    r.mark_mode = "off";
	    r.foreground=h.foreground;
	    r.thickness=h.thickness;
	    r.polyline_style=max(1,st);
	    r.line_style=max(h.line_style,1);
	  end
	  if mark_mode == "on" then
	    xpoly(x+drx/2,y-bbx(k,2)/2);
	    rb = gce();
	    rb.line_mode = "off";
	    rb.mark_mode = mark_mode;
	    rb.mark_mode=h.mark_mode
	    rb.mark_style=h.mark_style;
	    rb.mark_size_unit=h.mark_size_unit;
	    rb.mark_size=h.mark_size;
	    rb.mark_foreground=h.mark_foreground;	    
	    rb.mark_background=h.mark_background;	    	    
	    rb.thickness=h.thickness;
	  end
	  // end of the bruno 's modif ******************** 
	end
      else
	error('Only handle on polyline are allowed')
      end
      R=[R,r',rb]
    end
    xstring(x+drx*1.2,y-bbx(k,2),leg(k))
    r=gce()
    r.foreground = A.foreground;
    R=[R,r]
    y=y-bbx(k,2)-dh
  end
  glue(R)
  R=gce()
  draw(R)
  set('current_axes',Acur),
  //  drawnow()
  f.immediate_drawing=vis;
endfunction

function h=getvalidchildren(A)
  h=[]
  for k=1:size(A,'*')
    a=A(k)
    select a.type
    case "Polyline" then
      h=[h;a]
     case 'Axes'
      ax=a.children
      h=[h;getvalidchildren(ax($:-1:1))]
    case 'Compound'
      for k=size(a.children,'*'):-1:1
	h=[h;getvalidchildren(a.children(k))]

      end
    end
  end
endfunction
