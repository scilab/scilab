function [%pt,scs_m]=do_move(%pt,scs_m)
// Copyright INRIA
// get a scicos object to move, and move it with connected objects
//!
//get block to move
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
    [k,wh]=getobj(scs_m,[xc;yc])
    if k<>[] then break,end
  end

  scs_m_save=scs_m
  if typeof(scs_m.objs(k))=='Block'|typeof(scs_m.objs(k))=='Text' then
    needreplay=replayifnecessary()
    scs_m=moveblock(scs_m,k,xc,yc)
  elseif typeof(scs_m.objs(k))=='Link' then
    if wh>0 then
      scs_m=movelink(scs_m,k,xc,yc,wh)
    else
      scs_m=movecorner(scs_m,k,xc,yc,wh)
    end
  end
    if Cmenu=='Quit' then
      //active window has been closed
      [%win,Cmenu]=resume(%win,Cmenu)
  end
  [scs_m_save,enable_undo,edited,nc_save,needreplay]=resume(..
		       scs_m_save,%t,%t,needcompile,needreplay)

endfunction

function scs_m=moveblock(scs_m,k,xc,yc)
// Copyright INRIA  
// Move  block k and modify connected links if any
//!
//look at connected links
  dr=driver()
  connected=unique(get_connected(scs_m,k))
  o=scs_m.objs(k)
  xx=[];yy=[];ii=[];clr=[];mx=[];my=[]

  // build movable segments for all connected links
  //===============================================
  for i=connected
    oi=scs_m.objs(i)
    driver(dr)
    drawobj(oi) //erase link
    if pixmap then xset('wshow'),end
    // [xl,yl,ct,from,to]=oi([2,3,7:9])
    [xl,yl,ct,from,to]=(oi.xx,oi.yy,oi.ct,oi.from,oi.to)
   
    nl=prod(size(xl))
    
    if dr=='Rec' then driver('X11'),end
    xpolys(xl,yl,ct(1))// redraw thin link
    clr=[clr ct(1)]
    
    if from(1)==k&to(1)==k then
      //move all link
      ii=[ii 0]
    elseif from(1)==k then
      ii=[ii 1]
      // build movable segments for this link
      if nl>=4 then
	x1=xl(1:4)
	y1=yl(1:4)
      elseif nl==3 then 
	// 3 points link add one point at the begining
	x1=xl([1 1:3])
	y1=yl([1 1:3])
      elseif xl(1)==xl(2)|yl(1)==yl(2) then 
	// vertical or horizontal 2 points link, add a point in the middle
	x1=[xl(1);xl(1)+(xl(2)-xl(1))/2;xl(1)+(xl(2)-xl(1))/2;xl(2)]
	y1=[yl(1);yl(1)+(yl(2)-yl(1))/2;yl(1)+(yl(2)-yl(1))/2;yl(2)]
      else
	// oblique 2 points link add 2 points in the middle
	x1=[xl(1);xl(1)+(xl(2)-xl(1))/2;xl(1)+(xl(2)-xl(1))/2;xl(2)]
	y1=[yl(1);yl(1);yl(2);yl(2)]
      end
      //set allowed (x or y) move for each points of build movable segments
      if nl==3 then
	if xl(1)==xl(2) then 
	  mx=[mx,[1;1;1;0]]
	  my=[my,[1;1;0;0]]
	else
	  mx=[mx,[1;1;0;0]]
	  my=[my,[1;1;1;0]]
	end
      else
	if xl(1)==xl(2) then
	  mx=[mx,[1;1;0;0]]
	  my=[my,[1;1;1;0]]
	else
	  mx=[mx,[1;0;0;0]]
	  my=[my,[1;1;0;0]]
	end
      end
      xx=[xx x1];yy=[yy y1]  //store  movable segments for this link
    else
      ii=[ii -1]
      // build movable segments
      if nl>=4 then
	x1=xl(nl:-1:nl-3)
	y1=yl(nl:-1:nl-3)
      elseif nl==3 then 
	// 3 points link add one point at the end
	sel=[nl:-1:nl-2,nl-2]
	x1=xl([nl nl:-1:nl-2])
	y1=yl([nl nl:-1:nl-2])
      elseif xl(1)==xl(2)|yl(1)==yl(2) then 
	// vertical or horizontal 2 points link add a point in the middle
	xm=xl(2)+(xl(1)-xl(2))/2
	x1= [xl(2);xm;xm;xl(1)]
	ym=yl(2)+(yl(1)-yl(2))/2;
	y1= [yl(2);ym;ym;yl(1)]
      else
	// oblique 2 points link add 2 points in the middle
	xm=xl(2)+(xl(1)-xl(2))/2
	x1=[xl(2);xm;xm;xl(1)]
	y1=[yl(2);yl(2);yl(1);yl(1)]
      end
      if nl==3 then
	if x1(2)==x1(3) then 
	  mx=[mx,[1;1;1;0]]
	  my=[my,[1;1;0;0]]
	else
	  mx=[mx,[1;1;0;0]]
	  my=[my,[1;1;1;0]]
	end
      else
	if x1(1)==x1(2) then
	  mx=[mx,[1;1;0;0]]
	  my=[my,[1;1;1;0]]
	else
	  mx=[mx,[1;0;0;0]]
	  my=[my,[1;1;0;0]]
	end
      end
      xx=[xx x1];yy=[yy y1] //store  movable segments for this link
    end
  end

  // move a block and connected links
  //=================================
  [mxx,nxx]=size(xx)
  if connected<>[] then // move a block and connected links
    [xmin,ymin]=getorigin(o)
    xc=xmin;yc=ymin
    rep(3)=-1
    [xy,sz]=(o.graphics.orig,o.graphics.sz)
    // clear block
    driver(dr)
    drawobj(o)
    if dr=='Rec' then driver('X11'),end
    full_move=connected(find(ii==0))

    xpolys(xx+mx*(xc-xmin),yy+my*(yc-ymin),clr)// erase moving part of  links
    for i=full_move
      oi=scs_m.objs(i)
      xpolys(oi.xx+(xc-xmin),oi.yy+(yc-ymin),oi.ct(1))// erase  links
    end
    
    pat=xget('pattern')
    xset('pattern',default_color(0))
    while rep(3)==-1 ,  // move loop
			
      // draw block shape
      xrect(xc,yc+sz(2),sz(1),sz(2))
      // draw moving links
      xpolys(xx+mx*(xc-xmin),yy+my*(yc-ymin),clr)
      for i=full_move
	oi=scs_m.objs(i)
	xpolys(oi.xx+(xc-xmin),oi.yy+(yc-ymin),oi.ct(1))
      end

      // get new position
      if pixmap then xset('wshow'),end    
      rep=xgetmouse( 0);
      if rep(3)==-100 then //active window has been closed
	driver(dr);
	[%win,Cmenu]=resume(curwin,'Quit')
      end
  
      // clear block shape
      xrect(xc,yc+sz(2),sz(1),sz(2))
      // clear moving part of links
      xpolys(xx+mx*(xc-xmin),yy+my*(yc-ymin),clr)
      for i=full_move
	oi=scs_m.objs(i)
	xpolys(oi.xx+(xc-xmin),oi.yy+(yc-ymin),oi.ct(1))
      end

      xc=rep(1);yc=rep(2)
      xy=[xc,yc];
    end

    if xget('window')<>curwin then
      //active window has been closed
      [%win,Cmenu]=resume(curwin,'Quit')
    end

    xpolys(xx+mx*(xc-xmin),yy+my*(yc-ymin),clr) 
    for i=full_move
      oi=scs_m.objs(i)
      xpolys(oi.xx+(xc-xmin),oi.yy+(yc-ymin),oi.ct(1))
    end

    xset('pattern',pat)
  
    // update and draw block
    if and(rep(3)<>[2 5]) then o.graphics.orig=xy;  scs_m.objs(k)=o;end
    driver(dr)
    drawobj(o)
    if pixmap then xset('wshow'),end
    //udate moved links in scicos structure
    xx=xx+mx*(xc-xmin)
    yy=yy+my*(yc-ymin)
    i=0;
    for i1=1:prod(size(ii))
      oi=scs_m.objs(connected(i1))
      xl=oi.xx;yl=oi.yy;nl=prod(size(xl))
      if ii(i1)>0 then
	i=i+1
	if nl>=4 then
	  xl(1:4)=xx(:,i)
	  yl(1:4)=yy(:,i)
	elseif nl==3 then
	  xl=xx(2:4,i)
	  yl=yy(2:4,i)
	else
	  xl=xx(:,i)
	  yl=yy(:,i)
	end
      elseif ii(i1)<0 then
	i=i+1
	if nl>=4 then
	  xl(nl-3:nl)=xx(4:-1:1,i)
	  yl(nl-3:nl)=yy(4:-1:1,i)
	elseif nl==3 then
	  xl=xx(4:-1:2,i)
	  yl=yy(4:-1:2,i)
	else
	  xl=xx(4:-1:1,i)
	  yl=yy(4:-1:1,i)
	end
      else //full move
	xl=xl+(xc-xmin)
	yl=yl+(yc-ymin)
      end
      nl=prod(size(xl))
      //eliminate double points
      kz=find((xl(2:nl)-xl(1:nl-1))^2+(yl(2:nl)-yl(1:nl-1))^2==0)
      xl(kz)=[];yl(kz)=[]
      //store
      if dr=='Rec' then driver('X11'),end
      xpolys(xl,yl,oi.ct(1))// erase thin link
      if  and(rep(3)<>[2 5]) then
	oi.xx=xl;oi.yy=yl;
	scs_m.objs(connected(i1))=oi;
      end
      driver(dr)
      drawobj(oi)  //draw final link
    end
    if pixmap then xset('wshow'),end
  else // move an unconnected block
    rep(3)=-1
    [xy,sz]=(o.graphics.orig,o.graphics.sz)
    // clear block
    drawobj(o)
    dr=driver()
    if dr=='Rec' then driver('X11'),end
    while rep(3)==-1 , //move loop
      
      // draw block shape
      xrect(xc,yc+sz(2),sz(1),sz(2))
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
      if pixmap then xset('wshow'),end
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

