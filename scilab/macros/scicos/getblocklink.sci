function [k,wh]=getblocklink(scs_m,pt)
// Copyright INRIA
  n=lstsize(scs_m.objs)
  wh=[];
  x=pt(1);y=pt(2)
  data=[]
  k=[]
  eps=4
  for i=1:n //loop on objects
    o=scs_m.objs(i)
    if typeof(o)=='Block' then
      [orig,sz]=(o.graphics.orig,o.graphics.sz)
      orig=orig-eps;sz=sz+2*eps;
      data=[(orig(1)-x)*(orig(1)+sz(1)-x),(orig(2)-y)*(orig(2)+sz(2)-y)]
      if data(1)<0&data(2)<0 then k=i,break,end
    elseif typeof(o)=='Link' then
      xx=o.xx;yy=o.yy;
      [d,ptp,ind]=dist2polyline(xx,yy,pt)
      if d<eps then k=i,wh=ind,break,end
    end
  end
endfunction
