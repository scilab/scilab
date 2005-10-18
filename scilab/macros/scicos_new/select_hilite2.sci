function hilite_image=select_hilite2(Select,SelectRegion)
//
// Copyright INRIA
  if Select<>[] then 
    [junk,win,o]=get_selection(Select)    
    if or(winsid()==win) then
      winback=xget('window')
      xset('window',win)
      alu=xget('alufunction')
      xset('alufunction',6)
    else
      return
    end
    dr=driver()
    if dr=='Rec' then driver('X11'),end
    if typeof(o)=='Block' then
      graphics=o.graphics;
      [orig,sz]=(graphics.orig,graphics.sz)
      thick=xget('thickness')
      thickness=4*thick
      xset('thickness',thickness);
      xrect(orig(1),orig(2)+sz(2),sz(1),sz(2));
      if pixmap then xset('wshow'),end
      xset('thickness',thick);
      
      hilite_image=list(win,'rect',orig,sz,thickness)
    elseif typeof(o)=='Link' then
      if o.thick(2)>=0 then
	thick=xget('thickness');d=xget('dashes')
	thickness=maxi(o.thick(1),1)*maxi(o.thick(2),1)*4,
	xset('thickness',thickness)
	dashes=o.ct(1)
	xset('dashes',dashes)
	xx=o.xx,yy=o.yy,
	xpoly(xx,yy,'lines')
	xset('dashes',d)
	xset('thickness',thick)
	hilite_image=list(win,'poly',xx,yy,dashes,thickness)
      else
	hilite_image=list()
      end
      if pixmap then xset('wshow'),end
    end
    driver(dr)  
    if rhs>1 then 
      xset('alufunction',alu)
      xset('window',winback)
    end
  elseif SelectRegion<>list() then 
    [ox,oy,w,h,win]=SelectRegion(:)
    if or(winsid()==win) then
      winback=xget('window')
      xset('window',win)
      alu=xget('alufunction')
      xset('alufunction',6)
    else
      return
    end
    dr=driver()
    if dr=='Rec' then driver('X11'),end
    thick=xget('thickness')
    thickness=4
    xset('thickness',thickness);
    orig=[ox,oy-h];sz=[w,h]
    xrect(orig(1),orig(2)+sz(2),sz(1),sz(2));
    if pixmap then xset('wshow'),end
    xset('thickness',thick);
    
    hilite_image=list(win,'rect',orig,sz,thickness)
  else
    hilite_image=list()
  end
endfunction