function scs_m=movelink(scs_m,k,xc,yc,wh)
// move the  segment wh of the link k and modify the other segments if necessary
//!
  o=scs_m.objs(k)
  drawobj(o) //erase link
  if pixmap then xset('wshow'),end
  dr=driver()
  if dr=='Rec' then driver('X11'),end
  [xx,yy,ct]=(o.xx,o.yy,o.ct)
  xpolys(xx,yy,ct(1)) //redraw thin  link 
  if pixmap then xset('wshow'),end

  nl=size(o.xx,'*')  // number of link points
  if wh==1 then
    from=o.from;to=o.to;
    if is_split(scs_m.objs(from(1)))&is_split(scs_m.objs(to(1)))&nl<3 then
      scs_m=movelink1(scs_m)
    elseif ~is_split(scs_m.objs(from(1)))|nl<3 then
      p=projaff(xx(1:2),yy(1:2),[xc,yc])
      X1=[xx(1);p(1);xx(2)]
      Y1=[yy(1);p(2);yy(2)]
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

      xpolys(x1,y1,ct(1))//draw moving part of the link
      xx=[xx(1);x1(2);xx(2:$)]
      yy=[yy(1);y1(2);yy(2:$)]
      xpolys(xx,yy,ct(1)) // erase thin link
      if and(rep(3)<>[2 5]) then 
	o.xx=xx;o.yy=yy
	scs_m.objs(k)=o
      end
      driver(dr)
      drawobj(o)
      if pixmap then xset('wshow'),end
    else  // link comes from a split 
      scs_m=movelink2(scs_m)
      if Cmenu=='Quit' then [%win,Cmenu]=resume(curwin,Cmenu),end
    end
  elseif wh>=nl-1 then
    to=o.to
    if ~is_split(scs_m.objs(to(1)))|nl<3 then
      p=projaff(xx($-1:$),yy($-1:$),[xc,yc])
      X1=[xx($-1);p(1);xx($)]
      Y1=[yy($-1);p(2);yy($)]
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

      xpolys(x1,y1,ct(1))//draw moving part of the link
      xx=[xx(1:$-1);x1(2);xx($)]
      yy=[yy(1:$-1);y1(2);yy($)]
      xpolys(xx,yy,ct(1)) // erase thin link
      if and(rep(3)<>[2 5]) then 
	o.xx=xx;o.yy=yy
	scs_m.objs(k)=o
      end
      driver(dr)
      drawobj(o)
      if pixmap then xset('wshow'),end
    else // link goes to a split 
      scs_m=movelink3(scs_m)
      if Cmenu=='Quit' then [%win,Cmenu]=resume(curwin,Cmenu),end
    end
  elseif nl<4 then
    p=projaff(xx(wh:wh+1),yy(wh:wh+1),[xc,yc])
    X1=[xx(wh);p(1);xx(wh+1)]
    Y1=[yy(wh);p(2);yy(wh+1)]
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

    xpolys(x1,y1,ct(1))//draw moving part of the link
    xx=[xx(1:wh);x1(2);xx(wh+1:$)]
    yy=[yy(1:wh);y1(2);yy(wh+1:$)]
    xpolys(xx,yy,ct(1)) // erase thin link
    if and(rep(3)<>[2 5]) then
      o.xx=xx;o.yy=yy
      scs_m.objs(k)=o
    end
    driver(dr)
    drawobj(o)
    if pixmap then xset('wshow'),end
  else
    scs_m=movelink4(scs_m)
    if Cmenu=='Quit' then [%win,Cmenu]=resume(curwin,Cmenu),end
  end
