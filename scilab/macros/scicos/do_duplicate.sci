function [scs_m,needcompile]=do_duplicate(%pt,scs_m,needcompile)
// Copyright INRIA
  xinfo('Click where you want object to be placed (right-click to cancel)')
  win=%win;
  xc=%pt(1);yc=%pt(2);
  kc=find(win==windows(:,2))
  if kc==[] then
    message('This window is not an active palette')
    k=[]
  elseif windows(kc,1)<0 then //click dans une palette
    kpal=-windows(kc,1)
    palette=palettes(kpal)
    k=getblocktext(palette,[xc;yc])
    if k<>[] then 
      o=disconnect_ports(palette.objs(k)),
    end
  elseif win==curwin then //click dans la fenetre courante
    k=getblocktext(scs_m,[xc;yc])
    if k<>[] then
      o=disconnect_ports(scs_m.objs(k)) // mark ports disconnected
    end
  elseif slevel>1 then
    execstr('k=getblocktext(scs_m_'+string(windows(kc,1))+',[xc;yc])')
    if k<>[] then
      execstr('o=scs_m_'+string(windows(kc,1))+'.objs(k)')
      o=disconnect_ports(o)//mark ports disconnected
    end
  else
    message('This window is not an active palette')
    k=[]
  end
  
  
  if k<>[] then
    xset('window',curwin);
    rep(3)=-1
    [xy,sz]=(o.graphics.orig,o.graphics.sz)
    dr=driver()
    if dr=='Rec' then driver('X11'),end
    while rep(3)==-1 then //move loop
      // draw block shape
      xrect(xc,yc+sz(2),sz(1),sz(2))
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
    driver(dr)

    if xget('window')<>curwin then
      //active window has been closed
      [%win,Cmenu]=resume(curwin,'Quit')
    end
    // update and draw block
    if or(rep(3)==[2 5]) then
      if pixmap then xset('wshow'),end
      return
    end
    o.graphics.orig=xy
    drawobj(o)
    if pixmap then xset('wshow'),end
    scs_m_save=scs_m,nc_save=needcompile
    scs_m.objs($+1)=o
    needcompile=4
    [scs_m_save,nc_save,enable_undo,edited]=resume(scs_m_save,nc_save,%t,%t)
  end
endfunction

