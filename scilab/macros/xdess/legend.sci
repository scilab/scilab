function R=legend(varargin)
  if get('figure_style')=='old' then
    error('This function cannot be used with old style graphics, see legends')
  end
  f=gcf()
  vis=f.visible;
  narg=size(varargin)
  k0=1
  H=[]
  leg=[]
  Acur=gca();A=Acur;
  opt=1
  with_box=%t
  
  if type(varargin(1))==9 then //a handle
    H=varargin(1)
    if H.type=='Axes' then 
      A=H,
      //set("current_axes",A)
      k0=k0+1
      if k0<=narg&type(varargin(k0))==9 then //a handle
	H=varargin(k0);k0=k0+1
      end
    end
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
  
  
  r2=A.data_bounds;
  arect=A.margins;
  r1=A.axes_bounds;
  //create small axes on the top left corner (the axes is choosen very
  //small to avoid it can be selected for rotation in new graphic mode
  //case (a little tricky)
  xsetech(wrect=[r1(1),r1(2),r1(3)/1000,r1(4)/1000],frect=[0 0 1,1]/1000,arect=[0,0,0,0])
  xmin=arect(1);xmax=1-arect(2);ymin=-1+arect(4);ymax=-arect(3);
  axleg=gca();
  axleg.clip_state='off';
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
  if with_box then xrect(pos(1),pos(2),width,height),R=gce();end
  for k=1:nleg
    if k<=size(H,'*') then
      h=H(k)
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
	    st=h.polyline_style;
	    if st==3 then st=0,else st=0;end
	    xpoly([x;x+drx],[y;y]-bbx(k,2)/2,'lines');r=gce();
	    r.foreground=h.foreground;
	    r.thickness=h.thickness;
	    r.polyline_style=st;
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
	error('Only handle on polyline are allowed')
      end
    R=[R,r]
    end
    xstring(x+drx*1.2,y-bbx(k,2),leg(k))
    r=gce()
    R=[R,r]
    y=y-bbx(k,2)-dh
  end
  glue(R)
  R=gce()
  draw(R)
  set('current_axes',Acur),
  drawnow()
  f.visible=vis;
endfunction
function h=getvalidchildren(A)
  h=[]
  for k=1:size(A,'*')
    a=A(k)
    select a.type
      case "Polyline" then
      h=[h;a]
      case 'Axes'
      h=[h;getvalidchildren(a.children)]
      case 'Agregation'
      for k=1:size(a.children,'*')
	h=[h;getvalidchildren(a.children(k))]
      end
    end
  end
endfunction