endfunction

function scs_m=movelink4(scs_m)
  o;
  e=[min(yy(wh:wh+1))-max(yy(wh:wh+1)),min(xx(wh:wh+1))-max(xx(wh:wh+1))];
  e=e/norm(e)
  X1=xx(wh-1:wh+2);
  Y1=yy(wh-1:wh+2);
  x1=X1;y1=Y1;
  xpolys(x1,y1,ct(1)) //erase moving part of the link
  rep(3)=-1
  while rep(3)==-1 do
    xpolys(x1,y1,ct(1))//draw moving part of the link
    if pixmap then xset('wshow'),end
    rep=xgetmouse(0);
    if rep(3)==-100 then //active window has been closed
      driver(dr);
      [%win,Cmenu]=resume(curwin,'Quit')
    end
    xpolys(x1,y1,ct(1))//erase moving part of the link
    xc1=rep(1);yc1=rep(2)
    x1(2:3)=X1(2:3)+e(1)*(xc-xc1)
    y1(2:3)=Y1(2:3)+e(2)*(yc-yc1)
  end
  //erase rest of the link
  if xget('window')<>curwin then
    //active window has been closed
    [%win,Cmenu]=resume(curwin,'Quit')
  end

  xpolys(xx(1:wh-1),yy(1:wh-1),ct(1))
  xpolys(xx(wh+2:$),yy(wh+2:$),ct(1))
  if and(rep(3)<>[2 5]) then 
    o.xx(wh-1:wh+2)=x1;o.yy(wh-1:wh+2)=y1;
    scs_m.objs(k)=o
  end
  driver(dr)
  drawobj(o)
  if pixmap then xset('wshow'),end
