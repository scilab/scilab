function [%pt,scs_m]=do_stupidmove(%pt,scs_m)
// Copyright INRIA
// get a scicos object to move, and move it with connected objects
//!
  
//get block to move
  rela=.1
  while %t
    if %pt==[] then
      [btn,%pt,win,Cmenu]=cosclick()
      if Cmenu<>[] then
	[%win,Cmenu]=resume(win,Cmenu)
      end
    else
      win=%win;
    end
    xc=%pt(1);yc=%pt(2);%pt=[]
    [k,wh,scs_m]=stupid_getobj(scs_m,[xc;yc])
    if k<>[] then break,end
  end

  scs_m_save=scs_m
  if typeof(scs_m.objs(k))=='Block'| typeof(scs_m.objs(k))=='Text' then
    needreplay=replayifnecessary()
    scs_m=stupid_moveblock(scs_m,k,xc,yc)
  elseif typeof(scs_m.objs(k))=='Link' then
    scs_m=stupid_movecorner(scs_m,k,xc,yc,wh)
  end
  if Cmenu=='Quit' then
    //active window has been closed
    [%win,Cmenu]=resume(%win,Cmenu)
  end
  [scs_m_save,enable_undo,edited,nc_save,needreplay]=resume(..
						  scs_m_save,%t,%t,needcompile,needreplay)
endfunction

function scs_m=stupid_moveblock(scs_m,k,xc,yc)
// Move  block k and modify connected links if any
//look at connected links
  dr=driver()

  o=scs_m.objs(k)
  xx=[];yy=[];ii=[];clr=[];mx=[];my=[]

  // build movable segments for all connected links
  //===============================================
  xm=[];ym=[];  jj=0;
  connected=unique(get_connected(scs_m,k))
  for l=1:length(connected)
    i=connected(l);
    oi=scs_m.objs(i)
    driver(dr)
    if pixmap then xset('wshow'),end
    [xl,yl,ct,from,to]=(oi.xx,oi.yy,oi.ct,oi.from,oi.to)
    
    nl=prod(size(xl))
    if dr=='Rec' then driver('X11'),end
    if from(1)==k then
      xm=[xm,[xl(2);xl(1)]];ym=[ym,[yl(2);yl(1)]];
      draw_link_seg(oi,1:2) //erase link
      clr=[clr ct(1)]
    end
    if to(1)==k then
      xm=[xm,xl($-1:$)];ym=[ym,yl($-1:$)];
      draw_link_seg(oi,$-1:$) //erase link
      clr=[clr ct(1)]
    end
  end


  xmt=xm;ymt=ym;
  // move a block and connected links
  //=================================
  if size(connected,2)>0 then // move a block and connected links
    [xmin,ymin]=getorigin(o)
    xco=xc;yco=yc;
    rep(3)=-1
    [xy,sz]=(o.graphics.orig,o.graphics.sz)
    // clear block
    driver(dr)
    drawobj(o)
    if dr=='Rec' then driver('X11'),end
    dx=xc-xmin;dy=yc-ymin;
    
    while rep(3)==-1 ,  // move loop
      xrect(xc-dx,yc+sz(2)-dy,sz(1),sz(2))// draw block shape
      
      // draw moving links
      xpolys(xmt,ymt,clr)// draw moving part of links get new position
      if pixmap then xset('wshow'),end    
      rep=xgetmouse(0);
      if rep(3)==-100 then //active window has been closed
	driver(dr);
	[%win,Cmenu]=resume(curwin,'Quit')
      end
      // clear block shape
      xrect(xc-dx,yc+sz(2)-dy,sz(1),sz(2))
      // clear moving part of links
      xpolys(xmt,ymt,clr)// erase moving part of links
      xc=rep(1);yc=rep(2)      
      xmt(2,:)=xm(2,:)-xco+xc; ymt(2,:)=ym(2,:)-yco+yc; 
    end

    if xget('window')<>curwin then
      //active window has been closed
      [%win,Cmenu]=resume(curwin,'Quit')
    end
    xy=[xc-dx,yc-dy];
    
    // update and draw block
    if or(rep(3)==[2 5]) then //user cancels move
      driver(dr)
      drawobj(o)
      xpolys(xm,ym,clr)
      if pixmap then xset('wshow'),end
    else
      o.graphics.orig=xy;  scs_m.objs(k)=o; //update block coordinates
      driver(dr)
      drawobj(o)
      if pixmap then xset('wshow'),end
      j=0
      for l=1:length(connected)
	i=connected(l);
	oi=scs_m.objs(i);
	[xl,from,to]=(oi.xx,oi.from,oi.to);
	if from(1)==k then
	  j=j+1
	  oi.xx(1:2)=xmt([2,1],j)
	  oi.yy(1:2)=ymt([2,1],j)
	  draw_link_seg(oi,1:2) //draw link
	end
	if to(1)==k then
	  j=j+1
	  oi.xx($-1:$)=xmt(:,j)
	  oi.yy($-1:$)=ymt(:,j)
	  draw_link_seg(oi,$-1:$)//draw link
	end
	scs_m.objs(i)=oi
	if pixmap then xset('wshow'),end
      end
    end
  else // move an unconnected block
    rep(3)=-1
    [xy,sz]=(o.graphics.orig,o.graphics.sz)
    // clear block
    drawobj(o)
    dr=driver()
    if dr=='Rec' then driver('X11'),end
    while rep(3)==-1 , //move loop
      xrect(xc,yc+sz(2),sz(1),sz(2))// draw block shape
      if pixmap then xset('wshow'),end
      // get new position
      rep=xgetmouse(0)
      if rep(3)==-100 then //active window has been closed
	driver(dr);
	[%win,Cmenu]=resume(curwin,'Quit')
      end
      
      // clear block shape
      xrect(xc,yc+sz(2),sz(1),sz(2))
      xc=rep(1);yc=rep(2)
      xy=[xc,yc];
    end
    if xget('window')<>curwin then
      //active window has been closed
      [%win,Cmenu]=resume(curwin,'Quit')
    end
    // update and draw block
    if and(rep(3)<>[2 5]) then o.graphics.orig=xy,scs_m.objs(k)=o,end
    driver(dr)
    drawobj(o)
    if pixmap then xset('wshow'),end
  end
  
