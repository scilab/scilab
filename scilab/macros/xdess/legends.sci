function legends(leg, style, opt, with_box)
//
// PURPOSE
//    draw legends on a plot
// AUTHORS
//    F. Delebecque + slight modif from B. Pincon 
//   
  rhs=argn(2)
  if rhs < 2 then, error("bad number of arguments"), end
  if type(leg) ~= 10 then, error("first arg may a vector of strings"), end
  nleg=size(leg,'*')
  if type(style) ~= 1 then, error("second arg may a vector"), end  
  if size(style,"*") ~= nleg then
     error("first and second arg must have the same number of components")
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

  //preserve current graphic context
  d=driver(); 
  ls=xget('line style')
  clr=xget('color')
  //
  xset('line style',1)
  xset('color',xget('foreground'))
  
  [r1,r2,logflag,arect]=xgetech()
  xsetech(wrect=r1,frect=[0 0 1 1],arect=arect,logflag='nn')
  ymin=0
  ymax=1
  xmin=0
  xmax=1
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
  
  if with_box then
     xrect(pos(1),pos(2),width,height)
  end
  
  x=pos(1)+drx/5
  y=pos(2)-dy/60
  for k=1:nleg
    if style(1,k)<= 0 then
      if size(style,1)==2 then  xset("color",style(2,k));end
      xpolys(x+drx/2,y-bbx(k,2)/2,style(1,k))
      xset('color',clr)
    else
       if size(style,1)==2 then  xset("line style",style(2,k));end
       xsegs([x;x+drx],[y;y]-bbx(k,2)/2,style(1,k))
    end
    xstring(x+drx*1.2,y-bbx(k,2),leg(k))
    y=y-bbx(k,2)-dh
  end
  //reset saved graphic context
  xset('line style',ls)
  xset('color',clr)
  xsetech(wrect=r1,frect=r2,arect=arect,logflag=logflag)
endfunction