endfunction

function scs_m=movelink1(scs_m)
  o;
  //link between to splits
  e=[min(yy)-max(yy),min(xx)-max(xx)];
  e=e/norm(e)
  xpolys(xx,yy,ct(1))//erase  the link
  X1=xx;Y1=yy
  rep(3)=-1
  while rep(3)==-1 do
    xpolys(xx,yy,ct(1))  //draw  the link
    if pixmap then xset('wshow'),end
    rep=xgetmouse(0);
    if rep(3)==-100 then //active window has been closed
      driver(dr);
      [%win,Cmenu]=resume(curwin,'Quit')
    end
    xpolys(xx,yy,ct(1)) //erase moving part of the link
    xc1=rep(1);yc1=rep(2)
    xx=X1+e(1)*(xc-xc1)
    yy=Y1+e(2)*(yc-yc1)
  end
  if xget('window')<>curwin then
    //active window has been closed
    [%win,Cmenu]=resume(curwin,'Quit')
  end

  if and(rep(3)<>[2 5]) then o.xx=xx;o.yy=yy;end
  scs_m.objs(k)=o
  driver(dr)
  drawobj(o)
  if pixmap then xset('wshow'),end
  if or(rep(3)==[2 5]) then return,end

  //move split block and update other connected links
  connected=[get_connected(scs_m,from(1)),get_connected(scs_m,to(1))]

  for j=find(connected<>k),
    drawobj(scs_m.objs(connected(j))),//erase  other connected links
  end
  drawobj(scs_m.objs(from(1)))//erase split
  drawobj(scs_m.objs(to(1)))//erase split
  
  // change links
  if connected(1)<>k then
    //update links coordinates
    o=scs_m.objs(connected(1));
    if size(o.xx,'*')>2 then
      if o.xx($)==o.xx($-1) then
	o.xx($-1:$)=o.xx($-1:$)+e(1)*(xc-xc1);
	o.yy($)=o.yy($)+e(2)*(yc-yc1);
      elseif o.yy($)==o.yy($-1) then
	o.xx($)=o.xx($)+e(1)*(xc-xc1);
	o.yy($-1:$)=o.yy($-1:$)+e(2)*(yc-yc1);
      else
	o.xx($)=o.xx($)+e(1)*(xc-xc1);
	o.yy($)=o.yy($)+e(2)*(yc-yc1);
      end
    else
      o.xx($)=o.xx($)+e(1)*(xc-xc1);
      o.yy($)=o.yy($)+e(2)*(yc-yc1);
    end
    scs_m.objs(connected(1))=o;
    drawobj(o) //redraw link
  end
  for kk=2:size(connected,'*')
    if connected(kk)<>k then
      //update links coordinates
      o=scs_m.objs(connected(kk))
      if size(o.xx,'*')>2 then
	if o.xx(1)==o.xx(2) then
	  o.xx(1:2)=o.xx(1:2)+e(1)*(xc-xc1)
	  o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
	elseif o.yy(1)==o.yy(2) then
	  o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	  o.yy(1:2)=o.yy(1:2)+e(2)*(yc-yc1)
	else
	  o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	  o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
	end
      else
	o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
      end
      scs_m.objs(connected(kk))=o;
      drawobj(o)//redraw link
    end
  end
  //update split coordinates
  o=scs_m.objs(from(1))
  o.graphics.orig(1)=o.graphics.orig(1)+e(1)*(xc-xc1);
  o.graphics.orig(2)=o.graphics.orig(2)+e(2)*(yc-yc1);
  drawobj(o)//redraw split
  scs_m.objs(from(1))=o
  o=scs_m.objs(to(1))
  o.graphics.orig(1)=o.graphics.orig(1)+e(1)*(xc-xc1);
  o.graphics.orig(2)=o.graphics.orig(2)+e(2)*(yc-yc1);
  drawobj(o)//redraw split
  scs_m.objs(to(1))=o