endfunction

function scs_m=stupid_movecorner(scs_m,k,xc,yc,wh)
  o=scs_m.objs(k)
  [xx,yy,ct]=(o.xx,o.yy,o.ct)
  dr=driver()

  seg=[-wh-1:-wh+1]

  if dr=='Rec' then driver('X11'),end

  xpolys(xx(seg),yy(seg),ct(1)) //draw thin link
  if pixmap then xset('wshow'),end
  X1=xx(seg)
  Y1=yy(seg)
  x1=X1;y1=Y1;

  xpolys(x1,y1,ct(1)) //erase moving part of the link
  rep(3)=-1

  while rep(3)==-1 do
    xpolys(x1,y1,ct(1))//draw moving part of the link
    rep=xgetmouse(0);
    if rep(3)==-100 then //active window has been closed
      driver(dr);
      [%win,Cmenu]=resume(curwin,'Quit')
    end
    if pixmap then xset('wshow'),end
    xpolys(x1,y1,ct(1))//erase moving part of the link
    xc1=rep(1);yc1=rep(2)
    x1(2)=X1(2)-(xc-xc1)
    y1(2)=Y1(2)-(yc-yc1)
  end
  if xget('window')<>curwin then
    //active window has been closed
    [%win,Cmenu]=resume(curwin,'Quit')
  end
  if and(rep(3)<>[2 5]) then
    if abs(x1(1)-x1(2))<rela*abs(y1(1)-y1(2)) then
      x1(2)=x1(1)
    elseif abs(x1(2)-x1(3))<rela*abs(y1(2)-y1(3)) then
      x1(2)=x1(3)
    end  
    if abs(y1(1)-y1(2))<rela*abs(x1(1)-x1(2)) then
      y1(2)=y1(1)
    elseif abs(y1(2)-y1(3))<rela*abs(x1(2)-x1(3)) then
      y1(2)=y1(3)
    end  
    d=projaff([x1(1);x1(3)],[y1(1);y1(3)],[x1(2);y1(2)])
    if norm(d(:)-[x1(2);y1(2)])<..
	  rela*max(norm(d(:)-[x1(3);y1(3)]),norm(d(:)-[x1(1);y1(1)])) then
      xx(seg)=x1
      yy(seg)=y1
      xx(seg(2))=[]
      yy(seg(2))=[]
      x1(2)=[];y1(2)=[];seg(3)=[]
    else
      xx(seg)=x1
      yy(seg)=y1
    end
    o.xx=xx;o.yy=yy
    scs_m.objs(k)=o
  end
  driver(dr)
  draw_link_seg(o,seg)
  if pixmap then xset('wshow'),end
