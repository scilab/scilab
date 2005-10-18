function Paste_()
  if %win==curwin & %ppt<>[] then
    if typeof(Clipboard)=='Block' then
      xset("window",curwin)
      scs_m_save=scs_m,nc_save=needcompile
      blk=Clipboard
      blk.graphics.orig=%ppt
      scs_m.objs($+1)=blk
      drawobj(blk)
      edited=%t
      enable_undo=%t
      Select=[size(scs_m.objs),%win]
    elseif  typeof(Clipboard)=='diagram' then
      reg=Clipboard;
      Clipboard=list()  // region is not persistent
      xset("window",curwin)
      if size(reg.objs)>=1 then
	scs_m_save=scs_m,nc_save=needcompile
	n=lstsize(scs_m.objs)
	xc=%ppt(1),yc=%ppt(2)
	rect=dig_bound(reg)
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
	needcompile=4,
	enable_undo=%t
	edited=%t
      end
    end
  end
  Cmenu=[];%pt=[];%ppt=[]
endfunction