endfunction

function scs_m=movelink2(scs_m)
  o;
  e=[min(yy(1:2))-max(yy(1:2)),min(xx(1:2))-max(xx(1:2))];
  e=e/norm(e)
  X1=xx(1:3)
  Y1=yy(1:3)
  x1=X1;y1=Y1;
  xpolys(x1,y1,ct(1))//erase  moving part of the link
  if pixmap then xset('wshow'),end
  rep(3)=-1
  while rep(3)==-1 do
    xpolys(x1,y1,ct(1))  //draw moving part of the link
    if pixmap then xset('wshow'),end
    rep=xgetmouse(0);
    if rep(3)==-100 then //active window has been closed
      driver(dr)
      [%win,Cmenu]=resume(curwin,'Quit')
    end
    xpolys(x1,y1,ct(1)) //erase moving part of the link
    xc1=rep(1);yc1=rep(2)
    x1(1:2)=X1(1:2)+e(1)*(xc-xc1)
    y1(1:2)=Y1(1:2)+e(2)*(yc-yc1)
  end
  if xget('window')<>curwin then
    //active window has been closed
    [%win,Cmenu]=resume(curwin,'Quit')
  end

  xpolys(xx(3:$),yy(3:$),ct(1)) // erase rest of initial  link
  if pixmap then xset('wshow'),end
  if and(rep(3)<>[2 5]) then
    o.xx(1:3)=x1;o.yy(1:3)=y1;
  end
  scs_m.objs(k)=o
  driver(dr)
  drawobj(o)
  if pixmap then xset('wshow'),end
  if or(rep(3)==[2 5]) then return,end

  //move split block and update other connected links
  connected=get_connected(scs_m,from(1))

  for j=find(connected<>k),
    drawobj(scs_m.objs(connected(j))),//erase  other connected links
  end
  drawobj(scs_m.objs(from(1)))//erase split
			 
  // change links
  if connected(1)<>k then
    //update links coordinates
    o=scs_m.objs(connected(1));
    if size(o.xx,'*')>2 then
      if o.xx($)==o.xx($-1) then
	o.xx($-1:$)=o.xx($-1:$)+e(1)*(xc-xc1);
	o.yy($)=o.yy($)+e(2)*(yc-yc1);
      elseif o.yy($)==o.yy($-1) then
	o.xx($)=o.xx($)+e(1)*(xc-xc1);
	o.yy($-1:$)=o.yy($-1:$)+e(2)*(yc-yc1);
      else
	o.xx($)=o.xx($)+e(1)*(xc-xc1);
	o.yy($)=o.yy($)+e(2)*(yc-yc1);
      end
    else
      o.xx($)=o.xx($)+e(1)*(xc-xc1);
      o.yy($)=o.yy($)+e(2)*(yc-yc1);
    end
    scs_m.objs(connected(1))=o;
    drawobj(o) //redraw link
  end
  for kk=2:size(connected,'*')
    if connected(kk)<>k then
      //update links coordinates
      o=scs_m.objs(connected(kk))
      if size(o.xx,'*')>2 then
	if o.xx(1)==o.xx(2) then
	  o.xx(1:2)=o.xx(1:2)+e(1)*(xc-xc1)
	  o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
	elseif o.yy(1)==o.yy(2) then
	  o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	  o.yy(1:2)=o.yy(1:2)+e(2)*(yc-yc1)
	else
	  o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	  o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
	end
      else
	o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
      end
      scs_m.objs(connected(kk))=o;
      drawobj(o)//redraw link
    end
  end
  //update split coordinates
  o=scs_m.objs(from(1))
  o.graphics.orig(1)=o.graphics.orig(1)+e(1)*(xc-xc1);
  o.graphics.orig(2)=o.graphics.orig(2)+e(2)*(yc-yc1);
  drawobj(o)//redraw split
  scs_m.objs(from(1))=o
  if pixmap then xset('wshow'),end
