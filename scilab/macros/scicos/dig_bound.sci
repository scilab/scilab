function [rect]=dig_bound(objs)
// Copyright INRIA
n=size(objs)
if n<=1 then rect=[];return;end
xmin=100000;
xmax=-xmin;
ymin=xmin;
ymax=-xmin;

for i=2:n //loop on objects
  o=objs(i)
  if o(1)=='Block'|o(1)=='Text' then
    [orig,sz]=o(2)(1:2)
    xmin=min(xmin,orig(1))
    xmax=max(xmax,orig(1)+sz(1))
    ymin=min(ymin,orig(2))
    ymax=max(ymax,orig(2)+sz(2))
  elseif o(1)=='Link' then
    xx=o(2)(:);yy=o(3)(:);
    xmin=min([xmin;xx])
    xmax=max([xmax;xx])
    ymin=min([ymin;yy])
    ymax=max([ymax;yy])
  end
end
rect=[xmin,ymin,xmax,ymax]




