function %pt=do_help(%pt)
// Copyright INRIA

  cwin=%win
  if cwin==curwin then
    xc=%pt(1);yc=%pt(2);
    k=getobj(scs_m,[xc;yc])
    if k<>[] then
	o=scs_m.objs(k)
	name=o.gui
	nm=0
    else
      return
    end
  elseif or(windows(find(windows(:,1)<0),2)==cwin) then
    kwin=find(windows(:,2)==cwin)
    pal=palettes(-windows(kwin,1))
    xc=%pt(1);yc=%pt(2);
    k=getobj(pal,[xc;yc])
    if k<>[] then
      o=pal.objs(k)
      name=o.gui
      nm=0
    else
      return
    end
  else
    return
  end

  if nm==0 then
    help(name)
    return
  end

  if execstr('mess=%scicos_help(name)','errcatch') <>0 then
    mess='No help available on this topic. Sorry.';
  end
  message(mess)  
endfunction