endfunction

function [k,wh,scs_m]=stupid_getobj(scs_m,pt)
  n=lstsize(scs_m.objs)
  wh=[];
  x=pt(1);y=pt(2)
  data=[]
  k=[]
  for i=1:n //loop on objects
    o=scs_m.objs(i)
    if typeof(o)=='Block' then
      graphics=o.graphics
      [orig,sz]=(graphics.orig,graphics.sz)
      data=[(orig(1)-x)*(orig(1)+sz(1)-x),(orig(2)-y)*(orig(2)+sz(2)-y)]
      if data(1)<0&data(2)<0 then k=i,break,end
    elseif typeof(o)=='Link' then
      [frect1,frect]=xgetech();
      eps=4     
      xx=o.xx;yy=o.yy;
      [d,ptp,ind]=stupid_dist2polyline(xx,yy,pt,.85)
      if d<eps then 
	if ind==-1 then 
	  k=o.from(1),break,
	elseif ind==-size(xx,1) then 
	  k=o.to(1),break,
	elseif ind>0 then 
	  draw_link_seg(o,[ind,ind+1])
	  o.xx=[xx(1:ind);ptp(1);xx(ind+1:$)];
	  o.yy=[yy(1:ind);ptp(2);yy(ind+1:$)];
	  scs_m.objs(i)=o
	  k=i,wh=-ind-1,break,
	else k=i,wh=ind,draw_link_seg(o,[-ind-1:-ind+1]);break,end
      end
    elseif typeof(o)=='Text' then
      graphics=o.graphics
      [orig,sz]=(graphics.orig,graphics.sz)
      data=[(orig(1)-x)*(orig(1)+sz(1)-x),(orig(2)-y)*(orig(2)+sz(2)-y)]
      if data(1)<0&data(2)<0 then k=i,break,end
    end
  end
endfunction

function [d,pt,ind]=stupid_dist2polyline(xp,yp,pt,pereps)
// computes minimum distance from a point to a polyline
//d    minimum distance to polyline
//pt   coordinate of the polyline closest point
//ind  
//     if negative polyline closest point is a polyline corner:
//        pt=[xp(-ind) yp(-ind)]
//     if positive pt lies on segment [ind ind+1]

  x=pt(1)
  y=pt(2)
  xp=xp(:);yp=yp(:)
  cr=4*sign((xp(1:$-1)-x).*(xp(1:$-1)-xp(2:$))+..
	    (yp(1:$-1)-y).*(yp(1:$-1)-yp(2:$)))+..
     sign((xp(2:$)-x).*(xp(2:$)-xp(1:$-1))+..
          (yp(2:$)-y).*(yp(2:$)-yp(1:$-1)))

  ki=find(cr==5) // index of segments for which projection fall inside
  np=size(xp,'*')
  if ki<>[] then
    //projection on segments
    x=[xp(ki) xp(ki+1)]
    y=[yp(ki) yp(ki+1)]
    dx=x(:,2)-x(:,1)
    dy=y(:,2)-y(:,1)
    d_d=dx.^2+dy.^2
    d_x=( dy.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dx.*(dx*pt(1)+dy*pt(2)))./d_d
    d_y=(-dx.*(-x(:,2).*y(:,1)+x(:,1).*y(:,2))+dy.*(dx*pt(1)+dy*pt(2)))./d_d
    xp=[xp;d_x]
    yp=[yp;d_y]
  end

  zzz=[ones(np,1);zeros(size(ki,'*'),1)]*eps
  zz=[ones(np,1)*pereps;ones(size(ki,'*'),1)]
  [d,k]=min(sqrt((xp-pt(1)).^2+(yp-pt(2)).^2).*zz-zzz) 
  pt(1)=xp(k)
  pt(2)=yp(k)
  if k>np then ind=ki(k-np),else ind=-k,end
endfunction

function draw_link_seg(o,seg)
  if o.thick(2)>=0 then
    d=xget('dashes');thick=xget('thickness')
    t=maxi(o.thick(1),1)*maxi(o.thick(2),1)
    xset('thickness',t);xset('dashes',o.ct(1))
    xpoly(o.xx(seg),o.yy(seg),'lines')
    xset('dashes',d);xset('thickness',thick)
  end
endfunction

