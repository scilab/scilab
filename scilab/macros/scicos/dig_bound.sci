function [rect]=dig_bound(scs_m)
// Copyright INRIA
  n=lstsize(scs_m.objs)
  if n<1 then rect=[];return;end
  xmin=100000;
  xmax=-xmin;
  ymin=xmin;
  ymax=-xmin;
  for i=1:n //loop on objects
    o=scs_m.objs(i)
    if typeof(o)=='Block'|typeof(o)=='Text' then
      if execstr('orig=o.graphics.orig','errcatch')<>0 then pause,end
      sz=o.graphics.sz
      xmin=min(xmin,orig(1))
      xmax=max(xmax,orig(1)+sz(1))
      ymin=min(ymin,orig(2))
      ymax=max(ymax,orig(2)+sz(2))
    elseif typeof(o)=='Link' then
      xx=o.xx(:);yy=o.yy(:);
      xmin=min([xmin;xx])
      xmax=max([xmax;xx])
      ymin=min([ymin;yy])
      ymax=max([ymax;yy])
    end
  end
  rect=[xmin,ymin,xmax,ymax]
endfunction
