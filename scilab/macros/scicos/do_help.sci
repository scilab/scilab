function do_help()
// Copyright INRIA

  while %t do
    [btn,xc,yc,cwin,Cmenu]=cosclick(0)
    if Cmenu<>[] then
      name=Cmenu
      nm=1
      break
    elseif cwin==curwin then 
      k=getobj(scs_m,[xc;yc])
      if k<>[] then
	o=scs_m.objs(k)
	name=o.gui
	nm=0
	break
      end
    elseif or(windows(find(windows(:,1)<0),2)==cwin) then
      kwin=find(windows(:,2)==cwin)
      pal=palettes(-windows(kwin,1))
      k=getobj(pal,[xc;yc])
      if k<>[] then
	o=pal(k)
	name=o.gui
	nm=0
	break
      end
    end
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