endfunction

function scs_m=movelink3(scs_m)
  o;
  e=[min(yy($-1:$))-max(yy($-1:$)),min(xx($-1:$))-max(xx($-1:$))];
  e=e/norm(e)
  X1=xx($-2:$)
  Y1=yy($-2:$)
  x1=X1;y1=Y1;
  xpolys(x1,y1,ct(1)) //erase moving part of the link
  rep(3)=-1
  while rep(3)==-1 do
    xpolys(x1,y1,ct(1))//draw moving part of the link
    rep=xgetmouse(0);
    if rep(3)==-100 then //active window has been closed
      driver(dr)
      [%win,Cmenu]=resume(curwin,'Quit')
    end
 
    xpolys(x1,y1,ct(1))//erase moving part of the link
    xc1=rep(1);yc1=rep(2)
    x1($-1:$)=X1($-1:$)+e(1)*(xc-xc1)
    y1($-1:$)=Y1($-1:$)+e(2)*(yc-yc1)
    if pixmap then xset('wshow'),end
  end
  if xget('window')<>curwin then
    //active window has been closed
    [%win,Cmenu]=resume(curwin,'Quit')
  end

  xpolys(xx(1:$-2),yy(1:$-2),ct(1))//erase rest of the link
  if pixmap then xset('wshow'),end
  if and(rep(3)<>[2 5])  then
    o.xx($-2:$)=x1;o.yy($-2:$)=y1;
    scs_m.objs(k)=o
  end
  driver(dr)
  drawobj(o)
  if pixmap then xset('wshow'),end
  if or(rep(3)==[2 5]) then return,end

  //move split block and update other connected links
  connected=get_connected(scs_m,to(1))
  for j=find(connected<>k),
    drawobj(scs_m.objs(connected(j))),//erase connected links
  end
  drawobj(scs_m.objs(to(1))) //erase split

  for kk=2:size(connected,'*')
    //update links coordinates
    o=scs_m.objs(connected(kk))
    if size(o.xx,'*')>2 then
      if o.xx(1)==o.xx(2) then
	o.xx(1:2)=o.xx(1:2)+e(1)*(xc-xc1)
	o.yy(1)=o.yy(1)+e(2)*(yc-yc1) 
      elseif o.yy(1)==o.yy(2) then 
	o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	o.yy(1:2)=o.yy(1:2)+e(2)*(yc-yc1)
      else
	o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
	o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
      end
    else
      o.xx(1)=o.xx(1)+e(1)*(xc-xc1)
      o.yy(1)=o.yy(1)+e(2)*(yc-yc1)
    end
    scs_m.objs(connected(kk))=o;
    drawobj(o) // redraw connected links
  end
  o=scs_m.objs(to(1))
  o.graphics.orig(1)=o.graphics.orig(1)+e(1)*(xc-xc1);
  o.graphics.orig(2)=o.graphics.orig(2)+e(2)*(yc-yc1);
  drawobj(o) //redraw split
  scs_m.objs(to(1))=o
  if pixmap then xset('wshow'),end

