function drawobj(o)
// Copyright INRIA

  if typeof(o)=='Block' then
    ierr=execstr(o.gui+'(''plot'',o);','errcatch')
    if ierr<>0 then 
      message(['Block '+o.gui+ ' not defined';
	       'You must leave scicos and define it now.']),
    end
  elseif typeof(o)=='Link' then
    if o.thick(2)>=0 then
      thick=xget('thickness');d=xget('dashes')
      xset('thickness',maxi(o.thick(1),1)*maxi(o.thick(2),1))
      xset('dashes',o.ct(1))
      xpoly(o.xx,o.yy,'lines')
      xset('dashes',d)
      xset('thickness',thick)
    end
  elseif typeof(o)=='Text' then
    execstr(o.gui+'(''plot'',o)')
  end
endfunction
