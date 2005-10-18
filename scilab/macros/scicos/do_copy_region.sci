function [scs_m,needcompile]=do_copy_region(scs_m,needcompile)
  Cmenu='Copy Region'  // to copy_region from palette
  [btn,%pt,win,Cmenu]=cosclick()
  if Cmenu<>[] then
    Cmenu=resume(Cmenu)
  end
  xc=%pt(1);yc=%pt(2);
  disablemenus()
  [reg,rect]=get_region(xc,yc,win)
  // Copyright INRIA
  if rect==[] then enablemenus();return,end
  modified=length(reg)>1


  xinfo('Drag to destination position and click (left to fix, right to cancel)')
  rep(3)=-1
  yc=yc-rect(4)  
  dr=driver()
  if dr=='Rec' then driver('X11'),end
  pat=xget('pattern')
  xset('pattern',default_color(0))
  //move loop
  while rep(3)==-1 then 
    // draw block shape
    xrect(xc,yc+rect(4),rect(3),rect(4))
    if pixmap then xset('wshow'),end
    // get new position
    rep=xgetmouse()
    if rep(3)==-100 then //active window has been closed
      driver(dr);
      [%win,Cmenu]=resume(curwin,'Quit')
    end
    // erase block shape
    xrect(xc,yc+rect(4),rect(3),rect(4))
    if pixmap then xset('wshow'),end
    xc=rep(1);yc=rep(2)
    xy=[xc,yc];
  end
  if xget('window')<>curwin then
    //active window has been closed
    [%win,Cmenu]=resume(curwin,'Quit')
  end

  driver(dr);xset('pattern',pat)
  if or(rep(3)==[2 5]) then enablemenus();return,end

  scs_m_save=scs_m,nc_save=needcompile
  n=lstsize(scs_m.objs)
  for k=1:size(reg.objs)
    o=reg.objs(k)
    // translate blocks and update connection index 
    if typeof(o)=='Link' then
      o.xx=o.xx-rect(1)+xc
      o.yy=o.yy-rect(2)+yc
      [from,to]=(o.from,o.to)
      o.from(1)=o.from(1)+n;
      o.to(1)=o.to(1)+n;
    elseif typeof(o)=='Block' then
      o.graphics.orig(1)=o.graphics.orig(1)-rect(1)+xc
      o.graphics.orig(2)=o.graphics.orig(2)-rect(2)+yc
      k_conn=find(o.graphics.pin>0)
      o.graphics.pin(k_conn)=o.graphics.pin(k_conn)+n
      k_conn=find(o.graphics.pout>0)
      o.graphics.pout(k_conn)=o.graphics.pout(k_conn)+n
      k_conn=find(o.graphics.pein>0)
      o.graphics.pein(k_conn)=o.graphics.pein(k_conn)+n
      k_conn=find(o.graphics.peout>0)
      o.graphics.peout(k_conn)=o.graphics.peout(k_conn)+n
    elseif typeof(o)=='Text' then
      o.graphics.orig(1)=o.graphics.orig(1)-rect(1)+xc
      o.graphics.orig(2)=o.graphics.orig(2)-rect(2)+yc
    end
    scs_m.objs($+1)=o
    drawobj(o)
  end
  if modified then 
    needcompile=4,
    enablemenus()
    [scs_m_save,nc_save,enable_undo,edited]=resume(scs_m_save,nc_save,%t,%t)
  end
  enablemenus()
endfunction