endfunction

function scs_m=movecorner(scs_m,k,xc,yc,wh)
  o=scs_m.objs(k)
  [xx,yy,ct]=(o.xx,o.yy,o.ct)
  if wh==-1|wh==-size(xx,'*') then //link endpoint choosen
    scs_m=movelink(scs_m,k,xc,yc,-wh)
    return
  end
  dr=driver()

  wh=[-wh-1 -wh]
  drawobj(o) //erase link
  if dr=='Rec' then driver('X11'),end
  xpolys(xx,yy,ct(1)) //draw thin link
  if pixmap then xset('wshow'),end
  wh=[wh wh($)+1]
  X1=xx(wh)
  Y1=yy(wh)
  x1=X1;y1=Y1;

  xpolys(x1,y1,ct(1)) //erase moving part of the link
  rep(3)=-1

  while rep(3)==-1 do
    xpolys(x1,y1,ct(1))//draw moving part of the link
    rep=xgetmouse(0);
    if rep(3)==-100 then //active window has been closed
      driver(dr)
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

  [frect1,frect]=xgetech();
  eps=16        //0.04*min(abs(frect(3)-frect(1)),abs(frect(4)-frect(2)))
  if abs(x1(1)-x1(2))<eps then
    x1(2)=x1(1)
  elseif abs(x1(2)-x1(3))<eps then
    x1(2)=x1(3)
  end  
  if abs(y1(1)-y1(2))<eps then
    y1(2)=y1(1)
  elseif abs(y1(2)-y1(3))<eps then
    y1(2)=y1(3)
  end  
  d=projaff([x1(1);x1(3)],[y1(1);y1(3)],[x1(2);y1(2)])
  if norm(d(:)-[x1(2);y1(2)])<eps then
    xx(wh)=x1
    yy(wh)=y1
    xx(wh(2))=[]
    yy(wh(2))=[]
    x1(2)=[];y1(2)=[]
  else
    xx(wh)=x1
    yy(wh)=y1
  end
  xpolys(x1,y1,ct(1))//draw moving part of the link
  xpolys(xx,yy,ct(1)) //erase thin link
  if and(rep(3)<>[2 5]) then
    o.xx=xx;o.yy=yy
    scs_m.objs(k)=o
  end
  driver(dr)
  drawobj(o)
  if pixmap then xset('wshow'),end
endfunction
